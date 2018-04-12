/*! \file drawing.h
	\brief Contains Projection and Object3D class definitions

	This is the interface to the library developed for the CAD application which implements all the required algorithms.
	Libraries used:
		- stl::list
		- GLEW: OpenGL Extension Wrangler Library
		- GLUT: OpenGL Utility Toolkit
		- Cereal: Library to serialize objects and store them in binary format

		OpenGL libraries are used to render orthographic views and the 3D object
	.
*/
#ifndef __DRAWING_H
#define __DRAWING_H

#include <map>
#include <utility>
#include <string>
#include <list>
#include <vector>
#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

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

void initGL();

void reshape(GLsizei width, GLsizei height);

void initGL3D();

void reshape3D(GLsizei width, GLsizei height);

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
	vertex();
	vertex(float _x, float _y, float _z);
	vertex(const vertex&) = default;
	bool operator==(const vertex &v);
	vertex operator+(const vertex &v);
	vertex operator-(const vertex &v);
	vertex operator*(const float &f);

	template<class Archive>
	void serialize(Archive & ar){
		ar( x, y, z);
	}
};

//! A 2D vertex
/*!
@see vertex
*/
struct vert2D
{
	float x;
	float y;
	vert2D();
	vert2D(float _x, float _y);
	vert2D(const vertex &v);
	vert2D(const vert2D &v);

	template<class Archive>
	void serialize(Archive & ar){
		ar( x, y);
	}
};

//! structure for an edge
/*!
Edge is represented by pair of vertices. It also stores the information of visibility of an edge when required
*/
struct edge
{
	vertex v1;
	vertex v2;
	bool visi; 	/*!< bool variable denoting the visibility */
	edge();
	edge(const edge&) = default;
	edge(vertex a, vertex b, bool v = true);

	template<class Archive>
	void serialize(Archive & ar){
		ar( v1, v2, visi);
	}
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
	edge2D(const edge2D&) = default;
	edge2D(vert2D a, vert2D b, bool v = true);
	edge2D(const edge e);

	template<class Archive>
	void serialize(Archive & ar){
		ar( v1, v2, visi);
	}
};

//! structure for polygon face of a 3D object
/*!
A polygon face is represented by a list of edges. The equation of plane of the face is represented by the A,B,C & D parameters
*/
struct face
{
	float A, B, C, D;
	map <string, edge> edges; 	/*!< list of edges */
	map <string, vertex> verts; /*!< list of vertices */
	/*! function to compute the equation of the plane from the list of edges. */

	face();
	face(const face&) = default;
	void compParam();

	template<class Archive>
	void serialize(Archive & ar){
		ar( A, B, C, D, edges, verts);
	}
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

	Projection();
	/*! function to display the projection in a window. */
	static void display();
	/*!Function which helps take projection specification as input */
	void getProjection();

	template<class Archive>
	void serialize(Archive & ar){
		ar( name, elist, vlist);
	}
};


//! Class for representing 3D objects
/*!
A 3D object is represented by a list of faces, edges and vertices.
*/
class Object3D
{
protected:
	pair <float, float> _intersect_ratiois(edge e1, edge e2);
	// Methods for 3D reconstruction
	/*!
	Function that constructs the wireframe of the object i.e the edges outlning the 3D object
	@param FV denotes the input front orthographic projection
	@param TV denotes the input top orthographic projection
	@param SV denoted the input side orthographic projection
	@param rightside boolean value indicating wether right side view is taken, default value is  true
	@param righthand boolean value for right/left hand coordinate system to be followed, default value is true
	@return pair containing list of possible edges and vertices
	*/
	pair < map <string,edge>,map <string,vertex> > _wireframe(Projection FV, Projection TV, Projection SV, bool rightside = true ,bool righthand = true);
	/*!

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
	void _dashedLines(map <string, edge> &els, map <string, face> &fls);
	/*!
	Function that generates the projection after processing all the edges for overlap and intersection.
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	@return The corresponding orthographic view as Projection object
	*/

public:

	map <string, face> flist;
	map <string, edge> elist;
	map <string, vertex> vlist;

	Object3D();
	//! To compute Orthographic projections
	/*!
	Function which computes and returns an Orthographic projection of the object
	@param view a char* denoting the view of the projecion. It can take values - "front", "top", "rside", "lside"
	@return Object of Class Projection
	*/
	Projection projectTo2D(string view);

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


	//! Rotation of the 3D object with respect to the given coordinate axes. All angles are in degrees.
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

	static void display();

	static void display_wireframe();

	template<class Archive>
	void serialize( Archive & ar ){
		ar( flist, elist, vlist);
	}
};

extern Object3D default_ob; 	//Global 3D Object variable
extern Projection default_fv; //Global FV Projection variable
extern Projection default_tv; //Global TV Projection variable
extern Projection default_sv; //Global SV Projection variable


#endif
