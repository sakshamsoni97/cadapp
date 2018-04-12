# CADapp

This short application was developed in C/C++ as an assignment for the course COP290 under Prof. Subhashis Banerjee during Spring 2017-18. This application provides a method to generate Orthographic projections of a polyhedral object from its 3D specification and to reconstruct a 3D object from its orthographic projections. The application checks for object validity and has a simple GUI to interact and input projection/object specification and generate and render the projection/3D object.

## Libraries Used
* GLUT: OpenGL Utility Toolkit - To render the drawings
* GTK+ - To create the GUI
* stl: Standard Template Library - For *map* and *tuple* data structures
* cereal: A C++11 library to serialize/deserialize objects and store them in binary files

## Compilation Instructions
Change the directory to *src* using the command shown below

```
cd src
```
Make the program using the *make* command

```
make
```
Run using the *CADAPP* binary generated

```
./CADAPP
```
## Prerequisites

## Authors

* **Ishan Tewari** - *Collaborator* - [IshanTewari](https://github.com/IshanTewari)


* **Saksham Soni** - *Collaborator* - [sakshamsoni97](https://github.com/sakshamsoni97)

See also the list of [contributors](https://github.com/sakshamsoni97/cadapp/contributors) who participated in this project.
