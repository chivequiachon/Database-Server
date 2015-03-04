#ifndef ASINFO_H
#define ASINFO_H

struct AppServerConnectionInfo
{
	std::string realID;
	int socket;
	int time;

	bool operator==(const std::string &key)
	{
		return key == realID;
	}
};

#endif