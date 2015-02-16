/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Sphere.h


****************************************************************************/
#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "FaceList.h"

class Sphere : public Shape {

public:
	Sphere();
	~Sphere() {};

	void draw();

	void drawNormal();

	void makeFaceList();

protected:

	FaceList *faceList;

};
#endif