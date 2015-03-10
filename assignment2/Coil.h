/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Coil.h


****************************************************************************/

#ifndef Coil_H
#define Coil_H

#include "Shape.h"
#include "FaceList.h"
#include "VertexList.h"

class Coil : public Shape {

public:
	Coil();
	~Coil() {};

	void draw();

	void drawNormal();

	void makeFaceList();

	void makeVertexList();

protected:

	FaceList *faceList;
	VertexList *vertexList;

};
#endif