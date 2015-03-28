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

Vert static getMid(Vert v1, Vert v2);


static float dX;
static float dY;


/* * * * * * * * * * * * */


Cube::Cube() 
{
	cubelist = new CubeList();

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

