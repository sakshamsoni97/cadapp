/*! \file GUI.h
	\brief Header file for GUI functions definitions

	This is the header file for the GUI part of the application.
	Libraries used:
		- GTK+
*/
#ifndef __GUI_H
#define __GUI_H

#include <gtk/gtk.h>


#define CAD_APP_TYPE (cad_app_get_type ())
#define CAD_APP_WINDOW_TYPE (example_app_window_get_type ())
G_DECLARE_FINAL_TYPE (CADApp, cad_app, CAD, APP, GtkApplication)
G_DECLARE_FINAL_TYPE (ExampleAppWindow, example_app_window, EXAMPLE, APP_WINDOW, GtkApplicationWindow)

CADApp  *cad_app_new (void);

ExampleAppWindow *example_app_window_new(ExampleApp *app);
void example_app_window_open(ExampleAppWindow *win, GFile *file)

#endif

