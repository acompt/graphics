/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: CubeList.h


****************************************************************************/

#ifndef CUBELIST_H
#define CUBELIST_H

#include "Cube.h"

struct Vert {
	float vx;
	float vy;
	float vz;
	float isFirst;
	float xnorm;
	float ynorm;
	float znorm;
};

class CubeList {

public:
	CubeList();
	~CubeList() {};

	void makeList(float x_seg, float y_seg);

	void addVert(float x, float y, float z, float isFirst, float xnorm, float ynorm, float znorm);

	Vert getVert(int i);

	int getLength();

protected:

	Vert* theList;

	int count;
};

#endif
