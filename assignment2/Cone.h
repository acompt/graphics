/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cone.h


****************************************************************************/

#ifndef Cone_H
#define Cone_H

#include "Shape.h"
#include "FaceList.h"
#include "VertexList.h"

class Cone : public Shape {

public:
	Cone();
	~Cone() {};

	void draw();

	void drawNormal();

	void makeFaceList();

protected:

	FaceList *faceList;
	VertexList *vertexList;
};
#endif