#ifndef __CONTROLLER_TEST_H
#define __CONTROLLER_TEST_H

#include <string>

class TesterModel;
class TesterView;

class TesterController
{
private:
	TesterModel *m;
	TesterView *v;

public:
	explicit TesterController(TesterModel *m);
	~TesterController();
	
	void showAbout();
	void startServer(std::string host, std::string port);
};

#endif