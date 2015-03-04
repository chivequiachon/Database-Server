#ifndef MYSQL_H
#define MYSQL_H

#include "DBDriver.h"

#include <cppconn/resultset.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class MYSQL_Database : public Database
{
public:
	typedef sql::ResultSet ResultSet;

private:
	sql::Driver *driver;
	sql::Connection *con;
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	std::string db_url, db_pass, db_user;
	std::function<bool(ResultSet*&)> callback;

public:
	inline explicit MYSQL_Database(const std::string db_name) :
		Database(db_name), driver(0), con(0), res(0), pstmt(0)
	{
	}

	virtual ~MYSQL_Database();

	void setCallback(std::function<bool(ResultSet*&)> callback);
	void setMysqlDBInfo(std::string url, std::string pass, std::string user);
	void setSchema(std::string schema);
	
	virtual bool connectToDB();
	virtual bool closeDB();
	virtual bool executeStatement(const std::string &command);
};

#endif