/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: VertexList.h


****************************************************************************/
#ifndef VERTEXLIST_H
#define VERTEXLIST_H

struct Vertex {
	float x,y,z;
	float status;
	float nx, ny, nz;
};

class VertexList {

public:
	VertexList();
	~VertexList() {};

	void makeList(float x_seg, float y_seg);

	void addVertex(float x, float y, float z, float nx, float ny, float nz);

	Vertex getVertex(int i);

	int getLength();

	bool isEqual(Vertex v1, Vertex v2);

protected:

	Vertex* theList;

	int count;
};

#endif