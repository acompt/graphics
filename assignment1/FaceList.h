#ifndef FACELIST_H
#define FACELIST_H

struct Vertex {
	float x,y,z;
	float status;
};

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