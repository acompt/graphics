/****************************************************************************

Adam Piel & Andrea Compton
apiel02     acompt01

Tufts COMP 175: Assignment 1: Shapes
Due Monday, Feb 16, 2015

File name: Cube.cpp


****************************************************************************/

#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include <unistd.h> 

#define edge_length 1
#define eps 0.00001

#define NEG -0.5
#define POS 0.5



/* *  Static Functions * */


bool static crossNotDone(float a, float b, float sgn);
bool static downNotDone(float a, float b, float sgn);
static double multV(Vector v1, Vector v2);
static bool isEqual(double i, double j);

Vert static getMid(Vert v1, Vert v2);


static float dX;
static float dY;


/* * * * * * * * * * * * */


Cube::Cube() 
{
	cubelist = new CubeList();

}

double Cube::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

	double t1, t2, t3, t4, t5, t6, tsmall;
	Point px1, px2, py1, py2, pz1, pz2;
	Vector nx1 = Vector(1, 0, 0);
	Vector nx2 = Vector(-1, 0, 0);
	Vector ny1 = Vector(0, 1, 0);
	Vector ny2 = Vector(0, -1, 0);
	Vector nz1 = Vector(0, 0, 1);
	Vector nz2 = Vector(0, 0, -1);
	Point x1 = Point(0.5, 0, 0);
	Point x2 = Point(-0.5, 0, 0);
	Point y1 = Point(0, 0.5, 0);
	Point y2 = Point(0, -0.5, 0);
	Point z1 = Point(0, 0, 0.5);
	Point z2 = Point(0, 0, -0.5);

	t1 = -(multV((eyePointP - x1), nx1)) / multV(rayV, nx1);
	px1 = eyePointP + t1*rayV;
	t2 = -(multV((eyePointP - x2), nx2)) / multV(rayV, nx2);
	px2 = eyePointP + t2*rayV;

	t3 = -(multV((eyePointP - y1), ny1)) / multV(rayV, ny1);
	py1 = eyePointP + t3*rayV;
	t4 = -(multV((eyePointP - y2), ny2)) / multV(rayV, ny2);
	py2 = eyePointP + t4*rayV;

	t5 = -(multV((eyePointP - z1), nz1)) / multV(rayV, nz1);
	pz1 = eyePointP + t5*rayV;
	t6 = -(multV((eyePointP - z2), nz2)) / multV(rayV, nz2);
	pz2 = eyePointP + t6*rayV;

	//if t3, t4 within square, else -1
	tsmall = -1.0;

	if(px1[1] <= 0.5 && px1[1] >= -0.5 && px1[2] <= 0.5 && px1[2] >= -0.5 &&
		isEqual(px1[0], 0.5)) {
		tsmall = t1;
	}
	if(px2[1] <= 0.5 && px2[1] >= -0.5 && px2[2] <= 0.5 && px2[2] >= -0.5 &&
		isEqual(px2[0], -0.5)) {
		if (tsmall != -1.0)
			tsmall = fmin(t2, tsmall);
		else tsmall = t2;
	}
	if(py1[0] <= 0.5 && py1[0] >= -0.5 && py1[2] <= 0.5 && py1[2] >= -0.5) {
		if (tsmall != -1.0)
			tsmall = fmin(t3, tsmall);
		else tsmall = t3;
	}
	if(py2[0] <= 0.5 && py2[0] >= -0.5 && py2[2] <= 0.5 && py2[2] >= -0.5){ 
		if (tsmall != -1.0)
			tsmall = fmin(t4, tsmall);
		else tsmall = t4;
	}
	if(pz1[0] <= 0.5 && pz1[0] >= -0.5 && pz1[1] <= 0.5 && pz1[1] >= -0.5 &&
		isEqual(pz1[2], 0.5)) {
		if (tsmall != -1.0)
			tsmall = fmin(t5, tsmall);
		else tsmall = t5;
	}
	if(pz2[0] <= 0.5 && pz2[0] >= -0.5 && pz2[1] <= 0.5 && pz2[1] >= -0.5 &&
		isEqual(pz2[2], -0.5)) {
		if (tsmall != -1.0)
			tsmall = fmin(t6, tsmall);
		else tsmall = t6;
	}

	return tsmall;
}

Vector Cube::findIsectNormal(Point eyePoint, Vector ray, double dist){
	Point i = eyePoint + dist * ray;
	Vector norm = Vector(0, 0, 0);
	//top
	if(isEqual(i[1], 0.5)) norm = Vector(0, 1, 0);
	//bottom
	if(isEqual(i[1], -0.5)) norm = Vector(0, -1, 0);
	//back
	if(isEqual(i[0], 0.5)) norm = Vector(1, 0, 0);
	//front
	if(isEqual(i[0], -0.5)) norm = Vector(-1, 0, 0);
	//left
	if(isEqual(i[2], 0.5)) norm = Vector(0, 0, 1);
	//right
	if(isEqual(i[2], -0.5)) norm = Vector(0, 0, -1);

	// Point origin =  Point(0, 0, 0);
	// Vector norm = i - origin;
	norm.normalize();
	
	return norm;
}

