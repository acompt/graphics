/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: VertexList.cpp


****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "VertexList.h"
#include "Algebra.h"

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5


VertexList::VertexList()
{
	theList = NULL;
	//count = 0;

}

void VertexList::makeList(float x_seg, float y_seg)
{

	int vertices, sx, sy, sz;

	sx = x_seg + 1;
	sy = y_seg + 1;


	vertices = (((sx * sy) + (sx * 2) + (2))*100);

	if (theList != NULL) {
		delete[] theList;
	}
	theList = new Vertex[vertices];
	count = 0;

}

void VertexList::addVertex(float x, float y, float z, float nx, float ny, float nz)
{

	Vertex vertex;

	vertex.x = x;
	vertex.y = y;
	vertex.z = z;
	vertex.nx = nx;
	vertex.ny = ny;
	vertex.nz = nz;
	vertex.count = 1.0f;
	vertex.status = false;

	theList[count] = vertex;

	count++;

}

Vertex VertexList::getVertex(int i)
{
	return theList[i];
}

int VertexList::getLength()
{
	return count;
}

bool VertexList::isEqual(Vertex v1, Vertex v2) 
{
	//printf("%f\n", v1.x);
	//printf("%f\n", v2.x);
	(IN_RANGE(v1.x, v2.x)) && (IN_RANGE(v1.y, v2.y)) && (IN_RANGE(v1.z, v2.z));
}

void VertexList::setVertex(float x, float y, float z, float nx, float ny, float nz, int i, float c) 
{
	theList[i].x = x;
	theList[i].y = y;
	theList[i].z = z;
	theList[i].nx = nx;
	theList[i].ny = ny;
	theList[i].nz = nz;
	theList[i].count = c;
}






