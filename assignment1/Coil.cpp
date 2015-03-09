/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Coil.cpp


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
	vertexList = new VertexList;
}

void Coil::draw() {

	faceList->makeList(m_segmentsX, m_segmentsY, 20);
	vertexList->makeList(m_segmentsX, m_segmentsY);
	makeFaceList();
	makeVertexList();

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

 			Vector vec1(v2.x - v4.x, v2.y - v4.y, v2.z - v4.z);
			Vector vec2(v2.x - v3.x, v2.y - v3.y, v2.z - v3.z);
			Vector vec3 = cross(vec2, vec1);
			vec3.normalize();

			Vector vec6(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			Vector vec4(v1.x - v4.x, v1.y - v4.y, v1.z - v4.z);
			Vector vec5 = cross(vec6, vec4);
			vec5.normalize();

			faceList->addFace(v1, v2, v4, vec5.at(0), vec5.at(1), vec5.at(2));
			faceList->addFace(v2, v3, v4, vec3.at(0), vec3.at(1), vec3.at(2));

 			phi += addPhi;
 		}

		theta += addTheta;
	}

	// Now make the caps at the beginning and the end.

	Vertex bCent, eCent;

	bCent.x = (0.5 - radius);
	bCent.y = -0.5 + radius;
	bCent.z = 0;
	bCent.status = true;

	eCent.x = (0.5 - radius);
	eCent.y = 0.5 - radius;
	eCent.z = 0;
	eCent.status = true;

	phi = 0.0;

	for (int m = 0; m < this->m_segmentsY; m++ ) {

		v1 = getOutVert(bCent, 0.0, phi);
		v2 = getOutVert(bCent, 0.0, phi + addPhi);
		v3 = getOutVert(eCent, 0.0, phi);
		v4 = getOutVert(eCent, 0.0, phi + addPhi);

		Vector vec1(v1.x - bCent.x, v1.y - bCent.y, v1.z - bCent.z);
		Vector vec2(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		Vector vec3 = cross(vec1, vec2);
		vec3.normalize();

		Vector vec6(v3.x - eCent.x, v3.y - eCent.y, v3.z - eCent.z);
		Vector vec4(v3.x - v4.x, v3.y - v4.y, v3.z - v4.z);
		Vector vec5 = cross(vec6, vec4);
		vec5.normalize();


		faceList->addFace(v1, v2, bCent, vec3.at(0), vec3.at(1), vec3.at(2));
		faceList->addFace(v3, eCent, v4, vec5.at(0), vec5.at(1), vec5.at(2));

 		phi += addPhi;
	}

}

static Vertex getCentVert(double iter, double theta) {

	Vertex toReturn;

	toReturn.y = (-0.5 + radius) + ((iter) / totXSegs) * cRise;
	toReturn.x = (0.5 - radius) * cos(theta);
	toReturn.z = (0.5 - radius) * sin(theta);
	toReturn.status = false;

	return toReturn;
}

static Vertex getOutVert(Vertex cent, double theta, double phi) {

	Vertex toReturn;
	double newRad;

	toReturn.y = cent.y + radius * cos(phi);

	newRad = (0.5 - radius) + radius * sin(phi);

	toReturn.x = newRad * cos(theta);
	toReturn.z = newRad * sin(theta);
	toReturn.status = true;

	return toReturn;

}

void Coil::drawNormal() {
	Vertex vertex;
	printf("%i\n", vertexList->getLength());
	printf("%i\n", faceList->getLength());
	for (int i = 0; i < vertexList->getLength(); i++) {
		vertex = vertexList->getVertex(i);
		glBegin(GL_LINES);

		glVertex3f(vertex.x, vertex.y, vertex.z);
		glVertex3f(vertex.nx, vertex.ny, vertex.nz);
		// glVertex3f(face.a.x, face.a.y, face.a.z);
		// glVertex3f(face.a.x + face.nx, face.a.y + face.ny, face.a.z + face.nz);

		// glVertex3f(face.b.x, face.b.y, face.b.z);
		// glVertex3f(face.b.x + face.nx, face.b.y + face.ny, face.b.z + face.nz);

		// glVertex3f(face.c.x, face.c.y, face.c.z);
		// glVertex3f(face.c.x + face.nx, face.c.y + face.ny, face.c.z + face.nz);

		glEnd();
	}
}

void Coil::makeVertexList() {

	Face face;
	Vertex v1, v2, v3, vertex;
	bool found1, found2, found3;
	float cx, cy, cz, c;
	for (int i=0; i < faceList->getLength(); i++) {
		face = faceList->getFace(i);
		v1 = face.a;
		v2 = face.b;
		v3 = face.c;
		found1 = false;
		found2 = false;
		found3 = false;

		for(int j = 0; j < vertexList->getLength(); j++) {
			vertex = vertexList->getVertex(i);
			if ((IN_RANGE(v1.x, vertex.x)) && (IN_RANGE(v1.y, vertex.y)) && (IN_RANGE(v1.z, vertex.z))) {
				cx = v1.x + face.nx + vertex.x + vertex.nx;
				cy = v1.y + face.ny + vertex.y + vertex.ny;
				cz = v1.z + face.nz + vertex.z + vertex.nz;
				c = vertex.count + 1.0f;
				found1 = true;
				vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, i, c);
			}	
			if ((IN_RANGE(v2.x, vertex.x)) && (IN_RANGE(v2.y, vertex.y)) && (IN_RANGE(v2.z, vertex.z))){
				cx = v2.x + face.nx + vertex.x + vertex.nx;
				cy = v2.y + face.ny + vertex.y + vertex.ny;
				cz = v2.z + face.nz + vertex.z + vertex.nz;
				c = vertex.count + 1.0f;
				found2 = true;
				vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, i, c);
			}
			if ((IN_RANGE(v3.x, vertex.x)) && (IN_RANGE(v3.y, vertex.y)) && (IN_RANGE(v3.z, vertex.z))) {
				cx = v3.x + face.nx + vertex.x + vertex.nx;
				cy = v3.y + face.ny + vertex.y + vertex.ny;
				cz = v3.z + face.nz + vertex.z + vertex.nz;
				c = vertex.count + 1.0f;
				found3 = true;
				vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, i, c);
			}	
		}
		if (!found1) {
			vertexList->addVertex(v1.x, v1.y, v1.z, v1.x + face.nx, v1.y + face.ny, v1.z + face.nz);
		}	
		if (!found2) {
			vertexList->addVertex(v2.x, v2.y, v2.z, v2.x + face.nx, v2.y + face.ny, v2.z + face.nz);
		}
		if (!found3) {
			vertexList->addVertex(v3.x, v3.y, v3.z, v3.x + face.nx, v3.y + face.ny, v3.z + face.nz);
		}
	}

	for (int i = 0; i < vertexList->getLength(); i++) {
		Vertex v = vertexList->getVertex(i);
		float count = v.count;
		vertexList->setVertex(v.x, v.y, v.z, (v.nx/count), (v.ny/count), (v.nz/count), i, count);
	}
}



