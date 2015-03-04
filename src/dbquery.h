#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>

class DBQuery
{
	std::string dbquery;
	std::string checkLockQuery;
	std::string lockQuery;
	std::string unlockQuery;
	std::string encryptedKey;
	int operation;

public:
	explicit DBQuery();
	explicit DBQuery(std::string query, std::string lockCheckQuery,
					 std::string lockQuery, std::string unlockQuery,
					 std::string encryptedKey, int operation);
	inline ~DBQuery() {}

	void setQuery(std::string query);
	std::string getQuery() const;

	void setCheckLockQuery(std::string checkLockQuery);
	std::string getCheckLockQuery() const;

	void setLockQuery(std::string lockQuery);
	std::string getLockQuery() const;

	void setUnlockQuery(std::string unlockQuery);
	std::string getUnlockQuery() const;

	void setEncryptedKey(std::string encryptedkey);
	std::string getEncryptedKey() const;

	void setOperation(int operation);
	int getOperation() const;

	DBQuery& operator=(const DBQuery &dq);
};

#endif