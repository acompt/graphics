/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cone.cpp


****************************************************************************/


#include <GL/glui.h>
#include "Shape.h"
#include "Cone.h"
#include "Algebra.h"
#include <unistd.h> 
#include <tgmath.h> 

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5

Cone::Cone() {
	faceList = new FaceList;
	vertexList = new VertexList;
}

void Cone::draw() {

	faceList->makeList(m_segmentsX, m_segmentsY, 20);
	vertexList->makeList(m_segmentsX, m_segmentsY);
	makeFaceList();

	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;

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
		glNormal3f(face.nx, face.ny, face.nz);

		glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
		glVertex3f(x2, y2, z2);  // the direction of the front face depends 
		glVertex3f(x3, y3, z3);  // on the order in which you put the vertices
		glEnd();
	}

}

void Cone::makeFaceList() {
	double height = 1.0f;
	double diameter = 1.0f;
	double radius = (double)(diameter / 2);

	double initX = 0.5f;
	double initZ = 0.0f;
	double initY = -0.5f;

	double termX = 0.0;
	double termY = 0.5;
	double termZ = 0.0;

	double theta = 0.0f;
	double addTheta = (2 * PI) / m_segmentsX;

	// Fine base length of small triangles
	double baseTheta = addTheta / 2.0;
	double sideLength = 2 * ( 0.5 * tan(baseTheta));
	double tbase = sideLength / m_segmentsY;


	double slope;
	double radRatio = 0.5;


	theta += addTheta;
	//draw bottom
	for (int i = 0; i < (this -> m_segmentsX); i++) {

		Vertex vertex1, vertex2, vertex3;
		vertex1.x = initX;
		vertex1.y = initY;
		vertex1.z = initZ; 
		vertex1.status = true;
 
		initX = (radius * cos(theta));

		initZ = (radius * sin(theta));

		vertex2.x = initX;
		vertex2.y = initY;
		vertex2.z = initZ; 
		vertex2.status = true;

		vertex3.x = 0.0f;
		vertex3.z = 0.0f;
		vertex3.y = -0.5f;
		vertex3.status = true;

		Vector v1(vertex1.x - vertex2.x, vertex1.y - vertex2.y, 
					vertex1.z - vertex2.z);
		Vector v2(vertex1.x - vertex3.x, vertex1.y - vertex3.y, 
					vertex1.z - vertex3.z);
		Vector v3 = cross(v1, v2);
		v3.normalize();

		//make bottom faces
		faceList->addFace(vertex1, vertex2, vertex3, v3.at(0), 
							v3.at(1), v3.at(2));

		vertexList->addVertex(vertex1.x, vertex1.y, vertex1.z, vertex1.x, vertex1.y - 1.0f, vertex1.z);

		theta += addTheta;
	}
	//middle of bottom
	vertexList->addVertex(0.0f, -0.5f, 0.0f, 0.0f, -1.5f, 0.0f);

	initX = 0.5f;
	initZ = 0.0f;
	initY = -0.5f;

	// Draw each side
	for (int i = 0; i < (this -> m_segmentsX); i++) {


		Vertex startV, endV, v1, v2, v3, v4;
		double tot_dX, tot_dZ, dX, dZ;
		double uT_dX, uT_dY, uT_dZ, udX, udY, udZ, bT_dX, bT_dZ, bdX, bdZ;


		startV.x = initX;
		startV.y = initY;
		startV.z = initZ; 
 
		initX = (radius * cos(theta));
		initZ = (radius * sin(theta));

		endV.x = initX;
		endV.y = initY;
		endV.z = initZ; 


		tot_dX = endV.x - startV.x;
		tot_dZ = endV.z - startV.z;

		uT_dX = termX - startV.x;
		uT_dY = termY - startV.y;
		uT_dZ = termZ - startV.z;

		bT_dX = termX - endV.x;
		bT_dZ = termZ - endV.z;

		udX = uT_dX / m_segmentsY;
		udY = uT_dY / m_segmentsY;
		udZ = uT_dZ / m_segmentsY;

		bdX = bT_dX / m_segmentsY;
		bdZ = bT_dZ / m_segmentsY;

		

		for (int h = m_segmentsY; h > 0; h--) {

			v1 = startV;

			v2.x = v1.x + udX;
			v2.y = v1.y + udY;
			v2.z = v1.z + udZ;

			v4 = endV;

			v3.x = v4.x + bdX;
			v3.y = v4.y + udY;
			v3.z = v4.z + bdZ;

			Vector vec1(v4.x - v1.x, v4.y - v1.y, v4.z - v1.z);
			Vector vec2(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z);
			Vector vec3 = cross(vec1, vec2);
			vec3.normalize();

			faceList->addFace(v2, v1, v3, vec3.at(0), vec3.at(1), vec3.at(2));
			faceList->addFace(v3, v1, v4, vec3.at(0), vec3.at(1), vec3.at(2));

			float r = sqrt(v1.x * v1.x + v1.z * v1.z);
			float nr = r + 1.0f;
			float nx = (nr * v1.x) / r;
			float nz = (nr * v1.z) / r;
			vertexList->addVertex(v1.x, v1.y, v1.z, nx, v1.y + vec3.at(1), nz);

			startV = v2;
			endV = v3;

		}



		startV = endV;
		theta += addTheta;
	} 
}

void Cone::drawNormal() {
	Vertex vertex;

	for (int i = 0; i < vertexList->getLength(); i++) {
		vertex = vertexList->getVertex(i);
		glBegin(GL_LINES);

		glVertex3f(vertex.x, vertex.y, vertex.z);
		glVertex3f(vertex.nx, vertex.ny, vertex.nz);
		glEnd();
	}
}










