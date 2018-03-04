/*! \file GUI.h
	\brief Header file for GUI functions definitions

	This is the header file for the GUI part of the application.
	Libraries used:
		- GTK+
*/

#include <fstream>
#include <stdlib.h>
#include <gtk/gtk.h>

class HomeScreen {
  private:
  public:
      ScreenPainter();
      EventHandler();
      ErrorHandler();
}

class inputScreen2d3d {
  private:
  public:
      ScreenPainter();
      EventHandler();
      ErrorHandler();
}

class inputScreen3d2d {
  private:
  public:
      ScreenPainter();
      EventHandler();
      ErrorHandler();
}

class outputScreen2d3d {
  private:
  public:
      ScreenPainter();
      EventHandler();
      ErrorHandler();
}

class outputScreen3d2d {
  private:
  public:
      ScreenPainter();
      EventHandler();
      ErrorHandler();
}
