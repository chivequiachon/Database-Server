#include "view_test.h"
#include "controller_test.h"
#include "model_test.h"

#include <iostream>
#include <gtkmm.h>

void TesterView::sig_about_on_click()
{
	c->showAbout();
}

void TesterView::sig_server_start_on_click()
{
	c->startServer(ent_host->get_text(), ent_port->get_text());
}

TesterView::TesterView(TesterModel *m, TesterController *c) :
	m(m), c(c)
{
	maximize();
	set_title("PWS Database Server");

	bx_primary = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	add(*bx_primary);

	menuBar = Gtk::manage(new Gtk::MenuBar());
	bx_primary->pack_start(*menuBar, Gtk::PACK_SHRINK, 0);

	menuItem_help = Gtk::manage(new Gtk::MenuItem("_Help", true));
	menuBar->append(*menuItem_help);
	
	menu_about = Gtk::manage(new Gtk::Menu());
	menuItem_help->set_submenu(*menu_about);
	
	menuItem_about = Gtk::manage(new Gtk::MenuItem("_About", true));
	menuItem_about->signal_activate().connect(sigc::mem_fun(*this, &TesterView::sig_about_on_click));
	menu_about->append(*menuItem_about);

	grid_primary = Gtk::manage(new Gtk::Grid());
	grid_primary->set_border_width(1);
	grid_primary->set_row_spacing(1);
	bx_primary->add(*grid_primary);

	bx_control = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	grid_primary->attach(*bx_control, 0,0,1,1);

	grid_control = Gtk::manage(new Gtk::Grid());
	grid_control->set_border_width(10);
	grid_control->set_row_spacing(5);
	bx_control->add(*grid_control);

	frm_control = Gtk::manage(new Gtk::Frame("Control"));
	frm_control->set_size_request(350, 50);
	grid_control->attach(*frm_control, 0, 0, 9, 6);

	lbl_host = Gtk::manage(new Gtk::Label("Host:"));
	grid_control->attach(*lbl_host, 1, 3, 1, 1);

	ent_host = Gtk::manage(new Gtk::Entry());
	grid_control->attach(*ent_host, 2, 3, 2, 1);

	lbl_port = Gtk::manage(new Gtk::Label("Port:"));
	grid_control->attach(*lbl_port, 1, 4, 1, 1);
	
	ent_port = Gtk::manage(new Gtk::Entry());
	grid_control->attach(*ent_port, 2, 4, 2, 1);

	btn_start = Gtk::manage(new Gtk::Button("Start"));
	btn_start->set_size_request(100, 50);
	btn_start->signal_clicked().connect(sigc::mem_fun(*this, &TesterView::sig_server_start_on_click));
	grid_control->attach(*btn_start, 5, 3, 3, 2);

	bx_serverList = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	grid_primary->attach(*bx_serverList, 0, 1, 1, 1);

	grid_serverList = Gtk::manage(new Gtk::Grid());
	grid_serverList->set_border_width(10);
	grid_serverList->set_row_spacing(10);
	bx_serverList->add(*grid_serverList);

	frm_serverList = Gtk::manage(new Gtk::Frame("List Of App Servers"));
	frm_serverList->set_size_request(350, 560);
	grid_serverList->attach(*frm_serverList, 0, 0, 3, 4);

	trv_serverList = Gtk::manage(new Gtk::TreeView());
	sw_serverList = Gtk::manage(new Gtk::ScrolledWindow());
	sw_serverList->add(*trv_serverList);
	sw_serverList->set_size_request(310, 530);
	grid_serverList->attach(*sw_serverList, 1, 2, 1, 1);
	
	refTreeModel = Gtk::ListStore::create(columns);
	trv_serverList->set_model(refTreeModel);
	trv_serverList->append_column("App Server ID", columns.server_name);
	trv_serverList->append_column("Countdown Time", columns.server_time);

	bx_connectionStatus = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	grid_primary->attach(*bx_connectionStatus, 1, 0, 1, 2);

	grid_connectionStatus = Gtk::manage(new Gtk::Grid());
	grid_connectionStatus->set_border_width(10);
	grid_connectionStatus->set_row_spacing(10);
	bx_connectionStatus->add(*grid_connectionStatus);

	frm_connectionStatus = Gtk::manage(new Gtk::Frame("Connection Status"));
	frm_connectionStatus->set_size_request(970, 400);
	grid_connectionStatus->attach(*frm_connectionStatus, 0, 0, 3, 4);

	tv_connectionStatus = Gtk::manage(new Gtk::TextView());
	tv_connectionStatus->set_editable(false);
	sw_connectionStatus = Gtk::manage(new Gtk::ScrolledWindow());
	sw_connectionStatus->add(*tv_connectionStatus);
	sw_connectionStatus->set_size_request(930, 380);
	grid_connectionStatus->attach(*sw_connectionStatus, 1, 2, 1, 1);

	bx_console = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	bx_connectionStatus->add(*bx_console);

	grid_console = Gtk::manage(new Gtk::Grid());
	grid_console->set_border_width(10);
	grid_console->set_row_spacing(10);
	bx_console->add(*grid_console);

	frm_console = Gtk::manage(new Gtk::Frame("Console"));
	frm_console->set_size_request(970, 230);
	grid_console->attach(*frm_console, 0, 0, 3, 4);

	tv_console = Gtk::manage(new Gtk::TextView());
	tv_console->set_editable(false);
	sw_console = Gtk::manage(new Gtk::ScrolledWindow());
	sw_console->add(*tv_console);
	sw_console->set_size_request(930, 200);
	grid_console->attach(*sw_console, 1, 2, 1, 1);	

	show_all_children();

	m->registerObserver(this);
}

