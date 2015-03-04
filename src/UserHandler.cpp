#include "UserHandler.h"
#include "MultiClientServer.h"
#include "TCPServer.h"
#include "mysql.h"
#include "integrated_server_component.h"
#include "packet_creator.h"

#include <string>
#include <cstring>
#include <iterator>
#include <iostream>

DatabaseServer::DatabaseServer(ActiveQueue *c, ActiveMap *connection_logs,
							   ActiveQueue *console, ThreadPool *t, std::string host,
							   std::string port) :
	connection_logs(connection_logs), t(t), s(new MultiClientServer(new TCPServer())),
	db(new MYSQL_Database("mydb")), vm(new Fletcher()), so(new StatusObserver(c, console)),
	dbs(new IntegratedServerComponent(db, s, vm, connection_logs, so)),
	host(host), port(port)
{
	doubtful_sockets.setThreadPool(t, "ds_sched_thread");
	packet_tuple.setThreadPool(t, "pt_sched_thread");
	query_pairs.setThreadPool(t, "qp_sched_thread");

	/* Setup Database Driver */
	dynamic_cast<MYSQL_Database*>(db)->setMysqlDBInfo("tcp://127.0.0.1:3306", "root", "root");
	dynamic_cast<MYSQL_Database*>(db)->connectToDB();
	dynamic_cast<MYSQL_Database*>(db)->setSchema("PWS");

	s->setAcceptHook(
		[this](int s)->void
		{
			if(this)
			{
				this->so->socketDetect();
				this->doubtful_sockets.enqueue(s);
			}
		}
	);

	s->initServer(host, port);
}

DatabaseServer::~DatabaseServer()
{
	if(vm != NULL) delete vm;
	if(s != NULL)
	{
		s->killServer();
		delete s;
	}

	if(db != NULL) delete db;
	if(dbs != NULL) delete dbs;
	if(so != NULL) delete so;
}

