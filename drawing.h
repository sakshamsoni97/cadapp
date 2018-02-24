#include <list>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace::std

struct vertex
{
	float x;
	float y;
	float z;
}

struct edge
{
	vertex v1;
	vertex v2;
	bool visi;
}

struct face
{
	float A, B, C, D;
	list <edge> edges;
	void compParam();
}

class Projection
{
public:
	list <edge> elist;
	list <vertex> vlist;
	void display();
}

class Object3D
{

public:
	list <face> flist;
	list <edge> elist;
	list <vertex> vlist;
	
	Projection projectToFront();
	Projection projectToTop();
	Projection projectToSide();
	void rotate(float alpha, float beta, float gamma);
	void shift(float x0, float y0, float z0); 
	void display();
}