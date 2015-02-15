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