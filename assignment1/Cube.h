/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cube.h


****************************************************************************/

#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include "CubeList.h"

class Cube : public Shape {
public:
	Cube();
	~Cube() {};

	void draw();

	void drawNormal();

protected:

	CubeList *cubelist;

	void drawFace(float xc, float yc, float zc, float dirx, float diry, float dirz, float norm);


};

#endif
