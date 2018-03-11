/*! \file drawing.h
	\brief Contains Projection and Object3D class definitions

	This is the interface to the library developed for the CAD application which implements all the required algorithms.
	Libraries used:
		- stl::list
		- GLEW: OpenGL Extension Wrangler Library
		- GLUT: OpenGL Utility Toolkit
		- JsonCpp: Library to parse JSON objects and save objects as JSON

		OpenGL libraries are used to render orthographic views and the 3D object
	.
*/
#ifndef __DRAWING_H
#define __DRAWING_H

#include <map>
#include <tuple>
#include <string>
#include <list>
#include <vector>

using namespace::std;

void initGL();

void reshape(GLsizei width, GLsizei height);

vector <float> cross_prod(float [3], float [3]);

//! A 3D vertex
/*!
Structure for a point in 3 dimension.
*/
struct vertex
{
	float x;
	float y;
	float z;
};

//! A 2D vertex
/*!
@see vertex
*/
struct vert2D
{
	float x;
	float y;
	vert2D(float _x, float _y);
	vert2D(vertex v);
};

//! structure for an edge
/*!
Edge is represented by tuple of vertices. It also stores the information of visibility of an edge when required
*/
struct edge
{
	vertex v1;
	vertex v2;
	bool visi; 	/*!< bool variable denoting the visibility */
	
	edge();
	edge(vertex a, vertex b, bool v = true);
};

//! 2D edge
/*!
@see edge
*/
struct edge2D
{
	vert2D v1;
	vert2D v2;
	bool visi;		/*!< bool variable denoting the visibility */
	
	edge2D();
	edge2D(vert2D a, vert2D b, bool v = true);
	edge2D(edge e);
};

//! structure for polygon face of a 3D object
/*!
A polygon face is represented by a list of edges. The equation of plane of the face is represented by the A,B,C & D parameters
*/
struct face
{
	float A, B, C, D;
	map <string, edge> edges; 	/*!< list of edges */
	/*! function to compute the equation of the plane from the list of edges. */
	void compParam();
};

//! Class for representing projection
/*!
A projection is given by a list of 2D edges and a list of 2D vertices
*/
class Projection
{
public:
	string name;
	map <string, edge2D> elist;
	map <string, vert2D> vlist;

	/*! function to display the projection in a window. */
	void display();
	/*!Function which helps take projection specification as input */
	void getProjection();
};


//! Class for representing 3D objects
/*!
A 3D object is represented by a list of faces, edges and vertices.
*/
class Object3D
{
protected:
	tuple <float, float> _intersect_ratiois(edge e1, edge e2);
	// Methods for 3D reconstruction
	/*!
	Function that constructs the wireframe of the object i.e the edges outlning the 3D object
	@param FV denotes the input front orthographic projection
	@param TV denotes the input top orthographic projection
	@param SV denoted the input side orthographic projection
	@param rightside boolean value indicating wether right side view is taken, default value is  true
	@param righthand boolean value for right/left hand coordinate system to be followed, default value is true
	@return Tuple containing list of possible edges and vertices
	*/
	tuple <map <string,edge>,map <string,vertex>> _wireframe(Projection FV, Projection TV, Projection SV, bool rightside = true ,bool righthand = true);
	/*!
	Function that constructs Planar Graphs i.e. sets of valid coplanar edges
	@param p_edges indicates the set of all possible edges constructed in the wireframe Function
	@param p_vertex indicates the set of vertices possible, as obtained from the wireframe reconstruction algorithm
	@return List of all possible planar graphs
	*/
	list <map <string,edge>> _planarGraph(map <string,edge> p_edges,map <string,vertex> p_vertex);
	/*!
	Function that checks for visibilty of edges, as specified in views and eliminates, edges and planar graphs based on this
	@param  planarGraphs is the list of all possible planar graphs as obtained from the planar graph construction function
	@return Pruned list of possible planar graphs
	*/
	list <map <string,edge>> _hiddenEdge(list <map<string,edge>> planarGraphs);
	/*!
	Function that checks and creates possible face loops with valid normal direction, from the set of planar grpahs
	@param  planarGraphs is the pruned list of planar graphs after checking for hidden edge visibility
	@return List of all possible face loops
	*/
	list <map <string,face>> _faceLoops(list <map<string,edge>> planarGraphs);
	/*!
	Function that checks and creates possible body loops, from the set of face loops
	@param  faceLoops is the set of all possible face loops
	@return List of all possible body loops
	*/
	list<map <string,face>> _bodyLoops(map <string,face> faceLoops);
	/*!
	Function that constructs a 3D object by combining body loop objects and also checks object validity
	*/
	void _constructObject(list< map<string,face> >);

	//! Protected Static Methods used to generate a 3D Object from its orthographic projections
	/*!
	Pathological Edge and Vertex Removal (PEVR) Method
	@param edges is the set of all edges that have to be evaluated
	@param vertex is the set of all vertices that have to be evaluated
	@return Tuple containing the set of vertices and edges after performing PEVR
	*/
	static tuple<map <string,edge>,map <string,vertex>> _PEVR(map <string,edge> edges,map <string,vertex> vertex);
	/*!
	Redundant Edge Removal (RER) Method
	@param edges is the set of all edges that have to be evaluated
	@param vertex is the set of all vertices that have to be evaluated
	@return Tuple containing the set of vertices and edges after performing RER
	*/
	static tuple<map <string,edge>,map <string,vertex>> _RER(map <string,edge> edges,map <string,vertex> vertex);

	// Methods for Orthographic view generation
	/*!
	Function to handle overlapping edges while generating Orthographic projection.
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	*/
	void _overlappingEdges(map <string, edge> &els, map <string, vertex> &vls);
	/*!
	Function to handle intersecting edges while generating Orthographic projection.
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	*/
	void _intersectingEdges(map <string, edge> &els, map <string, vertex> &vls);
	/*!
	Function to mark the hidden lines as dashed in the Orthographic projection
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	*/
	void _dashedLines(map <string, edge> &els);
	/*!
	Function that generates the projection after processing all the edges for overlap and intersection.
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	@return The corresponding orthographic view as Projection object
	*/
	Projection _flatten(char* view);

public:
	map <string, face> flist;
	map <string, edge> elist;
	map <string, vertex> vlist;
	//! To compute Orthographic projections
	/*!
	Function which computes and returns an Orthographic projection of the object
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	@return Object of Class Projection
	*/
	Projection projectTo2D(char* view);

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
	/*! */
	void display();
};


#endif
