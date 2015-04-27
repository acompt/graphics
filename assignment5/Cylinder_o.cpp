/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cylinder.cpp


****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "Cylinder.h"
#include "Algebra.h"
#include <unistd.h> 

#define edge_length 1
#define eps 0.01

#define NEG -0.5
#define POS 0.5


static double multP(Point p1, Point p2);
static double multV(Vector v1, Vector v2);
static double multPV(Point p, Vector v);
static bool isEqual(double i, double j);
double minPos(double t1, double t2);


Cylinder::Cylinder() {
	faceList = new FaceList;
	vertexList = new VertexList;
}

double Cylinder::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

	double t = -1.0;
	double r = 0.5;

	double A, B, C, t1, t2, t3, t4, tsmall;
	Point p1, p2, p1a, p2a;
	Vector n_top = Vector(0, 1, 0);
	Vector n_bot = Vector(0, -1, 0);
	Point p_top = Point(0, 0.5, 0);
	Point p_bot = Point(0, -0.5, 0);

	A = rayV[0]*rayV[0] + rayV[2]*rayV[2];
	B = 2 * (rayV[0]*eyePointP[0] + rayV[2]*eyePointP[2]);
	C = eyePointP[0]*eyePointP[0] + eyePointP[2]*eyePointP[2] - r*r;

	double check = B*B - 4*A*C;

	if (check >= 0) {
		t1 = ((-B) + sqrt(check)) / (2*A);
		t2 = ((-B) - sqrt(check)) / (2*A);
	}

	
	t3 = -(multV((eyePointP - p_top), n_top)) / multV(rayV, n_top);
	p1 = eyePointP + t3*rayV;
	t4 = -(multV((eyePointP - p_bot), n_bot)) / multV(rayV, n_bot);
	p2 = eyePointP + t4*rayV;

	//if t3, t4 within circle, else -1
	//check p1[1] should be .5
	if (p1[0]*p1[0] + p1[2]*p1[2] > r*r) {
		t3 = -1.0;
		tsmall = t4;
	}
	if (p2[0]*p2[0] + p2[2]*p2[2] > r*r) {
		t4 = -1.0;
		tsmall = t3;
	}

	if (check < 0) return tsmall;

	p1a = eyePointP + t1*rayV;
	p2a = eyePointP + t2*rayV;

	bool p1a_in = ((p1a[1] < 0.5) && (p1a[1] > -0.5));
	bool p2a_in = ((p2a[1] < 0.5) && (p2a[1] > -0.5));

	if (p1a_in && p2a_in) {
		if(tsmall > 0) 
			t = fmin(tsmall, fmin(t1, t2));
		else
			t = fmin(t1, t2);
	} else if (p1a_in) {
		if(tsmall > 0) 
			t = fmin(tsmall, t1);
		else
			t = t1; 
	} else if (p2a_in) {
		if(tsmall > 0) 
			t = fmin(tsmall, t2);
		else
			t = t2; 
	}

	return t;
}


double minPos(double t1, double t2) {

	if ((t1 > 0) && (t2 < 0) ) {
		return t1;
	} else if ((t1 < 0) && (t2 >0)){
		return t2;
	} else if ((t1 > 0) && (t2 > 0)){
		return fmin(t1, t2);
	} else return -1.0;
}

Vector Cylinder::findIsectNormal(Point eyePoint, Vector ray, double dist){

	Point i = eyePoint + dist * ray;

	double y_top = 0.5;
	double y_bot = -0.5;

	if (isEqual(i[1], y_bot)) {
		//is on the bottom
		return Vector (0, -1, 0);

	} 
	else if (isEqual(i[1], y_top)) {
		//is on the top
		return Vector (0, 1, 0);
	}
	else {

		Point s = Point(0, i[1], 0);
		Vector norm = s - i;

		norm.normalize();
		return norm;
	}

}

static bool isEqual(double i, double j) {

	if (abs(i - j) < eps) {
		return true;
	} else {
		return false;
	}
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

void Cylinder::draw() {

	faceList->makeList(m_segmentsX, m_segmentsY, 10);
	vertexList->makeList(m_segmentsX, m_segmentsY);
	makeFaceList();

	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;

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
		glNormal3f(face.a.nx, face.a.ny, face.a.nz);
		glVertex3f(x1, y1, z1);  // set the three vertices for the triangle
		
		glNormal3f(face.b.nx, face.b.ny, face.b.nz);
		glVertex3f(x2, y2, z2);  // the direction of the front face depends 
		
		glNormal3f(face.c.nx, face.c.ny, face.c.nz);
		glVertex3f(x3, y3, z3);  // on the order in which you put the vertices
		glEnd();
	}

}

