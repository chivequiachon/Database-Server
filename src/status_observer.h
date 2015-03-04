#ifndef __STATOBS_H
#define __STATOBS_H

#include <ctime>
#include <string>

#include "proxy.h"
#include "servant.h"

class StatusCreator
{
public:
	static std::string createConnectionStatus(std::string notifyStr);
	static std::string createConsoleStatus(std::string notifyStr);
};

class StatusObserver
{
	typedef ServantProxy<Servant<std::string>, std::string> ActiveQueue;

	ActiveQueue *connection_status_records;
	ActiveQueue *console_records;

public:
	explicit StatusObserver(ActiveQueue *connection_status_records,
							ActiveQueue *console_records);
	inline ~StatusObserver() {}

	void socketDetect();
	void socketDestructionDetect();
	void socketExist(std::string id);
	void serverStarted();
	void socketConnectionSuccess();
	void householdID_Identified(std::string id);
	void householdDisconnected(std::string id);
	void packetReceivedFrom(std::string id);
	void connectionErased(std::string id);
	void packetProcessing();
	void validating();
	void failedValidation();
	void successfulValidation();
	void extracting();
	void failedExtraction();
	void successfulExtraction();
	void requesting();
	void failedRequest();
	void successfulRequest();
	void executing();
	void failedExecution();
	void successfulExecution();
	void failedPacketProcessing();
	void successfulPacketProcessing();
};

#endif