#include "executor.h"
#include "mysql.h"
#include "packet_creator.h"

#include <string>
#include <cstring>
#include <iostream>

int Executor::checkDataLock(DBQuery *query)
{
	// identify lock-checking query
	// NOTE: only the table "Status_Household" needs lock checking, any operations
	// with the use of "Status_Household" needs to check the lock every time to 
	// avoid data race.
	std::string lockCheckQuery = query->getCheckLockQuery();

	int lock_status;
	dynamic_cast<MYSQL_Database*>(db)->setCallback(
	    [&](MYSQL_Database::ResultSet*& res)->bool
	    {
	    	while(res->next())
	    		lock_status = res->getInt("Lock");

			return true;
	    }
    );

	dynamic_cast<MYSQL_Database*>(db)->executeStatement(lockCheckQuery);

	return lock_status;
}

void Executor::lockData(DBQuery *query)
{
	std::string updateLockQuery = query->getLockQuery();
	std::cout << updateLockQuery << std::endl;

	dynamic_cast<MYSQL_Database*>(db)->setCallback(
		[](MYSQL_Database::ResultSet*& res)->bool { return true; });

	dynamic_cast<MYSQL_Database*>(db)->executeStatement(updateLockQuery);
}

void Executor::unlockData(DBQuery *query)
{
	std::string updateLockQuery = query->getUnlockQuery();
	std::cout << updateLockQuery << std::endl;

	dynamic_cast<MYSQL_Database*>(db)->setCallback(
		[](MYSQL_Database::ResultSet*& res)->bool { return true; });

	dynamic_cast<MYSQL_Database*>(db)->executeStatement(updateLockQuery);
}

bool Executor::run() { return false; }

bool Executor::run(DBQuery *query, ResponseAttribute *response)
{
	if(response == NULL) return false;

	std::string str_query = query->getQuery();

	bool flag = true;
	int buffer = 0;

	if(str_query != "NULL")
	{
		int op = query->getOperation();
		unsigned char *id = reinterpret_cast<unsigned char*>(
				const_cast<char*>(query->getEncryptedKey().c_str()));
		
		response->setEncryptedKey(id);
		response->setRequestType(op);
		
		switch(op)
		{
			case 2: case 3: case 4: case 6: case 7:
				std::cout << str_query << std::endl;
				static_cast<MYSQL_Database*>(db)->setCallback(
					[&op, &buffer](MYSQL_Database::ResultSet*& res)->bool
					{
						return true;
					});
				flag = static_cast<MYSQL_Database*>(db)->executeStatement(str_query);
				break;

			case 5: case 8: case 9: case 10:
			{
				// if data is currently locked
				if(checkDataLock(query) == 1) // client-side application is currently using the data
				{
					std::cout << "Client-side application is currently using the data" << std::endl;
					return false;
				}
			}

			case 1:
			{
				lockData(query);
				std::cout << str_query << std::endl;
				static_cast<MYSQL_Database*>(db)->setCallback(
					[&op, &buffer](MYSQL_Database::ResultSet*& res)->bool
					{
						if(op == 5)
						{
	    					while(res->next())
	    						buffer = res->getInt("Balance");
						}
						else if(op == 8)
						{
							while(res->next())
	    						buffer = res->getInt("Shutdown");	
						}
						else if(op == 9)
						{
							while(res->next())
	    						buffer = res->getInt("Store");
						}

						return true;
					});
				flag = static_cast<MYSQL_Database*>(db)->executeStatement(str_query);
				unlockData(query);
				break;
			}
			
			default:
				flag = false;
				break;
		}
		
		response->setResult(!flag);
		response->setValueBuffer(buffer);
	}

	return true;
}