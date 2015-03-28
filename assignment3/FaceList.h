/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: FaceList.h


****************************************************************************/
#ifndef FACELIST_H
#define FACELIST_H
#include "VertexList.h"

// struct Vertex {
// 	float x,y,z;
// 	float status;
// 	float nx, ny, nz;
// };

struct Face {
	Vertex a,b,c;
	float nx, ny, nz;
};

class FaceList {

public:
	FaceList();
	~FaceList() {};

	void makeList(float x_seg, float y_seg, float dups);

	void addFace(Vertex v1, Vertex v2, Vertex v3, float nx, float ny, float nz);

	Face getFace(int i);

	int getLength();

protected:

	Face* theList;

	int count;
};

#endif
