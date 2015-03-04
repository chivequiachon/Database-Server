#include "status_observer.h"

#include <string>
#include <ctime>

std::string StatusCreator::createConnectionStatus(std::string notifyStr)
{
	// current date/time based on current system
	time_t now = time(0);
				   
	// convert now to string form
	std::string dt = std::string(ctime(&now));
	std::string stat = "[" + dt.substr(0, dt.length()-1) + "] " + notifyStr;

	return stat;
}

std::string StatusCreator::createConsoleStatus(std::string notifyStr)
{
	return notifyStr;
}

StatusObserver::StatusObserver(ActiveQueue *connection_status_records, ActiveQueue *console_records) :
	connection_status_records(connection_status_records),
	console_records(console_records)
{
}

void StatusObserver::socketDetect()
{
	std::string stat = StatusCreator::createConnectionStatus("A socket has been detected");
	connection_status_records->enqueue(stat);
}

void StatusObserver::socketDestructionDetect()
{
	std::string stat = StatusCreator::createConnectionStatus("Unknown socket destroyed");
	connection_status_records->enqueue(stat);
}

void StatusObserver::socketExist(std::string id)
{
	std::string stat = StatusCreator::createConnectionStatus("App Server id [" + id + "] already exist");
	connection_status_records->enqueue(stat);
}

void StatusObserver::serverStarted()
{
	std::string stat = StatusCreator::createConnectionStatus("Database server activated");
	connection_status_records->enqueue(stat);
}

void StatusObserver::socketConnectionSuccess()
{
	std::string stat = StatusCreator::createConnectionStatus("Connection success");
	connection_status_records->enqueue(stat);
}

void StatusObserver::householdID_Identified(std::string id)
{
	std::string stat = StatusCreator::createConnectionStatus(" App Server id [" + id + "]");
	connection_status_records->enqueue(stat);
}

void StatusObserver::householdDisconnected(std::string id)
{
	std::string stat = StatusCreator::createConnectionStatus(" App Server id [" + id + "] was disconnected");
	connection_status_records->enqueue(stat);
}

void StatusObserver::packetReceivedFrom(std::string id)
{
	std::string stat = StatusCreator::createConnectionStatus("A packet was received from App Server id [" + id + "]");
	connection_status_records->enqueue(stat);
}

void StatusObserver::connectionErased(std::string id)
{
	std::string stat = StatusCreator::createConnectionStatus("Connection by App Server id [" + id + "] was erased");
	connection_status_records->enqueue(stat);
}

void StatusObserver::packetProcessing()
{
	std::string stat = StatusCreator::createConnectionStatus("Processing packet...");
	connection_status_records->enqueue(stat);
}

void StatusObserver::validating()
{
	std::string stat = StatusCreator::createConsoleStatus("Validating packet...");
	console_records->enqueue(stat);
}

void StatusObserver::failedValidation()
{
	std::string stat = StatusCreator::createConsoleStatus("Packet validation failed.");
	console_records->enqueue(stat);
}

void StatusObserver::successfulValidation()
{
	std::string stat = StatusCreator::createConsoleStatus("Packet validation successful.");
	console_records->enqueue(stat);
}

void StatusObserver::extracting()
{
	std::string stat = StatusCreator::createConsoleStatus("Extracting data from packet...");
	console_records->enqueue(stat);
}

void StatusObserver::failedExtraction()
{
	std::string stat = StatusCreator::createConsoleStatus("Data extraction failed.");
	console_records->enqueue(stat);
}

void StatusObserver::successfulExtraction()
{
	std::string stat = StatusCreator::createConsoleStatus("Data extraction successful.");
	console_records->enqueue(stat);
}

void StatusObserver::requesting()
{
	std::string stat = StatusCreator::createConsoleStatus("Requesting for database access...");
	console_records->enqueue(stat);
}

void StatusObserver::failedRequest()
{
	std::string stat = StatusCreator::createConsoleStatus("Database access request failed.");
	console_records->enqueue(stat);
}

void StatusObserver::successfulRequest()
{
	std::string stat = StatusCreator::createConsoleStatus("Database access request successful.");
	console_records->enqueue(stat);
}

void StatusObserver::executing()
{
	std::string stat = StatusCreator::createConsoleStatus("Accessing database.");
	console_records->enqueue(stat);
}

void StatusObserver::failedExecution()
{
	std::string stat = StatusCreator::createConsoleStatus("Database access failed.");
	console_records->enqueue(stat);
}

void StatusObserver::successfulExecution()
{
	std::string stat = StatusCreator::createConsoleStatus("Database access successful.");
	console_records->enqueue(stat);
}

void StatusObserver::failedPacketProcessing()
{
	std::string stat = StatusCreator::createConnectionStatus("Packet processing failed.");
	connection_status_records->enqueue(stat);	
}

void StatusObserver::successfulPacketProcessing()
{
	std::string stat = StatusCreator::createConnectionStatus("Packet processing successful.");
	connection_status_records->enqueue(stat);	
}

