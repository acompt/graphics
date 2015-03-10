/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: CubeList.cpp


****************************************************************************/


#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "CubeList.h"

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5


CubeList::CubeList()
{
	theList = NULL;
	count = 0;

}

void CubeList::makeList(float x_seg, float y_seg)
{

	int totalVerts, sx, sy, sz;

	sx = x_seg + 1;
	sy = y_seg + 1;
	sz = x_seg + 1;


	totalVerts = 4 * ((sx * sy) + (sx * sz) + (sz * sy));

	if (theList != NULL) {
		delete[] theList;
	}
	theList = new Vert[totalVerts];
	count = 0;

}

void CubeList::addVert(float x, float y, float z, float isFirst, 
						float xnorm, float ynorm, float znorm)
{
	Vert vert;

	vert.vx = x;
	vert.vy = y;
	vert.vz = z;
	vert.isFirst = isFirst;

	vert.xnorm = xnorm;
	vert.ynorm = ynorm;
	vert.znorm = znorm;

	theList[count] = vert;

	count++;


}

Vert CubeList::getVert(int i)
{
	return theList[i];
}

int CubeList::getLength()
{
	return count;
}





