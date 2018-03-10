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
#include "drawing.h"

using namespace::std;

void initGL(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector <float> cross_prod(float a[3], float b[3]){
	vector <float> res = {0.0, 0.0, 0.0};
	// return cross product of *a* and *b*
	res[0] = a[1]*b[2] - b[1]*a[2];
	res[1] = -a[0]*b[2] + b[0]*a[2];
	res[2] = a[0]*b[1] - a[1]*b[0];

	return res;
}

void  face::compParam(){
	float x[3], y[3];
	vector <float> t(3);

	map< string, edge>::iterator it = face::edges.begin();
	x[0] = (it->second.v1.x - it->second.v2.x) ; x[1] = (it->second.v1.y - it->second.v2.y) ; x[2]= (it->second.v1.z - it->second.v2.z);

	bool col = true;
	while(col && it<face::edge.end()){
		advance(it,1);
		y[0] = (it->second.v1.x - it->second.v2.x) ; y[1] = (it->second.v1.y - it->second.v2.y) ; y[2] = (it->second.v1.z - it->second.v2.z);
		t = cross_prod(x, y)
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