/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Wednesday, Feb 11, 2015

File name: Coil.cpp

Description:



****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "Coil.h"
#include "Algebra.h"
#include <unistd.h> 
#include <tgmath.h> 

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5

/* *  Static Functions * */

static Vertex getCentVert(double iter, double theta);
static Vertex getOutVert(Vertex cent, double theta, double phi);

static double diameter = 1.0 / 7.0;
static double radius = diameter / 2.0;
static double cRise = 1.0 - 2 * radius;
static double totXSegs;

/* * * * * * * * * * * * */

Coil::Coil() {
	faceList = new FaceList;
}

void Coil::draw() {

	faceList->makeList(m_segmentsX, m_segmentsY, 20);
	makeFaceList();

	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;

	// if(vertexList==NULL || faceList==NULL){
	// 	return;
	// }

	for (int i = 0; i < faceList->getLength(); i++) {
		Face face = faceList->getFace(i);
		x1 = face.a.x;
		y1 = face.a.y;
		z1 = face.a.z;

		x2 = face.b.x;
		y2 = face.b.y;
		z2 = face.b.z;

		x3 = face.c.x;
		y3 = face.c.y;
		z3 = face.c.z;

		glBegin(GL_TRIANGLES); // sets the shapes that openGL draws and determines
                           // the number of vertices that are necessary
		//setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3); // makes sure that each 
                                                   // vertex is correctly 
                                                   // scaled
		glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
		glVertex3f(x2, y2, z2);  // the direction of the front face depends 
		glVertex3f(x3, y3, z3);  // on the order in which you put the vertices
		glEnd();
	}

}

void Coil::makeFaceList() {

	double theta = 0.0;
	double addTheta = (2 * PI) / m_segmentsX;

	double diameter = 1.0 / 7.0;
	double radius = diameter / 2.0;

	double phi = 0.0;
	double addPhi = (2 * PI) / m_segmentsY;

	totXSegs = 3 * (this -> m_segmentsX);

	Vertex vStart, vEnd, v1, v2, v3, v4;


	for (int i = 0; i < totXSegs; i++) {
		vStart = getCentVert(i, theta);
 		vEnd = getCentVert(i + 1, theta + addTheta);

 		phi = 0.0;
 		for (int j = 0; j < this->m_segmentsY; j++) {

 			v1 = getOutVert(vStart, theta, phi);
 			v2 = getOutVert(vEnd, theta + addTheta, phi);
 			v3 = getOutVert(vEnd, theta + addTheta, phi + addPhi);
 			v4 = getOutVert(vStart, theta, phi + addPhi);

			faceList->addFace(v1, v2, v4, 0.0, 0.0, 0.0);
			faceList->addFace(v2, v3, v4, 0.0, 0.0, 0.0);

 			phi += addPhi;
 		}

		theta += addTheta;
	}

	// Now make the caps at the beginning and the end.

	Vertex bCent, eCent;

	bCent.x = (0.5 - radius);
	bCent.y = -0.5 + radius;
	bCent.z = 0;

	eCent.x = (0.5 - radius);
	eCent.y = 0.5 - radius;
	eCent.z = 0;

	phi = 0.0;

	for (int m = 0; m < this->m_segmentsY; m++ ) {

		v1 = getOutVert(bCent, 0.0, phi);
		v2 = getOutVert(bCent, 0.0, phi + addPhi);
		v3 = getOutVert(eCent, 0.0, phi);
		v4 = getOutVert(eCent, 0.0, phi + addPhi);

		faceList->addFace(v1, v2, bCent, 0.0, 0.0, 0.0);
		faceList->addFace(v3, eCent, v4, 0.0, 0.0, 0.0);

 		phi += addPhi;
	}

}

static Vertex getCentVert(double iter, double theta) {

	Vertex toReturn;

	toReturn.y = (-0.5 + radius) + ((iter) / totXSegs) * cRise;
	toReturn.x = (0.5 - radius) * cos(theta);
	toReturn.z = (0.5 - radius) * sin(theta);

	return toReturn;
}

static Vertex getOutVert(Vertex cent, double theta, double phi) {

	Vertex toReturn;
	double newRad;

	toReturn.y = cent.y + radius * cos(phi);

	newRad = (0.5 - radius) + radius * sin(phi);

	toReturn.x = newRad * cos(theta);
	toReturn.z = newRad * sin(theta);

	return toReturn;

}





