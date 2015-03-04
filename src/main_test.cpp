#include "model_test.h"
#include "controller_test.h"

#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);

	TesterModel m;
	TesterController c(&m);
	return 0;
}