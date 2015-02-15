/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Wednesday, Feb 11, 2015

File name: Sphere.cpp

Description:



****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "Sphere.h"
#include "Algebra.h"
#include <unistd.h> 
#include <tgmath.h> 

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5

/* *  Static Functions * */

static Vertex getVert(double r, double theta, double phi);

/* * * * * * * * * * * * */

Sphere::Sphere() {
	faceList = new FaceList;
}

void Sphere::draw() {

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
		glNormal3f(face.nx, face.ny, face.nz);

		glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
		glVertex3f(x2, y2, z2);  // the direction of the front face depends 
		glVertex3f(x3, y3, z3);  // on the order in which you put the vertices
		glEnd();
	}

}

void Sphere::makeFaceList() {
	double height = 1.0;
	double diameter = 1.0;
	double radius = (double)(diameter / 2);

	double theta = 0.0;
	double addTheta = (2 * PI) / m_segmentsX;

	double phi = 0.0;
	double addPhi = ( PI) / m_segmentsY;

	// Fine base length of small triangles
	double baseTheta = addTheta / 2.0;
	double sideLength = 2 * ( 0.5 * tan(baseTheta));
	double tbase = sideLength / m_segmentsY;


	double slope;
	double radRatio = 0.5;


	theta += addTheta;
	//draw bottom


	Vertex v1, v2, v3, v4;


	for (int j = 0; j < m_segmentsY; j++) {



		for (int i = 0; i < (this -> m_segmentsX); i++) {

			v1 = getVert(radius, theta, phi);
	 		v2 = getVert(radius, theta + addTheta, phi);
	 		v3 = getVert(radius, theta, phi + addPhi);
	 		v4 = getVert(radius, theta + addTheta, phi + addPhi);

	 		Vector vec1(v1.x - v4.x, v1.y - v4.y, v1.z - v4.z);
			Vector vec2(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z);
			Vector vec3 = cross(vec1, vec2);
			vec3.normalize();

			Vector vec6(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			Vector vec4(v1.x - v4.x, v1.y - v4.y, v1.z - v4.z);
			Vector vec5 = cross(vec6, vec4);
			vec5.normalize();

			faceList->addFace(v1, v4, v3, vec3.at(0), vec3.at(1), vec3.at(2));
			faceList->addFace(v1, v2, v4, vec5.at(0), vec5.at(1), vec5.at(2));

			theta = theta + addTheta;
		}

		phi = phi + addPhi;
		theta = 0.0;

	}



}



static Vertex getVert(double r, double theta, double phi) {

	Vertex toReturn;

	toReturn.x = r * sin(phi) * cos(theta);
	toReturn.y = r * cos(phi);
	toReturn.z = r * sin(phi) * sin(theta);

	return toReturn;
}

void Sphere::drawNormal() {
	Face face;

	for (int i = 0; i < faceList->getLength(); i++) {
		face = faceList->getFace(i);
		
		glBegin(GL_LINES);
		glVertex3f(face.a.x, face.a.y, face.a.z);
		glVertex3f(face.a.x + face.nx, face.a.y + face.ny, face.a.z + face.nz);

		glVertex3f(face.b.x, face.b.y, face.b.z);
		glVertex3f(face.b.x + face.nx, face.b.y + face.ny, face.b.z + face.nz);

		glVertex3f(face.c.x, face.c.y, face.c.z);
		glVertex3f(face.c.x + face.nx, face.c.y + face.ny, face.c.z + face.nz);

		glEnd();
	}
}





