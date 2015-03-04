#ifndef ISC_H
#define ISC_H

#include "systemcomponent.h"
#include "Server.h"
#include "checksum.h"
#include "DBDriver.h"
#include "encrypted_packet.h"
#include "dbquery.h"
#include "proxy.h"
#include "app_server_info.h"
#include "status_observer.h"
#include "response_attrib.h"

#include <map>
#include <utility>
#include <string>

class IntegratedServerComponent : public SystemComponent
{
private:
	typedef MapServantProxy<MapServant<std::string, AppServerConnectionInfo>, std::string, AppServerConnectionInfo> ActiveMap;

	StatusObserver *so;
	SystemComponent *cv; // connection validator
	SystemComponent *bd; // breakdown detector
	SystemComponent *r, *v, *e, *rq, *ex;
	
public:
	explicit IntegratedServerComponent(Database *db, IServer *s,
		ChecksumModule *vm, ActiveMap *connection_logs, StatusObserver *so);
	virtual ~IntegratedServerComponent()
	{
		delete cv;
		delete bd;
		delete r;
		delete v;
		delete e;
		delete rq;
		delete ex;
	}

	virtual bool run();
	virtual bool run(int *time);
	virtual bool run(int socket);
	virtual bool run(int socket, EncryptedPacket *&out);
	virtual bool run(DBQuery *query, ResponseAttribute *ra);
	virtual bool run(EncryptedPacket *in, std::string *appServerID, DBQuery *out);
};

#endif