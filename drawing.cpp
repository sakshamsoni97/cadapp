/*! \file drawing.cpp
	\brief Implementation of the algorithms

	This file contains the implementation of all the required algorithms for the CAD application.
*/
#include <map>
#include <utility>
#include <string>
#include <list>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <gtk/gtk.h>
#include <jsoncpp/json/json.h>
#include "pngwriter.h"
#include <math.h>
#include <limits>
#include "drawing.h"

using namespace::std;

void initGL(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void initGL3D() {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}

void reshape3D(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

vector <float> cross_prod(float a[3], float b[3]){
	vector <float> res = {0.0, 0.0, 0.0};
	// return cross product of *a* and *b*
	res[0] = a[1]*b[2] - b[1]*a[2];
	res[1] = -a[0]*b[2] + b[0]*a[2];
	res[2] = a[0]*b[1] - a[1]*b[0];

	return res;
}
/*
void save_png(char* filename){
	int width = glutGet(GLUT_SCREEN_WIDTH);
    int height = glutGet( GLUT_SCREEN_HEIGHT);
    int npixels = width*height;
	GLubyte* OpenGLimage = new GLubyte[npixels];
	glReadPixels(0.0, 0.0, width, height, GL_RGB, GL_FLOAT, OpenGLimage);
	pngwriter PNG(width, height, 1.0, filename);
	size_t x = 1;   // start the top and leftmost point of the window
	size_t y = 1;
	unsigned int R, G, B;
	for(size_t i =0; i<npixels; i+=3){
		//R =  *OpenGLimage[i+1]p+OpenGLimage[i+2]; G = *p++; B =  *p++;
		PNG.plot(x, y, OpenGLimage[i], OpenGLimage[i+1], OpenGLimage[i+2]);
		x++;
		if(x>width) {x=1; y++;}
	}
	PNG.close();
}
*/
vertex::vertex(){x=0; y=0; z=0;}

vertex::vertex(float _x, float _y, float _z){x=_x; y=_y; z = _z;}

vert2D::vert2D(){x=0; y=0;}

vert2D::vert2D(const vertex &v){
	x = v.x;
	y = v.y;
}

vert2D::vert2D(float _x, float _y){x=_x; y=_y;}

vert2D::vert2D(const vert2D &v){x = v.x; y = v.y; }

edge2D::edge2D(){ visi = true;}

edge2D::edge2D(vert2D a, vert2D b, bool v){v1 = a; v2 = b; visi = v;}

edge2D::edge2D(const edge e){
	v1 = e.v1;
	v2 = e.v2;
	visi = e.visi;
}

edge::edge(vertex a, vertex b, bool v){v1 = a; v2 = b; visi = v;}

edge::edge(){ visi = true;}

face::face(){
	A=B=C=D=0.0;
}

void  face::compParam(){
	float x[3], y[3];
	vector <float> t(3);

	map< string, edge>::iterator it = face::edges.begin();
	x[0] = (it->second.v1.x - it->second.v2.x) ; x[1] = (it->second.v1.y - it->second.v2.y) ; x[2]= (it->second.v1.z - it->second.v2.z);

	bool col = true;
	while(col && it!=face::edges.end()){
		advance(it,1);
		y[0] = (it->second.v1.x - it->second.v2.x) ; y[1] = (it->second.v1.y - it->second.v2.y) ; y[2] = (it->second.v1.z - it->second.v2.z);
		t = cross_prod(x, y);
		if(t[1]==t[2] && t[2]==t[3] && t[3]==0)
			continue;
		col = false;
		}
	if(col)
		throw std::invalid_argument("problem in face");
	else{
		face::A = t[1]; face::B = t[1]; face::C = t[2];
		face::D = A*it->second.v1.x + B*it->second.v1.y + C*it->second.v1.z;
	}
}

void Projection::display(){
	glClear(GL_COLOR_BUFFER_BIT);

	for(const auto& ed : default_pr.elist){
		glBegin(GL_LINES);
		glLineWidth(2);
		glEnable(GL_LINE_SMOOTH);
		if(!ed.second.visi){
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(2, 0x00FF);
			glColor3f(0.2f, 0.2f, 0.2f);
		}
		else
			glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(ed.second.v1.x, ed.second.v1.y);
		glVertex2f(ed.second.v2.x, ed.second.v2.y);
		if(!ed.second.visi)
			glDisable(GL_LINE_STIPPLE);
		glDisable(GL_LINE_SMOOTH);
		glEnd();
	}

	glFlush();
}

void Projection::getProjection(){

}

void Object3D::display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   	// Render a color-cube consisting of 6 quads with different colors
  	glLoadIdentity();                 // Reset the model-view matrix
   	glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen
 
   	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   	glLineWidth(2);
   	glEnable(GL_LINE_SMOOTH);
   	glColor3f(0.0f, 0.0f, 0.0f);	

   	glBegin(GL_LINE_LOOP);

   	for(auto& sp : default_ob.flist){
		for(auto& ed : sp.second.edges){
			glVertex3f(ed.second.v1.x, ed.second.v1.y, ed.second.v1.z);
			glVertex3f(ed.second.v2.x, ed.second.v2.y, ed.second.v2.z);
		}
	}
   
   	glEnd();   // Done drawing the pyramid
   	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
 
   	glutSwapBuffers(); 
	glFlush();
}

void swap(float &a, float &b){
	float t = a;
	a = b;
	b = t;
}

pair <float, float> Object3D::_intersect_ratiois(edge e1, edge e2){
	float u, t, num, den;
	den = (e2.v2.x - e2.v1.x)*(e1.v2.y - e1.v1.y) - (e2.v2.y - e2.v1.y)*(e1.v2.x - e1.v1.x);
	if(den==0.0)
		return make_pair(999.0, 999.0);
	num = (e2.v2.y - e2.v1.y)*(e1.v1.x - e2.v1.x) - (e2.v2.x - e2.v1.x)*(e1.v1.y - e2.v1.y);  
	u = num/den;
	den = (e1.v2.x - e1.v1.x)*(e2.v2.y - e2.v1.y) - (e1.v2.y - e1.v1.y)*(e2.v2.x - e2.v1.x); 
	if(den==0.0)
		return make_pair(99.0, 999.0);
	num = (e1.v2.y - e1.v1.y)*(e2.v1.x - e1.v1.x) - (e1.v2.x - e1.v1.x)*(e2.v1.y - e1.v1.y);
	t = num/den;

	return make_pair(u, t);
}

float _point_on_segment(vertex v1, vertex v2, vertex v){
	if(v1.x == v2.x && v1.y == v2.y)
		if(v1.x == v.x && v1.y == v.y)
			return 0.0f;
		else
			return numeric_limits<float>::infinity();
	float m = v1.x == v2.x? (v.y-v1.y)/(v2.y-v1.y) :(v.x-v1.x)/(v2.x-v1.x);
	return m;
}

void Object3D::_overlappingEdges(map <string, edge> &els, map <string, vertex> &vls){
	map <string, edge>::iterator eit1 = els.begin();
	map <string, edge>::iterator eit2 = next(eit1,1);

	while(eit1 != els.end()){
		while(eit2 != els.end()){
			edge e1 = eit1->second, e2 = eit2->second;
			float m1, m2, m_1, m_2;
			m1 = e1.v1.x == e1.v2.x ? numeric_limits<float>::infinity() : (e1.v1.y - e1.v2.y)/(e1.v1.x - e1.v2.x);
			m1 = e2.v1.x == e2.v2.x ? numeric_limits<float>::infinity() : (e2.v1.y - e2.v2.y)/(e2.v1.x - e2.v2.x);
			if(m1==m2 && ((e1.v1.y - m1*e1.v1.x) == (e2.v1.y - m2*e2.v1.x))){
				m_1 = _point_on_segment(e1.v1, e1.v2, e2.v1);
				m_2 = _point_on_segment(e1.v1, e1.v2, e2.v2);
				
 				if(m_1<=1.0 && m1>=0.0 && m_2<=1.0 && m2>=0.0){
 					if(e2.v1.z<= e1.v1.z)
 						{els.erase(eit2); continue;}
 					if(e2.v1.z> e1.v1.z){
 						if(m_1==0.0 && m_2!=1.0){
 							eit1->second.v1 = e2.v2;
 						}else if(m_1==1.0 && m_2!=0.0){
 							eit1->second.v2 = e2.v2;
 						}else if(m_2==0.0 && m_1!=1.0){
 							eit1->second.v1 = e2.v1;
 						}else if(m_2==1.0 && m_1!=0.0){
 							eit1->second.v2 = e2.v1;
 						}else{
 							edge e;
	 						if(m_1<m_2)
	 							{e = edge(e2.v2, e1.v2); eit1->second.v2 = e2.v1;}
	 						else
	 							{e = edge(e2.v1, e1.v2); eit1->second.v2 = e2.v2;}
							els.insert(pair<string, edge>(eit1->first, e));
						}	
 					}
 				}
 				if((m_1>=1.0 && m2<=0.0) || (m_2>=1.0 && m1<=0.0)){
 					if(e2.v1.z<= e1.v1.z){
 						if(m_1==0.0 && m_2!=1.0){
 							eit2->second.v1 = e1.v2;
 						}else if(m_1==1.0 && m_2!=0.0){
 							eit2->second.v1 = e1.v1;
 						}else if(m_2==0.0 && m_1!=1.0){
 							eit2->second.v2 	= e1.v2;
 						}else if(m_2==1.0 && m_1!=0.0){
 							eit2->second.v2 = e1.v1;
 						}else{
 							edge e;
	 						if(m_1<m_2)
	 							{e = edge(e1.v2, e2.v2); eit2->second.v2 = e1.v1;}
	 						else
	 							{e = edge(e1.v1, e2.v2); eit2->second.v2 = e1.v2;}
							els.insert(pair<string, edge>(eit2->first, e));
						}
 					}
 					if(e2.v1.z>= e1.v1.z){
 						map <string, edge>::iterator it = prev(eit1,1);
 						els.erase(eit1);
 						eit1 = it; 
 						break;
 					}
 				}

 				if(m_1<1.0 && m1>0.0 && m_2>1.0){
 					if(e2.v1.z<=e1.v2.z)
 						eit2->second.v1 = e1.v2;
 					else
 						eit1->second.v2 = e2.v1;
 				}else if(m_1<1.0 && m1>0.0 && m_2<0.0){
 					if(e2.v1.z<=e1.v1.z)
 						eit2->second.v1 = e1.v1;
 					else
 						eit1->second.v1 = e2.v1;
 				}else if(m_2<1.0 && m2>0.0 && m_1<0.0){
 					if(e2.v2.z<=e1.v1.z)
 						eit2->second.v2 = e1.v1;
 					else
 						eit1->second.v1 = e2.v2;
 				}else if(m_2<1.0 && m2>0.0 && m_1>1.0){
 					if(e2.v2.z<=e1.v2.z)
 						eit2->second.v2 = e1.v2;
 					else
 						eit1->second.v2 = e2.v2;
 				}

			}
			advance(eit2, 1);
		}
		advance(eit1, 1);
		eit2 = next(eit1, 1);
	}	
}

void Object3D::_intersectingEdges(map <string, edge> &els, map <string, vertex> &vls){
	map <string, edge>::iterator eit1 = els.begin();
	map <string, edge>::iterator eit2 = next(eit1,1);

	while(eit1 != els.end()){
		while(eit2 != els.end()){
			edge e1 = eit1->second, e2 = eit2->second;
			pair <float, float> rs= _intersect_ratiois(eit1->second, eit2->second);
			if(rs.first>0 && rs.first<1 && rs.second>0 && rs.second<1){
				vertex vi;
				vi.x = e1.v1.x + rs.first*(e1.v2.x - e1.v1.x);
				vi.y = e1.v1.y + rs.first*(e1.v2.y - e1.v1.y); 
				{
					vi.z = e1.v1.z + rs.first*(e1.v2.z - e1.v1.z);
					edge e = edge(vi, e1.v2);
					els.insert(pair<string, edge>(eit1->first, e));
					eit1->second.v2 = vi;
					vls.insert(pair<string, vertex>(eit1->first, vi));
				}
				{
					vi.z = e2.v1.z + rs.second*(e2.v2.z - e2.v1.z);
					edge e = edge(vi, e2.v2);
					els.insert(pair<string, edge>(eit2->first, e));
					eit2->second.v2 = vi;
					vls.insert(pair<string, vertex>(eit2->first, vi));	
				}
			}
			advance(eit2, 1);
		}
		advance(eit1, 1);
		eit2 = next(eit1, 1);
	}
}

bool _point_behind_face(vertex v, face fc){
	fc.compParam();
	if(fc.C == 0)
		return false;
	float z_ = (fc.D - fc.A*v.x -fc.B*v.y)/fc.C;
	if(z_ <= v.z)
		return false;

	int count = 0;
	for(auto& sp : fc.edges){
		edge e2 = sp.second;
		if(e2.v1.y==e2.v2.y)
			continue;

		if(v.y == e2.v1.y && e2.v1.y < e2.v2.y)
			continue;

		if(v.y == e2.v2.y && e2.v1.y > e2.v2.y)
			continue;

		float m = (v.y - e2.v1.y)/(e2.v2.y - e2.v1.y);
		if(m>0.0 && m<1.0)
			count++;
	}

	if(count%2==1)
		return true;
	return false;		
}

void Object3D::_dashedLines(map <string, edge> &els, map <string, face> &fls){
	for(auto& sp : els){
		edge e = sp.second;
		for(auto& sp1 : fls){
			if(_point_behind_face(e.v1, sp1.second))
				sp.second.visi = false;
			else if(_point_behind_face(e.v2, sp1.second))
				sp.second.visi = false;
			else
				sp.second.visi = true;
		}
	}
}


Projection Object3D::projectTo2D(string view){
	map <string, face> _flist = flist;
	map <string, edge> _elist = elist;
	map <string, vertex> _vlist = vlist;
	
	if(view=="top"){
		for(auto& sp : _elist){
			swap(sp.second.v1.y, sp.second.v1.z); 
			sp.second.v1.y*=-1;
			swap(sp.second.v2.y, sp.second.v2.z);
			sp.second.v2.y*=-1;
		} 
		for(auto& sp : _vlist){
			swap(sp.second.y, sp.second.z);
			sp.second.y*=-1;
		}
		for(auto& sp : _flist){
			for(auto& sp2 : sp.second.edges){
				swap(sp2.second.v1.y, sp2.second.v1.z);
				sp2.second.v1.y*=-1;
				swap(sp2.second.v2.y, sp2.second.v2.z);
				sp2.second.v2.y*=-1;
			}
		}
	}

	if(view=="side"){
		for(auto& sp : _elist){
			swap(sp.second.v1.x, sp.second.v1.z);
			sp.second.v1.x*=-1;
			swap(sp.second.v2.x, sp.second.v2.z);
			sp.second.v2.x*=-1;
		}
		for(auto& sp : _vlist){
			swap(sp.second.x, sp.second.z);
			sp.second.x*=-1;
		}
		for(auto& sp : _flist){
			for(auto& sp2 : sp.second.edges){
				swap(sp2.second.v1.x, sp2.second.v1.z);
				sp2.second.v1.x*=-1;
				swap(sp2.second.v2.x, sp2.second.v2.z);
				sp2.second.v2.x*=-1;		
			}
		}
	}
	_overlappingEdges(_elist, _vlist);
	_intersectingEdges(_elist, _vlist);
	_dashedLines(_elist, _flist);

	Projection ortho;
	ortho.name = view;
	for(auto& sp : _elist){
		edge2D e2(sp.second);
		ortho.elist.insert(pair<string, edge2D> (sp.first, e2));
	}

	for(auto& sp : _vlist){
		vert2D v2(sp.second);
		ortho.vlist.insert(pair<string, vert2D> (sp.first, v2));
	}

	return ortho;
}

void rotate_point(vertex &v, float R[3][3]){
	vertex res;
	res.x= R[0][0]*v.x + R[0][1]*v.y + R[0][2]*v.z;
	res.x= R[1][0]*v.x + R[1][1]*v.y + R[1][2]*v.z;
	res.x= R[2][0]*v.x + R[2][1]*v.y + R[2][2]*v.z; 
	v = res;
}

void Object3D::rotate(float alpha, float beta, float gamma){
	float Rx[3][3] = {{1, 0, 0},{0, cos(alpha), sin(alpha)},{0, -sin(alpha), cos(alpha)}};
	float Ry[3][3] = {{cos(beta), 0, -sin(beta)},{0, 1, 0},{sin(beta), 0, cos(beta)}};
	float Rz[3][3] = {{cos(gamma), sin(gamma), 0},{-sin(gamma), cos(gamma), 0}, {0, 0, 1}};
	
	for(auto& sp : vlist){
		rotate_point(sp.second, Rx);
		rotate_point(sp.second, Ry);
		rotate_point(sp.second, Rz);
	}
	for(auto& sp : elist){
		rotate_point(sp.second.v1, Rx);
		rotate_point(sp.second.v1, Ry);
		rotate_point(sp.second.v1, Rz);

		rotate_point(sp.second.v2, Rx);
		rotate_point(sp.second.v2, Ry);
		rotate_point(sp.second.v2, Rz);
	}
	for(auto& sp : flist){

		for(auto& sp2 : sp.second.edges){
			rotate_point(sp2.second.v1, Rx);
			rotate_point(sp2.second.v1, Ry);
			rotate_point(sp2.second.v1, Rz);

			rotate_point(sp2.second.v2, Rx);
			rotate_point(sp2.second.v2, Ry);
			rotate_point(sp2.second.v2, Rz);
		}
	}

}

inline void shift_point(vertex &v, vertex v0){
	v.x-=v0.x;
	v.y-=v0.y;
	v.z-=v0.z;
}

void Object3D::shift(float x0, float y0, float z0){
	vertex v0(x0, y0, z0);

	for(auto& sp : vlist){
		shift_point(sp.second, v0);
	}
	for(auto& sp : elist){
		shift_point(sp.second.v1, v0);
		shift_point(sp.second.v2, v0);
	}
	for(auto& sp : flist){

		for(auto& sp2 : sp.second.edges){
			shift_point(sp2.second.v1, v0);
			shift_point(sp2.second.v2, v0);
		}
	}
}