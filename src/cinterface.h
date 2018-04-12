/*! \file cinterface.h

	\brief Provides wrapper functions to interface between GUI (written using GTK+ in C)
  with the backend (written in C++)

*/

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif


//! 2D to 3D conversion wrapper function
/*!
Wrapper Function invoked when 3D object has to be reconstructed from its
projections and displayed
*/
int _2d3dDrawWrapper(char* filepath,int argc, char** argv);

//! 3D to 2D conversion wrapper function
/*!
Wrapper Function invoked when 2D projections have to be made for an object and
displayed
*/
int _3d2dDrawWrapper(char* filepath,int argc, char** argv);

//! Rotation wrapper function
/*!
Wrapper Function invoked when the object is rotated
*/
void _rotateWrapper(float alpha, float beta, float gamma, int argc, char** argv);

//! Rotation wrapper function
/*!
Wrapper Function invoked when the object/projection have to be stored
*/
int _SaveWrapper(char* savepath);

//! Objection/Projection display function
/*!
Function invoked when object and its projection shave to be displayed
*/
void display_main(int argc, char** argv);

static void TimeEvent(int te);

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif
