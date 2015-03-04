#include "model_test.h"
#include "view_test.h"
#include "proxy.h"
#include "threadpool.h"

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

TesterModel::TesterModel() :
	s(NULL), t(NULL), console_records(NULL),
	connection_status_records(NULL),
	server_connection_logs(NULL), view(NULL)
{
}

TesterModel::~TesterModel()
{
	if(connection_status_records != NULL)
		delete connection_status_records;

	if(server_connection_logs != NULL)
		delete server_connection_logs;

	if(console_records != NULL)
		delete console_records;

	if(t != NULL) delete t;
	if(s != NULL) delete s;
}

void TesterModel::aboutInfo()
{
	std::string about_info = "(c)\nDeveloper: Mark Alfonso Quiachon";

	view->updateAboutDisplay(about_info);
}

void TesterModel::startServer(std::string host, std::string port)
{
	view->appendToConsole("Activating server...");

	// create thread pool
	t = new ThreadPool();
	if(t == NULL)
	{
		view->appendToConsole("Thread pool was not created successfully.");
		return;
	}

	console_records = new ActiveQueue(t, "cr_sched_thread");
	t->addTask("console_thread",
		[&]()->void
		{
			Promise<std::string> p_str = console_records->dequeue();
			std::string str = p_str.result(1000, std::string("NULL"), std::string("NULL"));
			
			if(str != "NULL")
				view->appendToConsole(str);

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}, 1
	);

	connection_status_records = new ActiveQueue(t, "csr_sched_thread");
	if(connection_status_records == NULL)
	{
		view->appendToConsole("Connection Status Records was not created successfully.");
		return;
	}

	server_connection_logs = new ActiveMap(t, "scl_sched_thread");
	if(server_connection_logs == NULL)
	{
		view->appendToConsole("Server Connection Logs was not created successfully.");
		return;
	}

	s = new DatabaseServer(connection_status_records,
						   server_connection_logs,
						   console_records,
						   t, host, port);
	if(s == NULL)
	{
		view->appendToConsole("Database Server was not activated successfully.");
		return;
	}

	console_records->enqueue("Initializing database server.");
	console_records->enqueue("Listening to port " + port + " and host " + host);
	console_records->enqueue("Thread pool was created successfully.");
	console_records->enqueue("Connection Status Records was created successfully.");
	console_records->enqueue("Server Connection Logs was created successfully.");
	console_records->enqueue("Database Server was activated successfully.");
	
	view->hostFieldSetEditable(false);
	view->portFieldSetEditable(false);

	s->run();
	
	t->addTask("csr_thread",
		[&]()->void
		{
			Promise<std::string> p_str = connection_status_records->dequeue();
			std::string str = p_str.result(1000, std::string("NULL"),
												 std::string("NULL"));
			if(str != "NULL")
				view->appendToConnectionStatus(str);

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}, 1
	);

	t->addTask("logs_thread",
		[&]()->void
		{
			for(auto& a : *(server_connection_logs->getMap()))
			{
				AppServerConnectionInfo &asci = a.second;

				if(asci.time <= 0)
					view->deleteRowFromServerList(asci.realID);
				else
					view->updateRowFromServerList(asci.realID, asci.time);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}, 1
	);
}

void TesterModel::stopServer()
{
	if(t) delete t;
	if(s) delete s;
	if(connection_status_records)
		delete connection_status_records;

	if(console_records) delete console_records;
	if(server_connection_logs)
		delete server_connection_logs;

	s = NULL;
	t = NULL;
	connection_status_records = NULL;
	console_records = NULL;
	server_connection_logs = NULL;
	
	view->hostFieldSetEditable(true);
	view->portFieldSetEditable(true);

	view->appendToConsole("Database Server was stopped successfully");
}

void TesterModel::registerObserver(TesterView *o)
{
	if(view == NULL) view = o;	
}

void TesterModel::notifyObservers()
{
	//for(auto& o : vec_observers)
		//o->updateOutput();
}