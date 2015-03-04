#ifndef __MODEL_TEST_H
#define __MODEL_TEST_H

#include <string>
#include <vector>

#include "servant.h"
#include "proxy.h"
#include "UserHandler.h"
#include "threadpool.h"
#include "app_server_info.h"

class TesterView;

class TesterModel
{
private:
	typedef ServantProxy<Servant<std::string>, std::string> ActiveQueue;
	typedef MapServantProxy<MapServant<std::string, AppServerConnectionInfo>, std::string, AppServerConnectionInfo> ActiveMap;

	DatabaseServer *s;
	ThreadPool *t;
	
	ActiveQueue *console_records;
	ActiveQueue *connection_status_records;
	ActiveMap *server_connection_logs;

	// Observer
	TesterView *view;

public:
	explicit TesterModel();
	~TesterModel();

	void aboutInfo();
	void startServer(std::string host, std::string port);
	void stopServer();

	void registerObserver(TesterView *o);
	void notifyObservers();
};

#endif