#include "DBDriver.h"

#include <string>

void Database::setDBName(const std::string db_name) { this->name = db_name; }
bool Database::createTable(const std::string &tableName, const std::string &elements)
{
	// execute db command
	std::string command = "CREATE TABLE " + tableName + elements + ";";
	return shortener(command);
}

bool Database::insertEntry(const std::string &tableName, const std::string &elements, 
				 const std::string &values)
{
	// execute db command
	std::string command = "INSERT INTO " + tableName + elements + " VALUES " + values + ";";
	return shortener(command);
}

bool Database::selectEntry(const std::string &tableName, const std::string &whereClause)
{
	// execute db command
	std::string command;
	if(whereClause == "")
		command = "SELECT * from " + tableName + ";";
	else
		command = "SELECT * from " + tableName + " WHERE " + whereClause + ";";
	return shortener(command);
}

bool Database::updateEntry(const std::string &tableName, const std::string &setClause,
				 const std::string &whereClause)
{
	// execute db command
	std::string command = "UPDATE " + tableName + " set " + setClause + " WHERE " + whereClause + ";";
	return shortener(command);
}

bool Database::deleteEntry(const std::string &tableName, const std::string &whereClause)
{
	std::string command = "DELETE from " + tableName + " WHERE " + whereClause + ";";
	return shortener(command);
}

bool Database::shortener(std::string &command)
{
	// connect to db
	if(!connectToDB())
	{
		return false;
	}

	// execute db command
	if(!executeStatement(command))
	{
		return false;
	}

	return closeDB(); // close db;
}