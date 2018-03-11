/*! \file drawing.cpp
	\brief Implementation of the algorithms

	This file contains the implementation of all the required algorithms for the CAD application.
*/
#include <map>
#include <tuple>
#include <string>
#include <list>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <gtk/gtk.h>
#include <jsoncpp/json/json.h>
#include <pngwriter.h>
#include <cmath.h>
#include <limits>
#include "drawing.h"

using namespace::std;

void initGL(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

vector <float> cross_prod(float a[3], float b[3]){
	vector <float> res = {0.0, 0.0, 0.0};
	// return cross product of *a* and *b*
	res[0] = a[1]*b[2] - b[1]*a[2];
	res[1] = -a[0]*b[2] + b[0]*a[2];
	res[2] = a[0]*b[1] - a[1]*b[0];

	return res;
}

void save_png(char* filename){
	int width = glutGet(GLUT_SCREEN_WIDTH);
    int height = glutGet( GLUT_SCREEN_HEIGHT);
    int npixels = width*height;
	GLfloat* OpenGLimage = new GLfloat[npixels];
	glReadPixels(0.0, 0.0, width, height, GL_RGB, GL_FLOAT, OpenGLimage);
	pngwriter PNG(width, height, 1.0, fileName);
	size_t x = 1;   // start the top and leftmost point of the window
	size_t y = 1;
	double R, G, B;
	for(size_t i =0; i<npixels; i+=3){
		PNG.plot(x, y, pixels[i], pixels[i+1], pixels[i+2]);
		x++;
		if(x>width) {x=1; y++;}
	}
	PNG.close();
}

void  face::compParam(){
	float x[3], y[3];
	vector <float> t(3);

	map< string, edge*>::iterator it = face::edges.begin();
	x[0] = (it->second->v1.x - it->second->v2.x) ; x[1] = (it->second->v1.y - it->second->v2.y) ; x[2]= (it->second->v1.z - it->second->v2.z);

	bool col = true;
	while(col && it!=face::edge.end()){
		advance(it,1);
		y[0] = (it->second->v1.x - it->second->v2.x) ; y[1] = (it->second->v1.y - it->second->v2.y) ; y[2] = (it->second->v1.z - it->second->v2.z);
		t = cross_prod(x, y)
		if(t[1]==t[2] && t[2]==t[3] && t[3]==0)
			continue;
		col = false;
		}
	if(col)
		throw std::invalid_argument("problem in face");
	else{
		face::A = t[1]; face::B = t[1]; face::C = t[2];
		face::D = A*it->second->v1.x + B*it->second->v1.y + C*it->second->v1.z;
	}

}

void Projection::display(){
	glClear(GL_COLOR_BUFFER_BIT);

	for(const auto& ed : Projection::elist){
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


	glFlush();
}

void swap(float &a, float &b){
	float t = a;
	a = b;
	b = t;
}

tuple <float, float> Object3D::_intersect_ratiois(edge e1, edge e2){
	float u, t, num, den;
	den = (e2.v2.x - e2.v1.x)*(e1.v2.y - e1.v1.y) - (e2.v2.y - e2.v1.y)*(e1.v2.x - e1.v1.x);
	if(den==0.0)
		return make_tuple(999.0, 999.0);
	num = (e2.v2.y - e2.v1.y)*(e1.v1.x - e2.v1.x) - (e2.v2.x - e2.v1.x)*(e1.v1.y - e2.v1.y);  
	u = num/den;
	den = (e1.v2.x - e1.v1.x)*(e2.v2.y - e2.v1.y) - (e1.v2.y - e1.v1.y)*(e2.v2.x - e2.v1.x); 
	if(den==0.0)
		return make_tuple(99.0, 999.0);
	num = (e1.v2.y - e1.v1.y)*(e2.v1.x - e1.v1.x) - (e1.v2.x - e1.v1.x)*(e2.v1.y - e1.v1.y);
	t = num/den;

	return make_tuple(u, t);
}

float _point_on_segment(vertex v1, vertex v2, vertex v){
	if(v1.x == v2.x && v1.y == v2.y)
		throw "invalid line segment";
	float m = v1.x == v2.x? (v.y-v1.y)/(v2.y-v1.y) :(v.x-v1.x)/(v2.x-v1.x);
	return m;
}

void Object3D::_overlappingEdges(map <string, edge> &els, map <string, vertex> &vls){
	map <string, edge>::iterator eit1 = els.begin();
	map <string, edge>::iterator eit2 = next(eit1,1);

	while(eit1 != els.end()){
		while(eit2 != _els.end()){
			edge e1 = eit1->second, e2 = eit2->second;
			float m1, m2, m_1, m_2;
			m1 = e1.v1.x == e1.v2.x ? numeric_limits::infinity() : (e1.v1.y - e1.v2.y)/(e1.v1.x - e1.v2.x);
			m1 = e2.v1.x == e2.v2.x ? numeric_limits::infinity() : (e2.v1.y - e2.v2.y)/(e2.v1.x - e2.v2.x);
			if(m1==m2 && ((e1.v1.y - m1*e1.v1.x) == (e2.v1.y - m2*e2.v1.x))){
				m_1 = _point_on_segment(e1.v1, e1.v2, e2.v1);
				m_2 = _point_on_segment(e1.v1, e1.v2, e2.v2);
				
 				if(m_1=<1.0 && m1>=0.0 && m_2=<1.0 && m2>=0.0){
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
	 						if(m_1<m_2)
	 							{edge e = edge(e2.v2, e1.v2); eit1->second.v2 = e2.v1;}
	 						else
	 							{edge e = edge(e2.v1, e1.v2); eit1->second.v2 = e2.v2;}
							els.insert(pair<string, edge>("virtual", e));
						}	
 					}
 				}
 				if((m_1>=1.0 && m2=<0.0) || (m_2>=1.0 && m1=<0.0)){
 					if(e2.v1.z<= e1.v1.z){
 						if(m_1==0.0 && m_2!=1.0){
 							eit2->second.v1 = e1.v2;
 						}else if(m_1==1.0 && m_2!=0.0){
 							eit2->second.v1 = e1.v1;
 						}else if(m_2==0.0 && m_1!=1.0){
 							eit2->second.v2 = e1.v2;
 						}else if(m_2==1.0 && m_1!=0.0){
 							eit2->second.v2 = e1.v1;
 						}else{
	 						if(m_1<m_2)
	 							{edge e = edge(e1.v2, e2.v2); eit2->second.v2 = e1.v1;}
	 						else
	 							{edge e = edge(e1.v1, e2.v2); eit2->second.v2 = e1.v2;}
							els.insert(pair<string, edge>("virtual", e));
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
		while(eit2 != _els.end()){
			edge e1 = eit1->second, e2 = eit2->second;
			tuple <float, float> rs= _intersect_ratiois(eit1->second, eit2->second);
			if(rs.first>0 && rs.first<1 && rs.second>0 && rs.second<1){
				vertex vi;
				vi.x = e1.v1.x + rs.first*(e1.v2.x - e1.v1.x);
				vi.y = e1.v1.y + rs.first*(e1.v2.y - e1.v1.y); 
				{
					vi.z = e1.v1.z + rs.first*(e1.v2.z - e1.v1.z);
					edge e = edge(vi, e1.v2);
					els.insert(pair<string, edge>("virtual", e));
					eit1->second.v2 = vi;
					vls.insert(pair<string, vertex>("virtual", vi));
				}
				{
					vi.z = e2.v1.z + rs.second*(e2.v2.z - e2.v1.z);
					edge e = edge(vi, e2.v2);
					els.insert(pair<string, edge>("virtual", e));
					eit2->second.v2 = vi;
					vls.insert(pair<string, vertex>("virtual", vi));	
				}
			}
			advance(eit2, 1);
		}
		advance(eit1, 1);
		eit2 = next(eit1, 1);
	}
}

void Object3D::_dashedLines(map <string, edge> &els){

}


Projection Object3D::projectTo2D(char* view){
	map <string, face> _flist = flist;
	map <string, edge> _elist = elist;
	map <string, vertex> _vlist = vlist;
	
	if(view=="top"){
		for(const auto& sp : _elist){
			swap(sp.second.v1.y, sp.second.v1.z);
			sp.second.v1.y*=-1;
			swap(sp.second.v2.y, sp.second.v2.z);
			sp.second.v2.y*=-1;
		} 
		for(const auto& sp : _vlist){
			swap(sp.second.y, sp.second.z);
			sp.second.y*=-1;
		}
	}

	if(view=="side"){
		for(const auto& sp : _elist){
			swap(sp.second.v1.x, sp.second.v1.z);
			sp.second.v1.x*=-1;
			swap(sp.second.v2.x, sp.second.v2.z);
			sp.second.v2.x*=-1;
		}
		for(const auto& sp : _vlist){
			swap(sp.second.x, sp.second.z);
			sp.second.x*=-1;
		}
	}
	_overlappingEdges(_elist, _vlist);
	_intersectingEdges(_elist, _vlist);
	_dashedLines(_elist, _flist);

}