void DatabaseServer::run()
{
	// client acceptor thread
	so->serverStarted();
	t->addTask("client_accept_thread", [&]()->void { s->startServer(); }, 1);

	// thread for connection validator
	t->addTask("cv_thread",
		[&]()->void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			Promise<int> live_socket = doubtful_sockets.dequeue();
			int socket = live_socket.result(1000, -1, -1);
			
			if(static_cast<IntegratedServerComponent*>(dbs)->run(socket))
			{
			}
		}, 1
	);

	// thread for db server receiver
	t->addTask("recv_thread",
		[&]()->void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			for(auto& a : *(connection_logs->getMap()))
			{
				AppServerConnectionInfo &asci = a.second;

				if(asci.time > -1 && asci.socket != -1)
				{
					// receive packet
					EncryptedPacket *packet = NULL;
					if(static_cast<IntegratedServerComponent*>(dbs)->run(asci.socket, packet))
					{
						so->packetReceivedFrom(asci.realID);
						packet_tuple.enqueue(std::make_tuple(asci.socket, a.first, packet));
						asci.time = 1000;
						continue;
					}

					// if time out, check connection logs
					if(!static_cast<IntegratedServerComponent*>(dbs)->run(&(asci.time)))
					{
						so->householdDisconnected(asci.realID);
						
						std::string str_id = a.first;
						unsigned char id[] =
						{
							static_cast<unsigned char>(str_id[0]),
							static_cast<unsigned char>(str_id[1]),
							static_cast<unsigned char>(str_id[2]),
							static_cast<unsigned char>(str_id[3]),
							0
						};

						// create a disconnection request
						{
						std::string disc_req = PacketCreator::formReadPacket("4p?2", id, 6, vm);
						EncryptedPacket *d_packet =
							new EncryptedPacket(reinterpret_cast<unsigned char*>(const_cast<char*>(disc_req.c_str())));
						packet_tuple.enqueue(std::make_tuple(0, a.first, d_packet));
						}

						// create a request to reset the app server
						// id of disconnected wcs
						{
						std::string disc_req = PacketCreator::formReadPacket("4p?2", id, 7, vm);
						EncryptedPacket *d_packet =
							new EncryptedPacket(reinterpret_cast<unsigned char*>(const_cast<char*>(disc_req.c_str())));
						packet_tuple.enqueue(std::make_tuple(0, a.first, d_packet));
						}
						
						shutdown(asci.socket, SHUT_RDWR);
						close(asci.socket);
						
						so->connectionErased(asci.realID);
						(connection_logs->getMap())->erase(a.first);

						continue;
					}
				}
			}
		}, 1
	);

	// thread for db server
	t->addTask("server_thread",
		[&]()->void
		{
			const std::tuple<int, std::string, EncryptedPacket*> def =
				std::make_tuple<int, std::string, EncryptedPacket*>(-1, "NULL", 0);
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			Promise<std::tuple<int, std::string, EncryptedPacket*> > p = packet_tuple.dequeue();
			std::tuple<int, std::string, EncryptedPacket*> ep = p.result(1000, def, def);
			int socket = std::get<0>(ep);

			if(socket != -1)
			{
				std::string appServerID = std::get<1>(ep);
				EncryptedPacket *enc_pack = std::get<2>(ep);
				DBQuery res;

				so->packetProcessing();
				if(!static_cast<IntegratedServerComponent*>(dbs)->run(enc_pack, &appServerID, &res))
				{
					//delete enc_pack;
					so->failedPacketProcessing();
					return;
				}

				//delete enc_pack;
				so->successfulPacketProcessing();
				query_pairs.enqueue(std::make_pair(socket, res));
			}			
		}, 1
	);

	// thread for db server executor
	t->addTask("exec_thread",
		[&]()->void
		{
			const std::pair<int, DBQuery> def = std::make_pair(-1, DBQuery());

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			Promise<std::pair<int, DBQuery> > p = query_pairs.dequeue();
			std::pair<int, DBQuery> pr = p.result(1000, def, def);
			
			if(pr.first != -1)
			{
				ResponseAttribute ra;

				if(!static_cast<IntegratedServerComponent*>(dbs)->run(&pr.second, &ra))
				{
					query_pairs.enqueue(pr);
					return;
				}

				ra.setSocket(pr.first);
				responses.enqueue(ra);
			}
		}, 1
	);

	// responder thread
	t->addTask("resp_thread",
		[&]()->void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			ResponseAttribute ra = responses.dequeue(ResponseAttribute());

			/*
			if(ra.getValueBuffer() != -1)
			{
				std::string packet;
				
				if(ra.getResult())
				{
					// create a nak packet
					packet = PacketCreator::formReadPacket("4p?2", ra.getEncryptedKey(), 10, vm);
					
					// send to designated app server
					s->sendData(ra.getSocket(),
						reinterpret_cast<unsigned char*>(
									const_cast<char*>(packet.c_str())), 11);
				}
				else
				{
					// to be change to conform to the dynamic packet requirement
					switch(ra.getRequestType())
					{
						case 6: case 7: break;
						case 1: case 2: case 3: case 4: // read packets
						{
							packet = PacketCreator::formReadPacket("4p?2", ra.getEncryptedKey(), ra.getRequestType(), vm);

							// send to designated app server
							s->sendData(ra.getSocket(),
								reinterpret_cast<unsigned char*>(
									const_cast<char*>(packet.c_str())), 11);
							break;
						}
						default: // write packets
						{
							std::string val = std::to_string(ra.getValueBuffer());
							unsigned char data[] =
							{
								static_cast<unsigned char>(val[0]),
								static_cast<unsigned char>(val[1]),
								static_cast<unsigned char>(val[2]),
								static_cast<unsigned char>(val[3]),
								0
							};
							
							packet = PacketCreator::formWritePacket("4p?2", ra.getEncryptedKey(), data, ra.getRequestType(), vm);
							
							// send to designated app server
							s->sendData(ra.getSocket(),
								reinterpret_cast<unsigned char*>(
									const_cast<char*>(packet.c_str())), 15);
							break;
						}
					}
				}
			}
			*/
			
		}, 1
	);
}