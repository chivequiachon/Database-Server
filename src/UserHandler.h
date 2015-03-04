#ifndef __UH_H
#define __UH_H

#include "threadpool.h"
#include "systemcomponent.h"
#include "Server.h"
#include "DBDriver.h"
#include "checksum.h"
#include "encrypted_packet.h"
#include "dbquery.h"
#include "servant.h"
#include "proxy.h"
#include "app_server_info.h"
#include "status_observer.h"
#include "concurrent_queue.h"
#include "response_attrib.h"

#include <map>
#include <utility>
#include <string>
#include <tuple>

class DatabaseServer
{
private:
	typedef MapServantProxy<MapServant<std::string, AppServerConnectionInfo>, std::string, AppServerConnectionInfo> ActiveMap;
	typedef ServantProxy<Servant<std::string>, std::string> ActiveQueue;

	ActiveMap *connection_logs;
	ServantProxy<Servant<int>, int> doubtful_sockets;
	ServantProxy<Servant<std::tuple<int, std::string, EncryptedPacket*> >, std::tuple<int, std::string, EncryptedPacket*> > packet_tuple;
	ServantProxy<Servant<std::pair<int, DBQuery> >, std::pair<int, DBQuery> > query_pairs;
	ConcurrentQueue<ResponseAttribute> responses;

	ThreadPool *t;

	IServer *s;
	Database *db;
	ChecksumModule *vm;
	
	StatusObserver *so;

	SystemComponent *dbs; // database server

	std::string host, port;

public:
	explicit DatabaseServer(ActiveQueue *c, ActiveMap *connection_logs, ActiveQueue *console,
							ThreadPool *t, std::string host, std::string port);
	~DatabaseServer();
	void run();
};
#endif