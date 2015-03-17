#include "Camera.h"
#include "stdio.h"


Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {

	// Find the look vector by subtracting eye point from
	// focus point, call other orient function.

	Vector look = Vector(focus[0] - eye[0],
							focus[1] - eye[1],
							focus[2] - eye[2]);

	look.normalize();

	this -> Orient(eye, look, up);

}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {

	this -> eyePoint = eye;
	this -> upVector = up;
	this -> lookVector = look;

	// Time to set the u, v, w vectors
	w = (-1) * look;
	w.normalize();

	u = cross(up, w);
	u.normalize();

	v = cross(w, u);

}

Matrix Camera::GetProjectionMatrix() {

	
	double c = (-1) * (this -> nearPlane / this -> farPlane);

	// The unhinge matrix
	M1 = Matrix (1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, (-1 / (1 + c)), (c / (1 + c)), // Changed this back
				 0, 0, (-1), 0);

	// Scale matrix
	double m2e11, m2e22, m2e33;
	m2e11 = 1 / (this->farPlane * tan(viewAngle / 2));
	m2e22 = 1 / (this->farPlane * tan(viewHeightAngle / 2));
	m2e33 = 1 / farPlane;
	M2 = Matrix (m2e11, 0, 0, 0,
				  0, m2e22, 0, 0,
				  0, 0, m2e33, 0,
				  0, 0, 0, 1);


	return M1 * M2;
}


void Camera::SetViewAngle (double viewAngle) {
	viewAngle = viewAngle * (PI / 180);
	// TODO FIX THIS SO THESE TWO THINGS ARE DEPENDENT 
	// BUT NOT JUST EQUAL
	this -> viewAngle = viewAngle;
	this -> viewHeightAngle = viewAngle;

}

void Camera::SetNearPlane (double nearPlane) {

	this -> nearPlane = nearPlane;

}

void Camera::SetFarPlane (double farPlane) {

	this -> farPlane = farPlane;

}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {

	this -> screenWidth = screenWidth;
	this -> screenHeight = screenHeight;

}

Matrix Camera::GetModelViewMatrix() {


	//Rotation matrix
	M3 = Matrix ( u[0], u[1], u[2], 0,
					v[0], v[1], v[2], 0,
					w[0], w[1], w[2], 0,
					0, 0, 0, 1);


	// Translation matrix
	M4 = Matrix (1, 0, 0, (-1) * eyePoint[0],
				0, 1, 0, (-1) * eyePoint[1],
				0, 0, 1, (-1) * eyePoint[2],
				0, 0, 0, 1);

	// M1 and M3 do something! We're on the right track!
	mvM = M3 * M4;
	return mvM;

}

Matrix Camera::RotateArbVec(double angle, Vector a) {
	
	Matrix R, N1, N2, N3, N1i, N2i, T, Ti;

	angle = angle * (PI / 180);
	M1 = rotY_mat(atan(a[2] / a[0]));
	M2 = rotZ_mat((-1)*atan(a[1]/sqrt(a[0]*a[0] + a[2]*a[2])));
	M3 = rotX_mat(angle);
	N1 = rotY_mat(atan(a[2] / a[0]));
	N2 = rotZ_mat((-1)*atan(a[1]/sqrt(a[0]*a[0] + a[2]*a[2])));
	N3 = rotX_mat(angle);

	N1i = inv_rotY_mat(atan(a[2] / a[0]));
	N2i = inv_rotZ_mat((-1)*atan(a[1]/sqrt(a[0]*a[0] + a[2]*a[2])));


	R = N1i * N2i * N3 * N2 * N1;

	return R;
}

void Camera::RotateV(double angle) {

	Matrix R = RotateArbVec(angle, v);

	w = R * w;
	u = R * u;

}

void Camera::RotateU(double angle) {

	Matrix R = RotateArbVec(angle, u);
	v = R*v;
	w = R*w;

}

void Camera::RotateW(double angle) {

	//return; // remove to try using below code

	Matrix R = RotateArbVec(angle, w);
	u = R * u;
	v = R * v;

}

void Camera::Translate(const Vector &v) {
	Matrix R = Matrix (1, 0, 0, v[0],
				 0, 1, 0, v[1],
				 0, 0, 1, v[2],
				 0, 0, 0, 1);
	eyePoint = R * eyePoint;
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
	Matrix T = Matrix (1, 0, 0, -p[0],
					 0, 1, 0, -p[1],
					 0, 0, 1, -p[2],
					 0, 0, 0, 1);

	Matrix R = RotateArbVec(degrees, axis);

	v = invert(T) * R * T * v;
	u = invert(T) * R * T * u;
	w = invert(T) * R * T * w;

}


Point Camera::GetEyePoint() {
	
	return eyePoint;
}

Vector Camera::GetLookVector() {
	
	return lookVector;
}

Vector Camera::GetUpVector() {
	
	return upVector;
}

double Camera::GetViewAngle() {
	
	return viewAngle;
}

double Camera::GetNearPlane() {
	
	return nearPlane;
}

double Camera::GetFarPlane() {
	
	return farPlane;
}

int Camera::GetScreenWidth() {
	
	return screenWidth;
}

int Camera::GetScreenHeight() {
	
	return screenHeight;
}

double Camera::GetFilmPlanDepth() {
	
	return filmPlanDepth;
}

double Camera::GetScreenWidthRatio() {

	return screenWidthRatio;
}
