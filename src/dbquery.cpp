#include "dbquery.h"

#include <string>

DBQuery::DBQuery() :
	dbquery("NULL"), lockQuery("NULL"), unlockQuery("NULL"),
	encryptedKey("NULL"), operation(0)
{
}

DBQuery::DBQuery(std::string query, std::string checkLockQuery,
				 std::string lockQuery, std::string unlockQuery,
				 std::string encryptedKey, int operation) :
	dbquery(query), checkLockQuery(checkLockQuery), lockQuery(lockQuery),
	unlockQuery(unlockQuery), encryptedKey(encryptedKey), operation(operation)
{
}

void DBQuery::setQuery(std::string query) { this->dbquery = query; }
std::string DBQuery::getQuery() const { return this->dbquery; }

void DBQuery::setCheckLockQuery(std::string checkLockQuery) { this->checkLockQuery = checkLockQuery; }
std::string DBQuery::getCheckLockQuery() const { return this->checkLockQuery; }

void DBQuery::setEncryptedKey(std::string encryptedKey) { this->encryptedKey = encryptedKey; }
std::string DBQuery::getEncryptedKey() const { return this->encryptedKey; }

void DBQuery::setLockQuery(std::string lockQuery) { this->lockQuery = lockQuery; }
std::string DBQuery::getLockQuery() const { return this->lockQuery; }

void DBQuery::setUnlockQuery(std::string unlockQuery) { this->unlockQuery = unlockQuery; }
std::string DBQuery::getUnlockQuery() const { return this->unlockQuery; }

void DBQuery::setOperation(int operation) { this->operation = operation; }
int DBQuery::getOperation() const { return this->operation; }

DBQuery& DBQuery::operator=(const DBQuery &dq)
{
	this->dbquery = dq.dbquery;
	this->checkLockQuery = dq.checkLockQuery;
	this->encryptedKey = dq.encryptedKey;
	this->lockQuery = dq.lockQuery;
	this->unlockQuery = dq.unlockQuery;
	this->operation = dq.operation;

	return *this;
}