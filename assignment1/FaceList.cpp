/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: FaceList.cpp


****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "FaceList.h"

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5


/* *  Static Functions * */



/* * * * * * * * * * * * */


FaceList::FaceList()
{
	theList = NULL;
	count = 0;

}

void FaceList::makeList(float x_seg, float y_seg, float dups)
{

	int faces, sx, sy, sz;

	sx = x_seg + 1;
	sy = y_seg + 1;
	sz = x_seg + 1;


	faces = dups * ((sx * sy) + (sx * sz) + (sz * sy));

	if (theList != NULL) {
		delete[] theList;
	}
	theList = new Face[faces];
	count = 0;

}

void FaceList::addFace(Vertex v1, Vertex v2, Vertex v3, float nx, float ny, float nz)
{

	Face face;

	face.a = v1;
	face.b = v2;
	face.c = v3;
	face.nx = nx;
	face.ny = ny;
	face.nz = nz;

	theList[count] = face;

	count++;

}

Face FaceList::getFace(int i)
{
	return theList[i];
}

int FaceList::getLength()
{
	return count;
}