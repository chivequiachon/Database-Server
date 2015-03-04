#include "connection_validator.h"
#include "Server.h"
#include "packet_creator.h"
#include "checksum.h"
#include "extractor.h"
#include "big_int_lib.h"
#include "app_server_info.h"
#include "status_observer.h"
#include "future.h"
#include "proxy.h"
#include "mysql.h"

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <cstring>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <tuple>

bool ConnectionValidator::run() { return false; }
bool ConnectionValidator::run(int socket)
{
	// manage first time connection
	unsigned char buffer[22] = "";

	if(socket > 0)
	{
		if(s->receiveData(socket, buffer, 22) > 0)
		{
			// first time connection of the clients of the base station
			if(buffer[0] == '4' && buffer[1] == 'p' && buffer[2] == '?' && buffer[3] == '2')
			{
				// validate data using fletcher's checksum algorithm
				if(buffer[9] == vm->calculateChecksum(buffer, 9))
				{
					if(Extractor::extractRequestType(buffer) == 1)
					{
						// get the encrypted ID and perform calculations to
						// extract household ID from it
						std::string encryptedID = Extractor::extractID(buffer);
						std::string householdID = Extractor::extractRealID(encryptedID);
						
						so->householdID_Identified(householdID);

						// check if socket is already connected using
						// the extracted id
						AppServerConnectionInfo currentVal = //connection_logs->getValue(encryptedID, {"NULL", -1, -1});
							GetWithDef(*(connection_logs->getMap()), encryptedID, {"NULL", -1, -1});
						if(currentVal.socket == -1)
						{
	    					connection_logs->setElement(encryptedID, {householdID, socket, 1000});
	    					so->socketConnectionSuccess();

	    					// retrieve contents of status_household table from db
	    					std::string query = "SELECT idUser, idEncrypt, Balance FROM status_household WHERE `idAppServer`=\'" + householdID + "\'";
	    					//std::vector<std::pair<std::string, std::string> > resultList;
	    					std::vector<std::tuple<std::string, std::string, int> > resultList;
	    					static_cast<MYSQL_Database*>(db)->setCallback(
								[&resultList](MYSQL_Database::ResultSet*& res)->bool
								{
									while(res->next())
									{
										std::string idEncrypt = res->getString("idEncrypt");
										std::string idUser = res->getString("idUser");
										int balance = res->getInt("Balance");

										resultList.push_back(std::make_tuple(idEncrypt, idUser, balance));
									}

									return true;
								});

							static_cast<MYSQL_Database*>(db)->executeStatement(query);

							// send contents to app server
							while(resultList.size() > 0)
							{
								// pop from list
								std::tuple<std::string, std::string, int> p = resultList.back();
								resultList.pop_back();

								// create packet
								unsigned char buffer2[18] = "";
								buffer2[0] = static_cast<unsigned char>(std::get<0>(p)[0]);
								buffer2[1] = static_cast<unsigned char>(std::get<0>(p)[1]);
								buffer2[2] = static_cast<unsigned char>(std::get<0>(p)[2]);
								buffer2[3] = static_cast<unsigned char>(std::get<0>(p)[3]);

								// concatenate contents
								strcat((char*)buffer2, const_cast<char*>(std::get<1>(p).c_str()));
								strcat((char*)buffer2, "-");
								strcat((char*)buffer2, const_cast<char*>(std::to_string(std::get<2>(p)).c_str()));
								
								// end packet
								buffer2[17] = '\0';

								// send data
								s->sendData(socket, buffer2, 18);

								// sleep
								std::this_thread::sleep_for(std::chrono::milliseconds(200));
							}

							unsigned char buffer2[2] = {'^', '\0'};
							s->sendData(socket, buffer2, 2);

							return true;
						}

						so->socketExist(householdID);
					}
				}
			}
			else
			{
				/// it might be an activity from the web server that needs real-time response

				// extract app server id from packet
				std::string packet = std::string(reinterpret_cast<char*>(buffer));
				std::string appServerID = packet.substr(1, 8);

				// erase occurrence of '-' char
				appServerID.erase(std::remove(appServerID.begin(), appServerID.end(), '-'), appServerID.end());

				std::cout << packet << std::endl;
				std::cout << appServerID << std::endl;

				// search socket using app server real id
				Promise<AppServerConnectionInfo> p_aci =
					connection_logs->getValueEnhanced<std::string>(appServerID, {"NULL", 0, 0});

				AppServerConnectionInfo aci = p_aci.result(1000, {"NULL", 0, 0}, {"NULL", 0, 0});
				
				int realTimeSocket;
				if(aci.socket != 0)
				{
					realTimeSocket = aci.socket;

					// send packet to the correct app server
					s->sendData(realTimeSocket, buffer, 22);
				}
			}
		}

		// header is invalid,
		// socket will be automatically destroyed
		so->socketDestructionDetect();

		shutdown(socket, SHUT_RDWR);
		close(socket);
	}		

	return false;
}