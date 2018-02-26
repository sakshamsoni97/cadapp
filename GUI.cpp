/*! \file GUI.cpp
	\brief Frontend of the application

	Front end and the UI of the application is handled here. UI is designed using GTK+ library. 
	Provides an interface between the methods in drawing.h and the user.
	Libraries used:
		- GTK+
		- fstream
		- stdlib
*/
#include <fstream>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "drawing.h"
#include "GUI.h"

struct _CADApp
{
  GtkApplication parent;
};

G_DEFINE_TYPE(CADApp, cad_app, GTK_TYPE_APPLICATION);

static void example_app_init (CADApp *app)
{
}
/*
static void preferences_activated (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       app)
{
  ExampleAppPrefs *prefs;
  GtkWindow *win;

  win = gtk_application_get_active_window (GTK_APPLICATION (app));
  prefs = example_app_prefs_new (EXAMPLE_APP_WINDOW (win));
  gtk_window_present (GTK_WINDOW (prefs));
}
*/

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry app_entries[] =
{
  { "preferences", preferences_activated, NULL, NULL, NULL },
  { "quit", quit_activated, NULL, NULL, NULL }
};

static void
cad_app_startup (GApplication *app)
{
  GtkBuilder *builder;
  GMenuModel *app_menu;
  const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };

  G_APPLICATION_CLASS (cad_app_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app),
                                   app_entries, G_N_ELEMENTS (app_entries),
                                   app);
  gtk_application_set_accels_for_action (GTK_APPLICATION (app),
                                         "app.quit",
                                         quit_accels);

  builder = gtk_builder_new_from_resource ("cadapp-menu.ui");
  app_menu = G_MENU_MODEL (gtk_builder_get_object (builder, "cadappmenu"));
  gtk_application_set_app_menu (GTK_APPLICATION (app), app_menu);
  g_object_unref (builder);
}

static void
cad_app_activate (GApplication *app)
{
  CADAppWindow *win;

  win = cad_app_window_new (CAD_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
cad_app_open (GApplication  *app,
                  GFile     **files,
                  gint       n_files,
                  const gchar   *hint)
{
  GList *windows;
  CADAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = CAD_APP_WINDOW (windows->data);
  else
    win = cad_app_window_new (EXAMPLE_APP (app));

  for (i = 0; i < n_files; i++)
    cad_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static void
cad_app_class_init (CADAppClass *class)
{
  G_APPLICATION_CLASS (class)->startup = cad_app_startup;
  G_APPLICATION_CLASS (class)->activate = cad_app_activate;
  G_APPLICATION_CLASS (class)->open = cad_app_open;
}

CADApp *
cad_app_new (void)
{
  return g_object_new (CAD_APP_TYPE,
                       "application-id", "cadapp",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}


struct _CADAppWindow
{
  GtkApplicationWindow parent;
};

typedef struct _CADAppWindowPrivate CADAppWindowPrivate;

struct _CADAppWindowPrivate
{
  GtkWidget *generate_button;
  GtkWidget *home_button;
  GtkWidget *back_button;

};

G_DEFINE_TYPE_WITH_PRIVATE(CADAppWindow, cad_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
cad_app_window_init (CADAppWindow *win)
{
}

static void
cad_app_window_dispose (GObject *object)
{
}

static void
cad_app_window_class_init (CADAppWindowClass *class)
{
}

VADAppWindow *
cad_app_window_new (CADApp *app)
{
  return g_object_new (CAD_APP_WINDOW_TYPE, "application", app, NULL);
}

void
cad_app_window_open (CADAppWindow *win,
                         GFile            *file)
{
}




