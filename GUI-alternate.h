/*! \file GUI.h
	\brief Header file for GUI functions definitions

	This is the header file for the GUI part of the application.
	Libraries used:
		- GTK+
*/

#include <fstream>
#include <stdlib.h>
#include <gtk/gtk.h>

//! Class for the home screen UI
/*!
Helps the user to select the kind of conversion to be performed (2D-3D or 3D-2D)
*/
class HomeScreen {
  private:
  public:
      /*! method to display the UI elements composing the home screen */
      ScreenPainter();
      /*! method to invoke correct behavior depending on event/action invoked by user */
      EventHandler();
      /*! method to improve robustness and handle errors/incorrect calls with/or console output */
      ErrorHandler();
}

//! Class for the for 2D to 3D conversion input screen UI
/*!
Helps the user to interact and specify the input specification of orthographic views to convert to a 3D object
*/
class inputScreen2d3d {
  private:
  public:
      /*! method to display the UI elements composing the input screen */
      ScreenPainter();
      /*! method to invoke correct behavior depending on event/action invoked by user */
      EventHandler();
      /*! method to improve robustness and handle errors/incorrect calls with/or console output */
      ErrorHandler();
}

//! Class for the for 3D to 2D conversion input screen UI
/*!
Helps the user to interact and specify the input specification of a 3D object and prepare its orthographic projection
*/
class inputScreen3d2d {
  private:
  public:
      /*! method to display the UI elements composing the input screen */
      ScreenPainter();
      /*! method to invoke correct behavior depending on event/action invoked by user */
      EventHandler();
      /*! method to improve robustness and handle errors/incorrect calls with/or console output */
      ErrorHandler();
}


//! Class for the for 2D to 3D conversion output screen UI
/*!
Displays the corresponding 3D object for the orthographic views specified in the input screen
*/
class outputScreen2d3d {
  private:
  public:
      /*! method to display the UI elements composing the output screen */
      ScreenPainter
      /*! method to invoke correct behavior depending on event/action invoked by user */
      EventHandler();
      /*! method to improve robustness and handle errors/incorrect calls with/or console output */
      ErrorHandler();
}

//! Class for the for 3D to 2D conversion output screen UI
/*!
Displays the corresponding orthographic projections for the 3D object specified in the input screen
*/
class outputScreen3d2d {
  private:
  public:
      /*! method to display the UI elements composing the output screen */
      ScreenPainter();
      /*! method to invoke correct behavior depending on event/action invoked by user */
      EventHandler();
      /*! method to improve robustness and handle errors/incorrect calls with/or console output */
      ErrorHandler();
}
