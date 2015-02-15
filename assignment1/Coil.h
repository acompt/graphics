#ifndef Coil_H
#define Coil_H

#include "Shape.h"
#include "FaceList.h"

class Coil : public Shape {

public:
	Coil();
	~Coil() {};

	void draw();

	void drawNormal() {};

	void makeFaceList();

protected:

	FaceList *faceList;

};
#endif