/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Sphere.cpp


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

static double multP(Point p1, Point p2);
static double multV(Vector v1, Vector v2);
static double multPV(Point p, Vector v);


/* *  Static Functions * */

static Vertex getVert(double r, double theta, double phi);

/* * * * * * * * * * * * */

Sphere::Sphere() {
	faceList = new FaceList;
	vertexList = new VertexList;
}


double Sphere::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

	/* transformMatrix from obj to world space. */
	double t = -1.0;
	double r = 0.5;

	double A, B, C, t1, t2;

	A = multV(rayV, rayV);
	B = 2 * multPV(eyePointP, rayV);
	C = (multP(eyePointP, eyePointP)) - r*r;

	double check = B*B - 4*A*C;

	if (check < 0) {
		return -1;
	}

	t1 = ((-B) + sqrt(check)) / (2*A);
	t2 = ((-B) - sqrt(check)) / (2*A);

	return fmin(t1, t2);



	return 0;
}

static double multP(Point p1, Point p2){

	double toR;
	double x, y, z;

	x = p1[0] * p2[0];
	y = p1[1] * p2[1];
	z = p1[2] * p2[2];

	toR = x + y + z;
	return toR;

}

static double multPV(Point p, Vector v){

	double toR;
	double x, y, z;

	x = p[0] * v[0];
	y = p[1] * v[1];
	z = p[2] * v[2];

	toR = x + y + z;
	return toR;

}


static double multV(Vector v1, Vector v2) {

	double toR;
	double x, y, z;

	x = v1[0] * v2[0];
	y = v1[1] * v2[1];
	z = v1[2] * v2[2];

	toR = x + y + z;
	return toR;


}

Vector Sphere::findIsectNormal(Point eyePoint, Vector ray, double dist){
	return Vector();
}

void Sphere::draw() {

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
		
		//glNormal3f(face.nx, face.ny, face.nz);
		glNormal3f(x1*2.0f, y1*2.0f, z1*2.0f);
		glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
		
		glNormal3f(x2*2.0f, y2*2.0f, z2*2.0f);
		glVertex3f(x2, y2, z2);  // the direction of the front face depends 
		
		glNormal3f(x3*2.0f, y3*2.0f, z3*2.0f);
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

			vertexList->addVertex(v1.x, v1.y, v1.z, v1.x*2.0f, v1.y*2.0f, v1.z*2.0f);
			//vertexList->addVertex(v2.x, v2.y, v2.z, v2.x*2.0f, v2.y*2.0f, v2.z*2.0f);
		
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





