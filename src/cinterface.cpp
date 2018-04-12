/*! \file cinterface.cpp
	\brief Implemenation of all wrapper functions

	This file contains the implementation of all the required wrapper funcitions
  required to interface between UI in C and the Backend in C++
*/
#include "drawing.h"
#include "cinterface.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <math.h>
#include <functional>

#include "../lib/cereal/types/unordered_map.hpp"
#include "../lib/cereal/types/map.hpp"
#include "../lib/cereal/types/utility.hpp"
#include "../lib/cereal/types/string.hpp"
#include "../lib/cereal/types/list.hpp"
#include "../lib/cereal/types/vector.hpp"
#include "../lib/cereal/types/memory.hpp"
#include "../lib/cereal/archives/binary.hpp"
#include "../lib/cereal/access.hpp"

using namespace::std;

int window_1, window_2;
bool refresh = false;

static void TimeEvent(int te)
{
   if(!refresh)
   	return;
   glutSetWindow( window_1 );
   glutPostRedisplay();  // Update screen with new rotation data

   glutSetWindow( window_2 );
   glutPostRedisplay();  // Update screen with new rotation data

   glutTimerFunc( 100, TimeEvent, 1);  // Reset our timmer.
}

extern "C" int _2d3dDrawWrapper(char* filepath,int argc, char** argv){
  //cout<<filepath<<endl;
  ifstream file;
  Projection FV, TV, SV;
  if(regex_match (filepath, regex("(.*cdp2)"))) {
    file.open(filepath, ios::binary);
    if(file){
      //cout<<"File Opened"<<endl;
      {
        cereal::BinaryInputArchive iarchive(file);
        iarchive(default_fv,default_tv,default_sv);
        //cout<<"Objects Read"<<endl;
        default_ob.create(default_fv,default_tv,default_sv);
        display_main(argc,argv);
      }
      file.close();
      return 0;
    }
    else {
      //cout<<"File Not Found"<<endl;
      return 1;
    }
  }
  else{
    //cout<<"Incorrect File Type"<<endl;
    return 2;
  }
}

extern "C" int _3d2dDrawWrapper(char* filepath,int argc, char** argv){
  //cout<<filepath<<endl;
  ifstream file;
  if(regex_match (filepath, regex("(.*cdp3)"))) {
    file.open(filepath,ios::binary);
    if(file){
      //cout<<"File Opened"<<endl;
      {
        cereal::BinaryInputArchive iarchive(file);
        iarchive(default_ob);
        //cout<<"Objects Read"<<endl;
        default_fv = default_ob.projectTo2D("front");
        default_tv = default_ob.projectTo2D("top");
        default_sv = default_ob.projectTo2D("side");
        display_main(argc,argv);
      }
      file.close();
      return 0;
    }
    else {
      //cout<<"File Not Found"<<endl;
      return 1;
    }
  }
  else{
    //cout<<"Incorrect File Type"<<endl;
    return 2;
  }
}

extern "C" void _rotateWrapper(float alpha, float beta, float gamma, int argc, char** argv){
  default_ob.rotate(fmod(alpha,360),fmod(beta,360),fmod(gamma,360));
  display_main(argc,argv);
}

extern "C" int _SaveWrapper(char* savepath){
  cout<<savepath<<endl;
  ofstream file;
  if(regex_match (savepath, regex("(.*cdp3)"))) {
    file.open(savepath,ios::binary);
    if(file){
      //cout<<"File Opened"<<endl;
      {
        cereal::BinaryOutputArchive oarchive(file);
        oarchive(default_ob);
        //cout<<"Objects Written"<<endl;
        return 0;
      }
    }
    else{
      //cout<<"File Not Found"<<endl;
      return 1;
    }
    file.close();
  }
  else{
    if(regex_match (savepath, regex("(.*cdp2)"))) {
      file.open(savepath,ios::binary);
      if(file){
        //cout<<"File Opened"<<endl;
        {
          cereal::BinaryOutputArchive oarchive(file);
          oarchive(default_fv,default_sv,default_tv);
          //cout<<"Objects Written"<<endl;
          return 0;
        }
      }
      else{
        //cout<<"File Not Found"<<endl;
        return 1;
      }
      file.close();
    }
    else{
      //cout<<"Incorrect File Type"<<endl;
      return 2;
    }
  }
}

extern "C" void display_main(int argc, char** argv){
  glutInit(&argc, argv);            // Initialize GLUT
	//glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutTimerFunc( 10, TimeEvent, 1);


  if(default_ob.flist.empty())
  {
  window_1 = glutCreateWindow("Orthographics");          // Create window with the given title
  glutDisplayFunc(default_tv.display);       // Register callback handler for window re-paint event
  glutReshapeFunc(reshape);       // Register callback handler for window re-size event
  initGL();

  window_2 = glutCreateWindow("3D wireframe");
  glutDisplayFunc(default_ob.display_wireframe);       // Register callback handler for window re-paint event
  glutReshapeFunc(reshape3D);       // Register callback handler for window re-size event
  initGL3D();
  }
  else
  {
	window_1 = glutCreateWindow("Orthographics");          // Create window with the given title
	glutDisplayFunc(default_tv.display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	initGL();                       // Our own OpenGL initialization

	glutInitWindowPosition(700, 50);
	window_2 = glutCreateWindow("3D - Isometeric");          // Create window with the given title
	glutDisplayFunc(default_ob.display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape3D);       // Register callback handler for window re-size event
	initGL3D();
  }

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);

	glutMainLoop();
}
