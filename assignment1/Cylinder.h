/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cylinder.h


****************************************************************************/

#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "FaceList.h"

class Cylinder : public Shape {

public:
	Cylinder();
	~Cylinder() {};

	void draw();

	void drawNormal();

	void makeFaceList();

	void makeLid(float theta, float initX, float initY, float initZ, 
				float addTheta, float radius);

	void makeSides(float theta, float initX, float initY, float initZ, 
				float addTheta, float radius, float stackHeight);
protected:

	FaceList *faceList;

};
#endif