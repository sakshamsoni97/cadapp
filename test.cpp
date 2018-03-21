#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "drawing.h"

/*#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

GLFWwindow* window;
*/
int main(int argc, char** argv){
  vertex v1(0.0,0.0,0.0), v2(1.0,0.0,0.0), v3(1.0,1.0,0.0), v4(0.0,1.0,0.0), v5(0.0,0.0,1.0), v6(1.0,0.0,1.0), v7(1.0,1.0,1.0), v8(0.0,1.0,1.0);
  edge e1(v1,v2), e2(v2,v3), e3(v3,v4), e4(v4,v1), e5(v5, v6), e6(v6,v7), e7(v8,v7), e8(v5,v8), e9(v1,v5), e10(v2,v6), e11(v7,v3), e12(v4,v8);
  face f1, f2, f3, f4, f5, f6;
  Object3D ob;
  // initializing faces
  // f1
  f1.edges["1_12"] = e1;
  f1.edges["2_23"] = e2;
  f1.edges["3_34"] = e3;
  f1.edges["4_14"] = e4;
  f1.verts["1"] = v1;
  f1.verts["2"] = v2;
  f1.verts["3"] = v3;
  f1.verts["4"] = v4;
  
  // f2
  f2.edges["1_56"] = e5;
  f2.edges["2_67"] = e6;
  f2.edges["3_78"] = e7;
  f2.edges["4_58"] = e8;
  f2.verts["1"] = v5;
  f2.verts["2"] = v6;
  f2.verts["3"] = v7;
  f2.verts["4"] = v8;

  // f3
  f3.edges["1_34"] = e3;
  f3.edges["4_37"] = e11;
  f3.edges["3_78"] = e7;
  f3.edges["2_48"] = e12;
  f3.verts["1"] = v4;
  f3.verts["2"] = v3;
  f3.verts["3"] = v7;
  f3.verts["4"] = v8;

  // f4
  f4.edges["1_12"] = e1;
  f4.edges["2_26"] = e10;
  f4.edges["3_56"] = e5;
  f4.edges["4_15"] = e9;
  f4.verts["1"] = v1;
  f4.verts["2"] = v2;
  f4.verts["3"] = v6;
  f4.verts["4"] = v5;

  // f5
  f5.edges["1_26"] = e10;
  f5.edges["2_67"] = e6;
  f5.edges["3_37"] = e11;
  f5.edges["4_23"] = e2;
  f5.verts["1"] = v2;
  f5.verts["2"] = v6;
  f5.verts["3"] = v7;
  f5.verts["4"] = v3;

  // f6
  f6.edges["1_15"] = e5;
  f6.edges["2_58"] = e8;
  f6.edges["3_48"] = e12;
  f6.edges["4_14"] = e4;
  f6.verts["1"] = v1;
  f6.verts["2"] = v5;
  f6.verts["3"] = v8;
  f6.verts["4"] = v4;

  // initialize object
  // flist
  ob.flist["a"] = f1;
  ob.flist["b"] = f2;
  ob.flist["c"] = f3;
  ob.flist["d"] = f4;
  ob.flist["e"] = f5;
  ob.flist["f"] = f6;
  // elist
  ob.elist["12"] = e1;
  ob.elist["23"] = e2;
  ob.elist["34"] = e3;
  ob.elist["14"] = e4;
  ob.elist["56"] = e5;
  ob.elist["67"] = e6;
  ob.elist["78"] = e7;
  ob.elist["58"] = e8;
  ob.elist["15"] = e9;
  ob.elist["26"] = e10;
  ob.elist["37"] = e11;
  ob.elist["48"] = e12;
  // vlist
  ob.vlist["1"] = v1;
  ob.vlist["2"] = v2;
  ob.vlist["3"] = v3;
  ob.vlist["4"] = v4;
  ob.vlist["5"] = v5;
  ob.vlist["6"] = v6;
  ob.vlist["7"] = v7;
  ob.vlist["8"] = v8;

  ob.rotate(-45.0f, -45.0f, 0.0f);

  Projection FV= ob.projectTo2D("front");
  Projection TV= ob.projectTo2D("top");
  Projection SV= ob.projectTo2D("side");

  ofstream out1("object.cdpi",ios::out | ios::binary);
  out1.write((char *)&ob, sizeof(ob));
  out1.flush();

  ofstream out2("front.cdpo",ios::out | ios::binary);
  out2.write((char *)&FV, sizeof(FV));
  out2.flush();

  ofstream out3("top.cdpo",ios::out | ios::binary);
  out3.write((char *)&TV, sizeof(TV));
  out3.flush();

  ofstream out4("side.cdpo",ios::out | ios::binary);
  out4.write((char *)&SV, sizeof(SV));
  out4.flush();
  /*
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
  */

	return 0;
}
