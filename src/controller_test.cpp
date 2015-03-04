#include "controller_test.h"
#include "model_test.h"
#include "view_test.h"

#include <iostream>
#include <gtkmm/main.h>

TesterController::TesterController(TesterModel *m) :
	m(m), v(NULL)
{
	if(this->v == NULL)
		this->v = new TesterView(m, this);

	Gtk::Main::run(*this->v);
}

TesterController::~TesterController()
{
}

void TesterController::showAbout()
{
	m->aboutInfo();
}

void TesterController::startServer(std::string host, std::string port)
{
	int count = 0;
	for(auto& a : host)
		if(a == '.') count++;

	if(count < 3 || count > 3)
	{
		v->appendToConsole("Server start failed.");
		return;
	}

	if(v->getButtonLabel() == "Start")
	{
		v->updateButtonLabel();
		m->startServer(host, port);
	}
	else
	{
		v->updateButtonLabel();
		m->stopServer();
	}
}