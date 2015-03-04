#ifndef __VIEW_TEST_H
#define __VIEW_TEST_H

#include <gtkmm.h>
#include <gtkmm/label.h>

#include <string>

class TesterModel;
class TesterController;

class TesterView : public Gtk::Window
{
private:
	// mvc elements
	TesterModel *m;
	TesterController *c;

	// scrolled window
	Gtk::ScrolledWindow *sw_serverList;
	Gtk::ScrolledWindow *sw_connectionStatus;
	Gtk::ScrolledWindow *sw_console;

	// menu and menu items
	Gtk::MenuBar *menuBar;
	Gtk::MenuItem *menuItem_help;
	Gtk::Menu *menu_about;
	Gtk::MenuItem *menuItem_about;

	// buttons
	Gtk::Button *btn_start;

	// entries
	Gtk::Entry *ent_host, *ent_port;

	// text view
	Gtk::TextView *tv_connectionStatus;
	Gtk::TextView *tv_console;

	// Boxes
	Gtk::Box *bx_primary;
	Gtk::Box *bx_control;
	Gtk::Box *bx_serverList;
	Gtk::Box *bx_connectionStatus;
	Gtk::Box *bx_console;
	
	// grids
	Gtk::Grid *grid_primary;
	Gtk::Grid *grid_control;
	Gtk::Grid *grid_serverList;
	Gtk::Grid *grid_connectionStatus;
	Gtk::Grid *grid_console;

	// label
	Gtk::Label *lbl_host;
	Gtk::Label *lbl_port;

	// frame
	Gtk::Frame *frm_control;
	Gtk::Frame *frm_serverList;
	Gtk::Frame *frm_connectionStatus;
	Gtk::Frame *frm_console;

	// tree view
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:
		ModelColumns() { add(server_name); add(server_time); }
		
		Gtk::TreeModelColumn<Glib::ustring> server_name;
		Gtk::TreeModelColumn<int> server_time;
	};

	Gtk::TreeView *trv_serverList;
	ModelColumns columns;
	Glib::RefPtr<Gtk::ListStore> refTreeModel;

private:
	// signals
	void sig_about_on_click();
	void sig_server_start_on_click();

public:
	explicit TesterView(TesterModel *m, TesterController *c);
	~TesterView();

	void updateAboutDisplay(std::string);
	void appendToConsole(std::string);
	void appendToConnectionStatus(std::string);
	void appendRowToServerList(std::string server, int server_time);
	void updateRowFromServerList(std::string server, int server_time);
	void deleteRowFromServerList(std::string key);
	void updateButtonLabel();
	
	void consoleSetEditable(bool set);
	void connectionStatSetEditable(bool set);
	void hostFieldSetEditable(bool set);
	void portFieldSetEditable(bool set);

	std::string getButtonLabel();
};

#endif