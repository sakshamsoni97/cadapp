/*! \file cadapp.cpp
	\brief Main execution starting point

	The program execution starts here. It contains the main() function.
*/
#include <GTK/gtk.h>
#include "GUI.h"

using namespace::std;


int main(int argc, char **argv)
{
	return g_application_run (G_APPLICATION (cad_app_new()), argc, argv);
}
