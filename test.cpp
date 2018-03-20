#include <iostream>
/*#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <utility>
#include <string>
#include <list>
#include <vector>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gtk/gtk.h>
#include <math.h>
#include <limits>
#include <ctime>
*/
#include <GL/glut.h>
#include "drawing.h"

/*#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

GLFWwindow* window;
*/
int main(int argc, char** argv){
  //Projections;
  Projection FV, TV, SV;
  //Initialising Projections
  //FV
  FV.name = "FV";
  struct vert2D v1(0.0f,0.0f), v2(1.0f,0.0f), v3(1.0f,1.0f), v4(0.0f,1.0f);
  struct edge2D e1(v1,v2), e2(v2,v3), e3(v3,v4), e4(v1,v4);
  FV.vlist["v1"] = v1;
  FV.vlist["v2"] = v2;
  FV.vlist["v3"] = v3;
  FV.vlist["v4"] = v4;
  FV.elist["1_12"] = e1;
  FV.elist["2_23"] = e2;
  FV.elist["3_34"] = e3;
  FV.elist["4_14"] = e4;

  struct vert2D v5(0.0f,0.0f), v6(1.0f,0.0f), v7(1.0f,1.0f), v8(0.0f,1.0f);
  struct edge2D e5(v5,v6), e6(v6,v7), e7(v7,v8), e8(v5,v8);
  TV.vlist["v1"] = v5;
  TV.vlist["v2"] = v6;
  TV.vlist["v3"] = v7;
  TV.vlist["v4"] = v8;
  TV.elist["5_56"] = e5;
  TV.elist["6_67"] = e6;
  TV.elist["7_78"] = e7;
  TV.elist["8_58"] = e8;

  struct vert2D v9(0.0f,0.0f), v10(1.0f,0.0f), v11(1.0f,1.0f), v12(0.0f,1.0f);
  struct edge2D e9(v9,v10), e10(v10,v11), e11(v11,v12), e12(v9,v12);
  SV.vlist["v1"] = v9;
  SV.vlist["v2"] = v10;
  SV.vlist["v3"] = v11;
  SV.vlist["v4"] = v12;
  SV.elist["9_910"] = e9;
  SV.elist["10_1011"] = e10;
  SV.elist["11_1112"] = e11;
  SV.elist["12_912"] = e12;

  //cout<<e10.v1.x<<endl;
  //cout<<TV.elist["10_1011"].v2.x<<endl;
  //TV = Projection(FV);
  //SV = Projection(FV);

  Object3D object;
  object.create(FV,TV,SV);

  object.rotate(-45.0f, -45.0f, 0.0f);

  glutInit(&argc, argv);            // Initialize GLUT
  //glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glutInitWindowSize(640, 480);   // Set the window's initial width & height
  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
  glutCreateWindow("test run");          // Create window with the given title
  
  default_ob = object;
  glutDisplayFunc(object.display_wireframe);       // Register callback handler for window re-paint event
  glutReshapeFunc(reshape3D);       // Register callback handler for window re-size event
  initGL3D();                       // Our own OpenGL initialization
  glutMainLoop();


	return 0;
}
