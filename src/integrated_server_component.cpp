#include "integrated_server_component.h"
#include "mysql.h"
#include "connection_validator.h"
#include "breakdown_detector.h"
#include "receiver.h"
#include "validator.h"
#include "extractor.h"
#include "requestor.h"
#include "executor.h"
#include "extracted_data.h"


#include <iostream>

IntegratedServerComponent::IntegratedServerComponent(Database *db, IServer *s,
	ChecksumModule *vm, ActiveMap *connection_logs, StatusObserver *so) :
		so(so), cv(new ConnectionValidator(s, vm, connection_logs, so, db)),
		bd(new BreakdownDetector(s)), r(new Receiver(s)),
		v(new Validator(vm)), e(new Extractor()), rq(new Requestor()),
		ex(new Executor(db))
{
}

bool IntegratedServerComponent::run() { return false; }
bool IntegratedServerComponent::run(int *time)
{
	return static_cast<BreakdownDetector*>(bd)->run(time);
}

bool IntegratedServerComponent::run(int socket)
{
	return static_cast<ConnectionValidator*>(cv)->run(socket);
}

bool IntegratedServerComponent::run(int socket, EncryptedPacket *&out)
{
	return static_cast<Receiver*>(r)->run(socket, out);
}

bool IntegratedServerComponent::run(DBQuery *query, ResponseAttribute *ra)
{
	so->executing();
	if(static_cast<Executor*>(ex)->run(query, ra))
	{
		so->successfulExecution();
		return true;
	}

	so->failedExecution();
	return false;
}

bool IntegratedServerComponent::run(EncryptedPacket *in, std::string *appServerID, DBQuery *out)
{
	ExtractedData ed;
	so->validating();
	if(!static_cast<Validator*>(v)->run(in))
	{
		so->failedValidation();
		return false;
	}
	so->successfulValidation();

	so->extracting();
	if(!static_cast<Extractor*>(e)->run(in, &ed))
	{
		so->failedExtraction();
		return false;
	}
	so->successfulExtraction();

	DBQuery query;
	so->requesting();
	if(!static_cast<Requestor*>(rq)->run(&ed, appServerID, &query))
	{
		so->failedRequest();
		return false;
	}
	so->successfulRequest();

	*out = query;

	return true;
}