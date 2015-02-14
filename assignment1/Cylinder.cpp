/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Wednesday, Feb 11, 2015

File name: Cube.cpp

Description:



****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "Cylinder.h"
#include "Algebra.h"
#include <unistd.h> 

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5

Cylinder::Cylinder() {
	faceList = new FaceList;
}

void Cylinder::draw() {

	faceList->makeList(m_segmentsX, m_segmentsY, 10);
	makeFaceList();

	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;

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

void Cylinder::drawNormal() {
	Face face;

	for (int i = 0; i < faceList->getLength(); i++) {
		face = faceList->getFace(i);
		glBegin(GL_LINES);
		glVertex3f(face.a.x, face.a.y, face.a.z);
		glVertex3f(face.a.x + face.nx, face.a.y + face.ny, face.a.z + face.nz);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(face.b.x, face.b.y, face.b.z);
		glVertex3f(face.b.x + face.nx, face.b.y + face.ny, face.b.z + face.nz);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(face.c.x, face.c.y, face.c.z);
		glVertex3f(face.c.x + face.nx, face.c.y + face.ny, face.c.z + face.nz);

		glEnd();
	}
}

void Cylinder::makeFaceList() {
	float height = 1.0f;
	float diameter = 1.0f;
	float radius = (float)(diameter / 2);

	float initX = 0.5f;
	float initZ = 0.0f;
	float initY = -0.5f;
	float theta = 0.0f;
	float addTheta = (2 * PI) / m_segmentsX;
	float stackHeight = height / m_segmentsY;


	theta += addTheta;

	makeLid(theta, initX, initY, initZ, addTheta, radius);
	//draw bottom and top
	
	makeSides(theta, initX, initY, initZ, addTheta, radius, stackHeight);

}

void Cylinder::makeLid(float theta, float initX, float initY, float initZ, 
			float addTheta, float radius) {

	for (int i = 0; i < m_segmentsX; i++) {
		Vertex vertex1, vertex2, vertex3;

		vertex1.x = initX;
		vertex1.y = initY;
		vertex1.z = initZ; 
 
		initX = (radius * cos(theta));
		initZ = (radius * sin(theta));

		vertex2.x = initX;
		vertex2.y = initY;
		vertex2.z = initZ; 

		vertex3.x = 0.0f;
		vertex3.z = 0.0f;
		vertex3.y = -0.5f;

		Vector v1(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
		Vector v2(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
		Vector v3 = cross(v1, v2);

		//make bottom faces
		faceList->addFace(vertex1, vertex2, vertex3, v3.at(0), v3.at(1), v3.at(2));
		//make top faces
		vertex3.y = 0.5f;
		vertex2.y = 0.5f;
		vertex1.y = 0.5f;

		Vector v5(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
		Vector v4(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
		v3 = cross(v5, v4);

		faceList->addFace(vertex1, vertex2, vertex3, -v3.at(0), -v3.at(1), -v3.at(2));
		theta += addTheta;
	}
}

void Cylinder::makeSides(float theta, float initX, float initY, float initZ, 
			float addTheta, float radius, float stackHeight) {

	float numFaces = m_segmentsX * 2;
	for (int j = 0; j < m_segmentsY; j++) {

		for (int i = 0; i < numFaces; i++) {
			Vertex vertex1, vertex2, vertex3, vertex4;

			vertex1.x = initX;
			vertex1.y = initY;
			vertex1.z = initZ; 

			vertex2.x = initX;
			vertex2.y = initY + stackHeight;
			vertex2.z = initZ; 

			initX = (radius * cos(theta));
			initZ = (radius * sin(theta));
			vertex3.x = initX;
			vertex3.y = initY;
			vertex3.z = initZ; 

			vertex4.x = initX;
			vertex4.y = initY + stackHeight;
			vertex4.z = initZ; 

			Vector v1(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
			Vector v2(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
			Vector v3 = cross(v1, v2);
			faceList->addFace(vertex1, vertex2, vertex3, 5*v3.at(0), 5*v3.at(1), 5*v3.at(2));

			Vector v6(vertex4.x - vertex2.x, vertex4.y - vertex2.y, vertex4.z - vertex2.z);
			Vector v4(vertex4.x - vertex3.x, vertex4.y - vertex3.y, vertex4.z - vertex3.z);
			Vector v5 = cross(v6, v4);

			faceList->addFace(vertex4, vertex2, vertex3, 5*v5.at(0), 5*v5.at(1), 5*v5.at(2));

			theta += addTheta;
		} 
		initY += stackHeight;
	}
}






