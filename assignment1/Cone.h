#ifndef Cone_H
#define Cone_H

#include "Shape.h"
#include "FaceList.h"

class Cone : public Shape {

public:
	Cone();
	~Cone() {};

	void draw();

	void drawNormal() {};

	void makeFaceList();

protected:

	FaceList *faceList;

};
#endif