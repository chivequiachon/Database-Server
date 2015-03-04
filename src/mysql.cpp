#include "mysql.h"

// mysql headers
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

MYSQL_Database::~MYSQL_Database()
{
	// if they point to something, delete
	if(con) delete con;
	if(res) delete res;
	if(pstmt) delete pstmt;
}

void MYSQL_Database::setCallback(std::function<bool(ResultSet*&)> callback)
{
	this->callback = callback;
}

void MYSQL_Database::setMysqlDBInfo(std::string url, std::string pass, std::string user)
{
	this->db_url = url;
	this->db_pass = pass;
	this->db_user = user;
}

void MYSQL_Database::setSchema(std::string schema)
{
	con->setSchema(schema);
}

bool MYSQL_Database::connectToDB()
{
	try
	{
		driver = get_driver_instance();
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		con = driver->connect(db_url, db_user, db_pass);
		return true;
	}
	catch(sql::SQLException &e)
	{
		return false;
	}
}

bool MYSQL_Database::closeDB()
{
	try
	{
		if(con != NULL)
		{
			this->con->close();
			delete this->con;
			this->driver->threadEnd();
		}

		return true;
	}
	catch(sql::SQLException &e)
	{
		return false;
	}
}

bool MYSQL_Database::executeStatement(const std::string &command)
{
	try
	{
		// set and execute query
		pstmt = con->prepareStatement(command);
		res = pstmt->executeQuery();

		// execute callback
		return this->callback(res);
	}
	catch(sql::SQLException &e)
	{
		std::cout << "ERROR: SQL Exception in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "ERROR: " << e.what();
		std::cout << " MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}