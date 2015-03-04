#ifndef _DBDRIVER_H
#define _DBDRIVER_H

#include <string>

// sqlite3 headers
//#include "sqlite3.h"

class Database
{
private:
	std::string name;

private:
	virtual bool connectToDB() = 0;
	virtual bool closeDB() = 0;
	virtual bool executeStatement(const std::string &command) = 0;

	bool shortener(std::string &command);

public:
	inline explicit Database(const std::string db_name) : name(db_name) {}

	virtual ~Database() {}
	virtual void setDBName(const std::string db_name);
	virtual bool createTable(const std::string &tableName, const std::string &elements);
	virtual bool insertEntry(const std::string &tableName, const std::string &elements, 
							 const std::string &values);
	virtual bool selectEntry(const std::string &tableName, const std::string &whereClause);
	virtual bool updateEntry(const std::string &tableName, const std::string &setClause,
							 const std::string &whereClause);
	virtual bool deleteEntry(const std::string &tableName, const std::string &whereClause);
};


/*
class SQLITE3_Database : public Database
{
private:
	
	// for manipulating function pointers, function objects, and lambdas
	template <typename Function>
	struct function_traits : public function_traits<decltype(&Function::operator())>
	{};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const>
	{
	  typedef ReturnType (*pointer)(Args...);
	  typedef std::function<ReturnType(Args...)> function;
	};

	template <typename Function>
	typename function_traits<Function>::pointer to_function_pointer (Function& lambda)
	{
	  return static_cast<typename function_traits<Function>::pointer>(lambda);
	}

	template <typename Function>
	typename function_traits<Function>::function to_function (Function& lambda)
	{
	  return static_cast<typename function_traits<Function>::function>(lambda);
	}
	
	struct DataEncapsulator
	{
		std::function<int(void*, int, char**, char**)> f;
		void *data;
	};
	
	static int callback2(void* data, int size, char **a, char **b)
	{
		DataEncapsulator de = *static_cast<DataEncapsulator*>(data);
		void *encap_data = de.data;
		
		return de.f(encap_data, size, a, b);
	}

private:
	sqlite3 *db;
	int(*callback)(void*, int, char**, char**);
	//std::function<int(void*, int, char**, char**)> f;
	void *data;

public:
	SQLITE3_Database(const std::string db_name) : Database(db_name), db(NULL), data(NULL)  {}
	virtual ~SQLITE3_Database() {}

	template <typename Function>
	void setCallback(Function lambda)//std::function<int(void*, int, char**, char**)> f)
	{
		auto function = new (decltype(to_function_pointer(lambda)))(to_function_pointer(lambda));
		callback = static_cast<int(*)(void*, int, char**, char**)>(*function);
		//this->f = f;
	}

	void setData(void *data) { this->data = data; }
	virtual bool connectToDB()
	{
		return sqlite3_open(this->name.c_str(), &db) ? false : true;
	}

	virtual bool closeDB()
	{
		if(db == NULL) return false; // db was never opened or is already close
		else
		{
			sqlite3_close(db); // close db
			db = NULL;
			return true;
		}
	}

	virtual bool executeStatement(const std::string &command)
	{
		// encapsulate data
		//DataEncapsulator de = {this->f, this->data};
		
		char *zErrMsg = NULL;
		int rc = sqlite3_exec(this->db, command.c_str(), callback, data, &zErrMsg);
		if(rc != SQLITE_OK)
		{
			sqlite3_free(zErrMsg);
			closeDB();
			return false;
		}

		return true;
	}
};*/


#endif
