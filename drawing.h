/*! \file drawing.h
	\brief Contains Projection and Object3D class definitions

	This is the interface to the library developed for the CAD application which implements all the required algorithms.
	Libraries used:
		- stl::list
		- GLEW: OpenGL Extension Wrangler Library
		- GLUT: OpenGL Utility Toolkit
*/

#include <list>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace::std

//! structure for a 3D vertex
struct vertex
{
	float x;
	float y;
	float z;
}

//! A 2D vertex
/*!
@see vertex
*/
struct vert2D
{
	float x;
	float y;
}

//! structure for an edge
/*!
Edge is represented by tuple of vertices. It also stores the information of visibility of an edge when required
*/
struct edge
{
	vertex v1;
	vertex v2;
	//! bool variable denoting the visibility
	bool visi;
}

//! 2D edge
/*!
@see edge
*/
struct edge2D
{
	vert2D v1;
	vert2D v2;
}

//! structure for polygon face of a 3D object
/*!
A polygon face is represented by a list of edges. The equation of plane of the face is represented by the A,B,C & D parameters 
*/
struct face
{
	//! parameters for equation of plane
	float A, B, C, D;
	//! list of edges
	list <edge> edges;
	//! function to compute the equation of the plane from the list of edges.
	void compParam();
}

//! Class for representing projection
/*!
A projection is given by a list of 2D edges and a list of 2D vertices
*/
class Projection
{
public:
	list <edge2D> elist;
	list <vert2D> vlist;

	//! function to display the projection in a window. 
	void display();
}


//! Class for representing 3D objects
/*!
A 3D object is represented by a list of faces, edges and vertices. 
*/
class Object3D
{
protected:
	// Methods for 3D reconstruction
	void _wireframe();
	void _planarGraph();
	void _hiddenEdge();
	void _faceLoops();
	void _bodyLoops();

	// Methods for Orthographic view generation
	void _overlappingEdges();
	void _intersectingEdges();
	void _dashedLines();
	
public:
	list <face> flist;
	list <edge> elist;
	list <vertex> vlist;
	//! To compute Orthographic projections
	/*!
	Function which computes and returns an Orthographic projection of the object
	@param view a char* denoting the view of the projecion. It can take values - 'front', 'top', 'side'
	@return Object of Class Projection
	*/
	Projection projectToFront(char* view);

	//! Initialize the 3D object using 3 Orthographic projections
	/*!
	This function uses 3 Orthographic projection to reconstruct the 3D object from them and initialize itself
	@param FV the front Orthographic projection
	@param TV the top Orthographic projection
	@param SV the side view Orthographic projection
	@param rightside boolean value telling which side view is taken, default true
	@param righthand boolean value for right/left hand coordinate system to be followed, default true
	*/
	void create(Projection FV, Projection TV, Projection SV, bool rightside = true ,bool righthand = true);

	
	//! Rotation of the 3D object with respect to the given coordinate axes
	/*!
	@param alpha Angular displacement about the x axis
	@param beta Angular displacement about the y axis
	@param gamma Angular displacement about the z axis
	*/
	void rotate(float alpha, float beta, float gamma);
	
	//! shifting of origin of the 3D coordinate axes
	/*!
	@param x0 Offset in x direction
	@param y0 Offset in y direction
	@param z0 Offset in z direction
	*/
	void shift(float x0, float y0, float z0); 

	//! Method to render image of the object
	void display();
}

