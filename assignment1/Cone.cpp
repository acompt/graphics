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

	//	vertexList->addVertex(vertex1.x, vertex1.y, vertex1.z, vertex1.x, vertex1.y - 1.0f, vertex1.z);

		theta += addTheta;
	}
	//middle of bottom
	//vertexList->addVertex(0.0f, -0.5f, 0.0f, 0.0f, -1.5f, 0.0f);

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
<<<<<<< HEAD
		bdZ = bT_dZ / m_segmentsY;	
=======
		bdZ = bT_dZ / m_segmentsY;

		
>>>>>>> 211c6ee40b88252b6557e5582c24d5896fcf7356

		for (int h = m_segmentsY; h > 0; h--) {

			v1 = startV;
			v1.status = false;

			v2.x = v1.x + udX;
			v2.y = v1.y + udY;
			v2.z = v1.z + udZ;
<<<<<<< HEAD
			v2.status = false;
=======

			v4 = endV;

			v3.x = v4.x + bdX;
			v3.y = v4.y + udY;
			v3.z = v4.z + bdZ;


			faceList->addFace(v2, v1, v3, NULL, NULL, NULL ); //vec3.at(0), vec3.at(1), vec3.at(2));
			faceList->addFace(v3, v1, v4, NULL, NULL, NULL ); //vec3.at(0), vec3.at(1), vec3.at(2));



			startV = v2;
			endV = v3;


/*
			for (int i = 0; i < h; i++) {
>>>>>>> 211c6ee40b88252b6557e5582c24d5896fcf7356

			v4 = endV;
			v4.status = false;

			v3.x = v4.x + bdX;
			v3.y = v4.y + udY;
			v3.z = v4.z + bdZ;
			v3.status = false;

			Vector vec1(v2.x - v4.x, v2.y - v4.y, v2.z - v4.z);
			Vector vec2(v2.x - v3.x, v2.y - v3.y, v2.z - v3.z);
			Vector vec3 = cross(vec2, vec1);
			vec3.normalize();

<<<<<<< HEAD
			Vector vec6(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			Vector vec4(v1.x - v4.x, v1.y - v4.y, v1.z - v4.z);
			Vector vec5 = cross(vec6, vec4);
			vec5.normalize();
=======
				vertexList->addVertex(v1.x, v1.y, v1.z, v1.x*2.0f, v1.y*2.0f, v1.z*2.0f);
>>>>>>> 211c6ee40b88252b6557e5582c24d5896fcf7356

			faceList->addFace(v1, v2, v4, vec5.at(0), vec5.at(1), vec5.at(2));
			faceList->addFace(v2, v3, v4, vec3.at(0), vec3.at(1), vec3.at(2));

			// Vector vec6(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			// Vector vec4(v1.x - v4.x, v1.y - v4.y, v1.z - v4.z);
			// Vector vec5 = cross(vec6, vec4);
			// vec5.normalize();

<<<<<<< HEAD
			// faceList->addFace(v2, v1, v3, -vec3.at(0), vec3.at(1), -vec3.at(2));
			// faceList->addFace(v3, v1, v4, vec3.at(0), vec3.at(1), vec3.at(2));

		//	vertexList->addVertex(v1.x, v1.y, v1.z, v1.x*2.0f, v1.y*2.0f, v1.z*2.0f);

			startV = v2;
			endV = v3;
		}

=======
					faceList->addFace(v3, v2, v1, vec6.at(0), 
										vec6.at(1), vec6.at(2));
				}
				v1 = v2;
			}
			startV.x = startV.x + udX;
			startV.y = startV.y + udY;
			startV.z = startV.z + udZ; */
		}



>>>>>>> 211c6ee40b88252b6557e5582c24d5896fcf7356
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
		// glVertex3f(face.a.x, face.a.y, face.a.z);
		// glVertex3f(face.a.x + face.nx, face.a.y + face.ny, face.a.z + face.nz);

		// glVertex3f(face.b.x, face.b.y, face.b.z);
		// glVertex3f(face.b.x + face.nx, face.b.y + face.ny, face.b.z + face.nz);

		// glVertex3f(face.c.x, face.c.y, face.c.z);
		// glVertex3f(face.c.x + face.nx, face.c.y + face.ny, face.c.z + face.nz);

		glEnd();
	}
}

void Cone::makeVertexList() {

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
			vertex = vertexList->getVertex(j);
			if ((IN_RANGE(v1.x, vertex.x)) && (IN_RANGE(v1.y, vertex.y)) && (IN_RANGE(v1.z, vertex.z))) {
				if(!v1.status) {
					cx = face.nx + vertex.nx;
					cy = face.ny + vertex.ny;
					cz = face.nz + vertex.nz;
					c = vertex.count + 1.0f;
					found1 = true;
					vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, j, c);
				}
			}	
			if ((IN_RANGE(v2.x, vertex.x)) && (IN_RANGE(v2.y, vertex.y)) && (IN_RANGE(v2.z, vertex.z))){
				if(!v2.status) {
					cx = face.nx + vertex.nx;
					cy = face.ny + vertex.ny;
					cz = face.nz + vertex.nz;
					c = vertex.count + 1.0f;
					found2 = true;
					vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, j, c);
				}
			}
			if ((IN_RANGE(v3.x, vertex.x)) && (IN_RANGE(v3.y, vertex.y)) && (IN_RANGE(v3.z, vertex.z))) {
				if (!v3.status) {
					cx = face.nx + vertex.nx;
					cy = face.ny + vertex.ny;
					cz = face.nz + vertex.nz;
					c = vertex.count + 1.0f;
					found3 = true;
					vertexList->setVertex(vertex.x, vertex.y, vertex.z, cx, cy, cz, j, c);
				}
			}	
		}
		if (!found1) {
			vertexList->addVertex(v1.x, v1.y, v1.z, face.nx, face.ny, face.nz);
		}	
		if (!found2) {
			vertexList->addVertex(v2.x, v2.y, v2.z, face.nx, face.ny, face.nz);
		}
		if (!found3) {
			vertexList->addVertex(v3.x, v3.y, v3.z, face.nx, face.ny, face.nz);
		}
	}

	for (int i = 0; i < vertexList->getLength(); i++) {
		Vertex v = vertexList->getVertex(i);
		float count = v.count;
		vertexList->setVertex(v.x, v.y, v.z, (v.nx/count), (v.ny/count), (v.nz/count), i, count);
	}
}