void TesterView::updateAboutDisplay(std::string aboutInfo)
{
	Gtk::MessageDialog dlg(aboutInfo, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.set_title("About");
    dlg.run();
}

void TesterView::appendToConsole(std::string str)
{
	str += '\n';
	Glib::RefPtr<Gtk::TextBuffer> ref = tv_console->get_buffer();
	ref->insert_at_cursor(str);
}

void TesterView::appendToConnectionStatus(std::string str)
{
	str += '\n';
	Glib::RefPtr<Gtk::TextBuffer> ref = tv_connectionStatus->get_buffer();
	ref->insert_at_cursor(str);
}

void TesterView::appendRowToServerList(std::string server, int server_time)
{
	Gtk::TreeModel::Row row = *(refTreeModel->append());
    row[columns.server_name] = server;
    row[columns.server_time] = server_time;
}

void TesterView::updateRowFromServerList(std::string server, int server_time)
{
	typedef Gtk::TreeModel::Children type_children; //minimise code length.
	type_children children = refTreeModel->children();
	for(type_children::iterator iter = children.begin();
	    iter != children.end(); ++iter)
	{
		Gtk::TreeModel::Row row = *iter;
		if(row[columns.server_name] == server)
		{
			row[columns.server_time] = server_time;
			return;
		}
	}

	appendRowToServerList(server, server_time);
}

void TesterView::deleteRowFromServerList(std::string key)
{
	typedef Gtk::TreeModel::Children type_children; //minimise code length.
	type_children children = refTreeModel->children();
	for(type_children::iterator iter = children.begin();
	    iter != children.end(); ++iter)
	{
		Gtk::TreeModel::Row row = *iter;
		if(row[columns.server_name] == key)
		{
			refTreeModel->erase(iter);
			break;
		}
	}
}

void TesterView::updateButtonLabel()
{
	if(btn_start->get_label() == "Start")
		btn_start->set_label("Stop");
	else
		btn_start->set_label("Start");
}

void TesterView::consoleSetEditable(bool set)
{
	tv_console->set_editable(set);
}

void TesterView::connectionStatSetEditable(bool set)
{
	tv_connectionStatus->set_editable(set);
}

void TesterView::hostFieldSetEditable(bool set)
{
	ent_host->set_editable(set);
}

void TesterView::portFieldSetEditable(bool set)
{
	ent_port->set_editable(set);
}

std::string TesterView::getButtonLabel()
{
	return btn_start->get_label();
}

TesterView::~TesterView()
{
	delete m;
	delete c;
	
	delete lbl_host;
	delete lbl_port;
	
	delete btn_start;

	delete sw_serverList;
	delete sw_connectionStatus;
	delete sw_console;
	
	delete tv_console;
	delete tv_connectionStatus;

	delete frm_console;
	delete frm_serverList;
	delete frm_connectionStatus;
	delete frm_control;

	delete grid_serverList;
	delete grid_connectionStatus;
	delete grid_serverList;
	delete grid_control;
	delete grid_primary;
	
	delete bx_serverList;
	delete bx_connectionStatus;
	delete bx_console;
	delete bx_control;
	delete bx_primary;
	
	delete ent_host;
	delete ent_port;
	
	delete menuBar;
	delete menu_about;
	delete menuItem_help;
	delete menuItem_about;

	delete trv_serverList;
}