void Cylinder::drawNormal() {
	Vertex vertex;

	for (int i = 0; i < vertexList->getLength(); i++) {
		// face = faceList->getFace(i);
		vertex = vertexList->getVertex(i);

		glBegin(GL_LINES);
		glVertex3f(vertex.x, vertex.y, vertex.z);
		glVertex3f(vertex.nx, vertex.ny, vertex.nz);

		// glVertex3f(face.b.x, face.b.y, face.b.z);
		// glVertex3f(face.b.x + face.nx, face.b.y + face.ny, face.b.z + face.nz);

		// glVertex3f(face.c.x, face.c.y, face.c.z);
		// glVertex3f(face.c.x + face.nx, face.c.y + face.ny, face.c.z + face.nz);

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
		vertex1.nx = vertex1.x;
		vertex1.ny = vertex1.y - 1.0f;
 		vertex1.nz = vertex1.z;

		initX = (radius * cos(theta));
		initZ = (radius * sin(theta));

		vertex2.x = initX;
		vertex2.y = initY;
		vertex2.z = initZ; 
		vertex2.nx = vertex2.x;
		vertex2.ny = vertex2.y - 1.0f;
 		vertex2.nz = vertex2.z;

		vertex3.x = 0.0f;
		vertex3.z = 0.0f;
		vertex3.y = -0.5f;
		vertex3.nx = vertex3.x;
		vertex3.ny = vertex3.y - 1.0f;
 		vertex3.nz = vertex3.z;

		vertexList->addVertex(vertex1.x, vertex1.y, vertex1.z, vertex1.x, vertex1.y - 1.0f, vertex1.z);
		vertexList->addVertex(vertex2.x, vertex2.y, vertex2.z, vertex2.x, vertex2.y - 1.0f, vertex2.z);
		
		Vector v1(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
		Vector v2(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
		Vector v3 = cross(v1, v2);
		v3.normalize();

		//make bottom faces
		faceList->addFace(vertex2, vertex3, vertex1, v3.at(0), v3.at(1), v3.at(2));
		//make top faces
		vertex3.y = 0.5f;
		vertex2.y = 0.5f;
		vertex1.y = 0.5f;
		vertex1.ny = vertex1.y + 1.0f;
		vertex2.ny = vertex2.y + 1.0f;
		vertex3.ny = vertex3.y + 1.0f;

		vertexList->addVertex(vertex1.x, vertex1.y, vertex1.z, vertex1.x, vertex1.y + 1.0f, vertex1.z);
		vertexList->addVertex(vertex2.x, vertex2.y, vertex2.z, vertex2.x, vertex2.y + 1.0f, vertex2.z);

		Vector v5(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
		Vector v4(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
		v3 = cross(v5, v4);
		v3.normalize();

		// vertex1.ny = vertex1.y + 1.0f;
		// vertex2.ny = vertex2.y + 1.0f;
		// vertex3.ny = vertex3.y + 1.0f;
		faceList->addFace(vertex1, vertex2, vertex3, -v3.at(0), -v3.at(1), -v3.at(2));
		theta += addTheta;
	}
	vertexList->addVertex(0.0f, -0.5f, 0.0f, 0.0f, -1.5f, 0.0f);
	vertexList->addVertex(0.0f, 0.5f, 0.0f, 0.0f, 1.5f, 0.0f);
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
			vertex1.nx = vertex1.x * 2.0f;
			vertex1.ny = vertex1.y;
 			vertex1.nz = vertex1.z * 2.0f;

			vertex2.x = initX;
			vertex2.y = initY + stackHeight;
			vertex2.z = initZ; 
			vertex2.nx = vertex2.x * 2.0f;
			vertex2.ny = vertex2.y;
 			vertex2.nz = vertex2.z * 2.0f;

			initX = (radius * cos(theta));
			initZ = (radius * sin(theta));
			vertex3.x = initX;
			vertex3.y = initY;
			vertex3.z = initZ; 
			vertex3.nx = vertex3.x * 2.0f;
			vertex3.ny = vertex3.y;
 			vertex3.nz = vertex3.z * 2.0f;

			vertex4.x = initX;
			vertex4.y = initY + stackHeight;
			vertex4.z = initZ; 
			vertex4.nx = vertex4.x * 2.0f;
			vertex4.ny = vertex4.y;
 			vertex4.nz = vertex4.z * 2.0f;

			Vector v1(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
			Vector v2(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
			Vector v3 = cross(v1, v2);
			v3.normalize();
			faceList->addFace(vertex1, vertex2, vertex3, v3.at(0), v3.at(1), v3.at(2));

			Vector v6(vertex4.x - vertex2.x, vertex4.y - vertex2.y, vertex4.z - vertex2.z);
			Vector v4(vertex4.x - vertex3.x, vertex4.y - vertex3.y, vertex4.z - vertex3.z);
			Vector v5 = cross(v4, v6);
			v5.normalize();

			faceList->addFace(vertex4, vertex3, vertex2, v5.at(0), v5.at(1), v5.at(2));

			vertexList->addVertex(vertex1.x, vertex1.y, vertex1.z, vertex1.x * 2.0f, vertex1.y, vertex1.z * 2.0f);
			if (j == (m_segmentsY-1)) {
				vertexList->addVertex(vertex4.x, vertex4.y, vertex4.z, vertex4.x * 2.0f, vertex4.y, vertex4.z * 2.0f);
			}
			theta += addTheta;
		} 
		initY += stackHeight;
	}
}