static bool isEqual(double i, double j) {

	if (abs(i - j) < eps) {
		return true;
	} else {
		return false;
	}
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

void Cube::draw()
{

	dX = ((float)edge_length) / ((float)(this -> m_segmentsX));
	dY = ((float)edge_length) / ((float)(this -> m_segmentsY));

	cubelist->makeList(m_segmentsX, m_segmentsY);

	drawFace(NEG, POS, POS, 1, -1, 0, 1.0);
	drawFace(POS, POS, POS, 0, -1, -1, 1.0);
	drawFace(POS, POS, NEG, -1, -1, 0, -1.0);
	drawFace(NEG, POS, NEG, 0, -1, 1, -1.0);
	drawFace(NEG, POS, NEG, 1, 0, 1, 1.0);
	drawFace(NEG, NEG, POS, 1, 0, -1, -1.0);

}


void Cube::drawFace(float xc, float yc, float zc, float dirx, 
					float diry, float dirz, float norm)
{

	float ax = xc;
	float ay = yc;
	float az = zc;
	float cross_term, down_term;
	float *c_ptr;
	float *d_ptr;
	float c_sgn, d_sgn;
	float dC, dD;
	float cStart;
	float isFirst;

	float xnorm, ynorm, znorm;

	/* Determine terminating points for both the cross, 
		and down (relative) directions. */
	if (dirx != 0){
		cross_term = dirx * ((float)(edge_length)) / 2;
		c_ptr = &ax;
		c_sgn = dirx;
		dC = dX;
		cStart = xc;

		if (diry != 0){
			down_term = diry * (((float)(edge_length)) / 2);
			d_ptr = &ay;
			d_sgn = diry;
			dD = dY;
			xnorm = 0;
			ynorm = 0;
			znorm = norm;
		} else {
			down_term = dirz * (((float)(edge_length)) / 2);
			d_ptr = &az;
			d_sgn = dirz;
			dD = dX;
			xnorm = 0;
			ynorm = norm;
			znorm = 0;
		}
	} else {
		cross_term = dirz * ((float)(edge_length)) / 2;
		c_ptr = &az;
		c_sgn = dirz;
		dC = dX;
		cStart = zc;

		down_term = diry * (((float)(edge_length)) / 2);
		d_ptr = &ay;
		d_sgn = diry;
		dD = dY;

		xnorm = norm;
		ynorm = 0;
		znorm = 0;
	}


	while (downNotDone(*d_ptr, down_term, d_sgn)) {


		glBegin(GL_TRIANGLE_STRIP);
		isFirst = 1;
		while (crossNotDone(*c_ptr, cross_term, c_sgn)) {

			glNormal3f(xnorm, ynorm, znorm);
	        glVertex3f(ax, ay, az);
	        cubelist->addVert(ax, ay, az, isFirst, xnorm, ynorm, znorm);
	        isFirst = 0;

			*d_ptr += (d_sgn) * dD;
			glNormal3f(xnorm, ynorm, znorm);
			glVertex3f(ax, ay, az);
			cubelist->addVert(ax, ay, az, isFirst, xnorm, ynorm, znorm);

			*d_ptr -= (d_sgn) * dD;
			*c_ptr += (c_sgn) * dC;

	    }
		glEnd();

		*d_ptr += (d_sgn) * dD;
		*c_ptr = (float)cStart;
	}

}


void Cube::drawNormal()
{
	
	Vert v1, v2, v3;
	float cx, cy, cz;


	for (int i = 0; i < cubelist->getLength(); i++) {

		v1 = cubelist->getVert(i);
		v2 = cubelist->getVert(i + 1);
		v3 = cubelist->getVert(i + 2);

		if (v2.isFirst || v3.isFirst){
			continue;
		}

		cx = (v1.vx + v2.vx + v3.vx) / 3;
		cy = (v1.vy + v2.vy + v3.vy) / 3;
		cz = (v1.vz + v2.vz + v3.vz) / 3;

		glBegin(GL_LINES);
		glVertex3f(cx, cy, cz);
		glVertex3f(cx + v1.xnorm, cy + v1.ynorm, cz + v1.znorm);

		glEnd();

	}


}

bool static crossNotDone(float a, float b, float sgn)
{

	if (((sgn > 0) && (a <= b)) || ((sgn < 0) && (a >= b))) {
		return true;
	}
	if (fabs((float)(a - b)) < ((float)eps)) {
		return true;
	}
	return false;

}

bool static downNotDone(float a, float b, float sgn)
{

	if (fabs((float)(a - b)) < ((float)eps)) {
		return false;
	}
	if (((sgn < 0) && (a > b)) || ((sgn > 0) && (a < b))) { 
		return true;
	}

	return false;

}

