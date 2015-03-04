#include "requestor.h"
#include "extracted_data.h"
#include "dbquery.h"
#include "extractor.h"

#include <string>

bool Requestor::run() { return false; }
bool Requestor::run(ExtractedData *in, std::string *appServerID, DBQuery *out)
{
	std::string householdID, encryptedID, realAppServerID, data, query;
	int requestType = 0;

	// decrypt encrypted ID using big integer calculations
	encryptedID = in->getEncryptedID();

	if(encryptedID != "NULL")
	{
		realAppServerID = Extractor::extractRealID(*appServerID);
		householdID = Extractor::extractRealID(encryptedID);
		
		// get request type
		requestType = in->getRequestType();

		// get all data from the extracted message
		data = in->getData();

		// check equivalent query from the request type
		switch(requestType)
		{
			case 1: // insert water usage
				query = "INSERT INTO waterusage_household (idUser, Volume, DateTime) VALUES (\'" + householdID + "\', \'" + data + "\', NOW())";
				break;
			case 2: // update water balance
				query = "UPDATE status_household SET Balance=" + data + " WHERE `idUser`=\'" + householdID + "\'";
				break;
			case 3: // update connection status (for first time connection and disconnection)
				query = "UPDATE status_household SET Status=1, idAppServer=" + realAppServerID + " WHERE `idUser`=\'" + householdID + "\'";
				break;
			case 4:
				query = "UPDATE status_household SET Status=0, idAppServer=" + realAppServerID + " WHERE `idUser`=\'" + householdID + "\'";
				break;
			case 5: // retrieve water balance
				query = "SELECT Balance FROM status_household WHERE idUser=\'" + householdID + "\'";
				break;
			case 6: // disconnect all wcs from app server
				query = "UPDATE status_household SET Status=0 WHERE `idAppServer`=\'" + realAppServerID + "\'";
				break;
			case 7: // reset app server id of each disconnected wcs
				query = "UPDATE status_household SET idAppServer=0 WHERE `Status`=0";
				break;
			case 8: // retrieve shutdown
				query = "SELECT Shutdown FROM status_household WHERE idUser=\'" + householdID + "\'";
				break;
			case 9: // retrieve store
				query = "SELECT Store FROM status_household WHERE idUser=\'" + householdID + "\'";
				break;
			case 10: // update store
				query = "UPDATE status_household SET Store=0 WHERE `idUser`=\'" + householdID + "\'";
				break;
			default:
				return false;
		};

		// create lock checking query
		std::string lockCheckQuery = "SELECT * FROM `status_household` WHERE `idUser`=" + householdID;

		// create lock and unlock query
		std::string unlockQuery = "UPDATE `status_household` SET `Lock`=0 WHERE `idUser`=" + householdID;
		std::string lockQuery = "UPDATE `status_household` SET `Lock`=1 WHERE `idUser`=" + householdID;

		// create database request object
		*out = DBQuery(query, lockCheckQuery, lockQuery, unlockQuery, encryptedID, requestType);

		// db request object creation successful
		return true;
	}

	return false;
}