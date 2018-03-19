/*! \file drawing.cpp
	\brief Implementation of the algorithms

	This file contains the implementation of all the required algorithms for the CAD application.
*/
#include <map>
#include <utility>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <gtk/gtk.h>
#include <math.h>
#include <limits>
#include "drawing.h"

using namespace::std;

Object3D default_ob;
Projection default_pr;

void initGL(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void initGL3D() {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
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
   //gluPerspective(45.0f, aspect, 0.1f, 100.0f);

   // Set up orthographic projection view [NEW]
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      glOrtho(-3.0 * aspect, 3.0 * aspect, -3.0, 3.0, 0.1, 100);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     glOrtho(-3.0, 3.0, -3.0 / aspect, 3.0 / aspect, 0.1, 100);
   }
}

vector <float> cross_prod(float a[3], float b[3]){
	vector <float> res; //= {0.0, 0.0, 0.0};
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

bool vertex::operator==(const vertex &v){
	return (v.x==x && v.y==y && v.z==z);
}

vertex vertex::operator+(const vertex &v){
	vertex res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

vertex vertex::operator-(const vertex &v){
	vertex res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

vertex vertex::operator*(const float &f){
	vertex res;
	res.x = x*f;
	res.y = y*f;
	res.z = z*f;
	return res;
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
	glBegin(GL_LINES);
	glLineWidth(5);
	glEnable(GL_LINE_SMOOTH);
	for(const auto& ed : default_pr.elist){
		if(!ed.second.visi){
			glEnable(GL_LINE_STIPPLE);
			glLineWidth(2);
			glLineStipple(1, 0x00FF);
			glColor3f(0.0f, 0.0f, 0.8f);
		}
		else{
			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(5);
		}
		glVertex2f((GLfloat) ed.second.v1.x, (GLfloat) ed.second.v1.y);
		glVertex2f((GLfloat) ed.second.v2.x, (GLfloat) ed.second.v2.y);
		if(!ed.second.visi)
			glDisable(GL_LINE_STIPPLE);
		
	}
	glDisable(GL_LINE_SMOOTH);
	glEnd();

	glFlush();
}

void Projection::getProjection(){

}

void Object3D::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.5f, 0.0f, -5.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	vertex *vt;

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   	for(auto& sp : default_ob.flist){
   		glBegin(GL_LINE_LOOP);
		glLineWidth(3);
		glEnable(GL_LINE_SMOOTH);
		glColor3f(0.0f, 0.0f, 0.0f);
		vt = NULL;
		for(auto& ed : sp.second.verts){
			//cout<<ed.first<<endl;
			glVertex3f((GLfloat) ed.second.x, (GLfloat) ed.second.y, (GLfloat) ed.second.z);
		}
		glEnd();
	}
   
   	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   	for(auto& sp : default_ob.flist){
   		glBegin(GL_POLYGON);
		glColor4f(0.2f, 0.2f, 0.4f, 0.4f);
		vt = NULL;
		for(auto& ed : sp.second.verts){
			//cout<<ed.first<<endl;
			glVertex3f((GLfloat) ed.second.x, (GLfloat) ed.second.y, (GLfloat) ed.second.z);
		}
		glEnd();
	}

   	glutSwapBuffers();
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
	map <string, edge>::iterator eit2 = els.begin();

	while(eit2!=els.end()){
		if(eit2->second.v1.x == eit2->second.v2.x && eit2->second.v1.y == eit2->second.v2.y){
			map <string, edge>::iterator it = next(eit2,1);
 			els.erase(eit2);
 			eit2 = it;
 			continue;
		}
		advance(eit2, 1);
	}

	eit2 = next(eit1, 1);

	bool f;
	while(eit1 != els.end()){
		float m1, m2, m_1, m_2;
		edge e1 = eit1->second;
		m1 = e1.v1.x == e1.v2.x ? 1000000.0f : (e1.v1.y - e1.v2.y)/(e1.v1.x - e1.v2.x);
		//cout<<eit1->first<<" "<<m1<<endl;
		f =true;
		while(eit2 != els.end()){
			edge e2 = eit2->second;
			m2 = e2.v1.x == e2.v2.x ? 1000000.0f : (e2.v1.y - e2.v2.y)/(e2.v1.x - e2.v2.x);
			if(m1==m2){
				if(m1 == 1000000.0f){
					if(e1.v1.x != e2.v1.x)
					{advance(eit2, 1); continue;}
				}else if((e1.v1.y - m1*e1.v1.x) != (e2.v1.y - m2*e2.v1.x))
					{advance(eit2, 1); continue;}

				m_1 = _point_on_segment(e1.v1, e1.v2, e2.v1);
				m_2 = _point_on_segment(e1.v1, e1.v2, e2.v2);
				
 				if((m_1<=1.0 || m_1>=0.0) && (m_2<=1.0 || m_2>=0.0)){
 					if(e2.v1.z<= e1.v1.z){
 						map <string, edge>::iterator it = next(eit2,1);
			 			//cout<<"2"<<" "<<eit2->first<<" "<<m2<<endl; 
			 			els.erase(eit2);
			 			eit2 = it;
			 			continue;
 					}

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
 				if((m_1>=1.0 && m_2<=0.0) || (m_2>=1.0 && m_1<=0.0)){
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
 						map <string, edge>::iterator it = next(eit1,1);
 						//cout<<"3"<<eit2->first<<endl; 
 						els.erase(eit1);
 						eit1 = it; 
 						f = false;
 						break;
 					}
 				}

 				if(m_1<1.0 && m_1>0.0 && m_2>1.0){
 					if(e2.v1.z<=e1.v2.z)
 						eit2->second.v1 = e1.v2;
 					else
 						eit1->second.v2 = e2.v1;
 				}else if(m_1<1.0 && m_1>0.0 && m_2<0.0){
 					if(e2.v1.z<=e1.v1.z)
 						eit2->second.v1 = e1.v1;
 					else
 						eit1->second.v1 = e2.v1;
 				}else if(m_2<1.0 && m_2>0.0 && m_1<0.0){
 					if(e2.v2.z<=e1.v1.z)
 						eit2->second.v2 = e1.v1;
 					else
 						eit1->second.v1 = e2.v2;
 				}else if(m_2<1.0 && m_2>0.0 && m_1>1.0){
 					if(e2.v2.z<=e1.v2.z)
 						eit2->second.v2 = e1.v2;
 					else
 						eit1->second.v2 = e2.v2;
 				}

			}
			advance(eit2, 1);
		}
		if(f)
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
					
				vi.z = e1.v1.z + rs.first*(e1.v2.z - e1.v1.z);
				edge e = edge(vi, e1.v2);
				els.insert(pair<string, edge>(eit1->first+"_1", e));
				eit1->second.v2 = vi;
				vls.insert(pair<string, vertex>(eit1->first+"p", vi));
	
				vi.z = e2.v1.z + rs.second*(e2.v2.z - e2.v1.z);
				e = edge(vi, e2.v2);
				els.insert(pair<string, edge>(eit2->first+"_1", e));
				eit2->second.v2 = vi;
				vls.insert(pair<string, vertex>(eit2->first+"p", vi));	
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
	if(z_ < v.z - 0.01)
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

		if( abs((v.y - e2.v1.y)*(e2.v2.x - e2.v1.x)-(e2.v2.y - e2.v1.y)*(v.x-e2.v1.x))<0.01 )
			return false;

		float m = (v.y - e2.v1.y)/(e2.v2.y - e2.v1.y);
		
		if( (m*(e2.v2.x - e2.v1.x)+e2.v1.x) < v.x )
			continue;

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
		sp.second.visi = true;
		for(auto& sp1 : fls){
			vertex mid = e.v1*0.5 + e.v2*0.5; 
			if(_point_behind_face(mid, sp1.second))
				{sp.second.visi = false; break;}
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
			for(auto& sp2 : sp.second.verts){
				swap(sp2.second.y, sp2.second.z);
				sp2.second.y*=-1;
			}
		}
	}else if(view=="side"){
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
			for(auto& sp2 : sp.second.verts){
				swap(sp2.second.x, sp2.second.z);
				sp2.second.x*=-1;		
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
	res.y= R[1][0]*v.x + R[1][1]*v.y + R[1][2]*v.z;
	res.z= R[2][0]*v.x + R[2][1]*v.y + R[2][2]*v.z;
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
		for(auto& sp2 : sp.second.verts){
			rotate_point(sp2.second, Rx);
			rotate_point(sp2.second, Ry);
			rotate_point(sp2.second, Rz);

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
		for(auto& sp2 : sp.second.verts){
			shift_point(sp2.second, v0);
		}
	}
}

pair < map <string,edge>,map <string,vertex> > Object3D::_wireframe(Projection FV, Projection TV, Projection SV, bool rightside,bool righthand){
	cout<<"Entered Wireframe"<<endl;
	map <string,edge2D>::iterator iF = FV.elist.begin();
	map <string,edge> p_elist;
  map <string,vertex> p_vlist;

  int i = 0;

  for(auto& itF:FV.vlist){
		//itT = TV.vlist.begin();
		//cout<<"itF: "<<itF.second.x<<" "<<itF.second.y<<endl;
    for(auto& itT:TV.vlist){
			//cout<<"itF: "<<itF->second.x<<" "<<itF->second.y<<"itT: "<<itT->second.x<<" "<<itT->second.y<<endl;
			//itS = SV.vlist.begin();
      for(auto& itS:SV.vlist){
				//cout<<"itF: "<<itF.second.x<<" "<<itF.second.y<<" itT: "<<itT.second.x<<" "<<itT.second.y<<" itS: "<<itS.second.x<<" "<<itS.second.y<<endl;
        if(itF.second.x==itT.second.x&&itF.second.y==itS.second.y&&itT.second.y==itS.second.x){
          string str = "v";
					//cout<<"itF: "<<itF.second.x<<" "<<itF.second.y<<" itT: "<<itT.second.x<<" "<<itT.second.y<<" itS: "<<itS.second.x<<" "<<itS.second.y<<endl;
          p_vlist[str.append(to_string(i))] = vertex((itF.second.x + itT.second.x)/2,(itT.second.y + itS.second.x)/2,(itF.second.y + itS.second.y)/2);
					cout<<str<<" "<<(itF.second.x + itT.second.x)/2<<" "<<(itT.second.y + itS.second.x)/2<<" "<<(itF.second.y + itS.second.y)/2<<endl;
          i++;
        }
      }
    }
  }
  vlist = p_vlist;

  map <string,edge2D>::iterator iteF = FV.elist.begin();
  map <string,edge2D>::iterator iteT = TV.elist.begin();
  map <string,edge2D>::iterator iteS = SV.elist.begin();
  map <string,vertex>::iterator it1 = p_vlist.begin();
  map <string,vertex>::iterator it2;

  i=0;

  while(it1!=p_vlist.end()){
    //cout<<it1->second.x<<" "<<it1->second.y<<" "<<it1->second.z<<endl;
    it2 = ++it1;
    --it1;
    while(it2!=p_vlist.end()){
      int flag=0;
			for(auto& itF:FV.elist){
				if((it1->second.x==itF.second.v1.x && it2->second.x==itF.second.v2.x && it1->second.z==itF.second.v1.y && it2->second.z==itF.second.v2.y)||(it2->second.x==itF.second.v1.x && it1->second.x==itF.second.v2.x && it2->second.z==itF.second.v1.y && it1->second.z==itF.second.v2.y)){
          flag++;
					break;
        }
			}
			for(auto& itT:TV.elist){
				if((it1->second.x==itT.second.v1.x && it2->second.x==itT.second.v2.x && it1->second.y==itT.second.v1.y && it2->second.y==itT.second.v2.y)||(it2->second.x==itT.second.v1.x && it1->second.x==itT.second.v2.x && it2->second.y==itT.second.v1.y && it1->second.y==itT.second.v2.y)){
					flag++;
					break;
				}
			}
			for(auto& itS:SV.elist){
				if((it1->second.y==itS.second.v1.x && it2->second.y==itS.second.v2.x && it1->second.z==itS.second.v1.y && it2->second.z==itS.second.v2.y)||(it2->second.y==itS.second.v1.x && it1->second.y==itS.second.v2.x && it2->second.z==itS.second.v1.y && it1->second.z==itS.second.v2.y)){
					flag++;
					break;
				}
			}
			int cnt=0;
			if(it1->second.x==it2->second.x)
				cnt++;
			if(it1->second.y==it2->second.y)
				cnt++;
			if(it1->second.z==it2->second.z)
				cnt++;

				//cout<<"Edge Being Considered: "<<": ("<<it1->second.x<<","<<it1->second.y<<","<<it1->second.z<<") <---> ("<<it2->second.x<<","<<it2->second.y<<","<<it2->second.z<<") Flag: "<<flag<<" Count: "<<cnt<<endl;

			if((flag==2&&cnt==2)||flag==3) {
				string str = "e";
				p_elist[str.append(to_string(i))] = edge(it1->second,it2->second);
				cout<<str<<": ("<<it1->second.x<<","<<it1->second.y<<","<<it1->second.z<<") <---> ("<<it2->second.x<<","<<it2->second.y<<","<<it2->second.z<<")"<<endl;
				i++;
			}
      ++it2;
    }
    ++it1;
  }

  elist = p_elist;

  return make_pair(p_elist,p_vlist);
}

void Object3D::create(Projection FV, Projection TV, Projection SV, bool rightside,bool righthand){
	//cout<<"Entered Create"<<endl;
	//cout<<TV.elist["10_1011"].v2.x<<endl;
	/*map <string,edge2D>::iterator iteF = FV.elist.begin();
	while(iteF!=FV.elist.end()){
		cout<<iteF->second.v2.x<<endl;
		++iteF;
	}*/
  _wireframe(FV,TV,SV,rightside,righthand);
}
