#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "systemcomponent.h"
#include "DBDriver.h"
#include "checksum.h"
#include "dbquery.h"
#include "encrypted_packet.h"
#include "response_attrib.h"

class Executor : public SystemComponent
{
	Database *db;
	
private:
	int checkDataLock(DBQuery *query);
	void lockData(DBQuery *query);
	void unlockData(DBQuery *query);

public:	
	inline explicit Executor(Database *db) : db(db)
	{
	}

	virtual ~Executor()
	{
	}

	virtual bool run();
	virtual bool run(DBQuery *query, ResponseAttribute *response);
};

#endif