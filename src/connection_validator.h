#ifndef CONVAL_H
#define CONVAL_H

#include "systemcomponent.h"
#include "Server.h"
#include "checksum.h"
#include "proxy.h"
#include "app_server_info.h"
#include "status_observer.h"
#include "DBDriver.h"

#include <map>
#include <utility>
#include <string>

class ConnectionValidator : public SystemComponent
{
	typedef MapServantProxy<MapServant<std::string, AppServerConnectionInfo>, std::string, AppServerConnectionInfo> ActiveMap;

	IServer *s;
	ChecksumModule *vm;
	ActiveMap *connection_logs;
	StatusObserver *so;
	Database *db;

public:
	inline explicit ConnectionValidator(IServer *s, ChecksumModule *vm,
										ActiveMap *connection_logs,
										StatusObserver *so, Database *db) :
		s(s), vm(vm), connection_logs(connection_logs), so(so), db(db) {}
	
	virtual ~ConnectionValidator() {}

	virtual bool run();
	virtual bool run(int socket);
};


#endif