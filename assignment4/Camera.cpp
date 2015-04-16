
#include "Camera.h"
#include "stdio.h"


Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Reset() {

	SetViewAngle(30);
	Point theP = Point(0, 0, 0);
	Vector lVec = Vector(0, 0, -1);
	Vector uVec = Vector(0, 1, 0);
	Orient(theP, lVec, uVec);

	SetNearPlane(1);
	SetFarPlane(500);
}

Vector Camera::getV(){
	return this->v;

}

Vector Camera::getU(){
	return this->u;


}



void Camera::Orient(Point& eye, Point& focus, Vector& up) {

	Vector look = focus - eye;

	look.normalize();

	this -> Orient(eye, look, up);

}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
	

	this -> eyePoint = eye;
	this -> upVector = up;
	this -> lookVector = look;

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
	m2e22 = 1 / (this->farPlane * (GetScreenWidthRatio() * tan(viewHeightAngle / 2)));
	m2e33 = 1 / farPlane;
	M2 = Matrix (m2e11, 0, 0, 0,
				  0, m2e22, 0, 0,
				  0, 0, m2e33, 0,
				  0, 0, 0, 1);


	return M1 * M2;
}


void Camera::SetViewAngle (double viewAngle) {
	//viewAngle = viewAngle * (PI / 180);

	this -> viewAngle = viewAngle;	
	this -> viewHeightAngle = viewAngle;
	this -> filmPlanDepth = -1.0 / tan((viewAngle*RAD) / 2.0);

}

void Camera::SetNearPlane (double nearPlane) {

	this -> nearPlane = nearPlane;

}

void Camera::SetFarPlane (double farPlane) {

	this -> farPlane = farPlane;

}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {

	this -> screenWidth = (double)screenWidth;
	this -> screenHeight = (double)screenHeight;
	this -> screenWidthRatio = this -> screenHeight / this -> screenWidth;

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


void Camera::RotateV(double angle) {

	angle = angle * (PI / 180);
	Matrix R = rot_mat(v, angle);
	w = R * w; 
	u = R * u; 

}

void Camera::RotateU(double angle) {

	angle = angle * (PI / 180);
	Matrix R = rot_mat(u, angle);
	v = R*v;
	w = R*w;

	lookVector = R * lookVector;
	upVector = R * upVector;


}

void Camera::RotateW(double angle) {

	angle = angle * (PI / -180);
	Matrix R = rot_mat(w, angle);
	u = R * u;
	v = R * v;

	upVector = R * upVector;

}

void Camera::Translate(const Vector &v) {
	Matrix R = Matrix (1, 0, 0, v[0],
				 0, 1, 0, v[1],
				 0, 0, 1, v[2],
				 0, 0, 0, 1);
	eyePoint = R * eyePoint;
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
	degrees = degrees * (PI / 180);
	Matrix R = rot_mat(p, axis, degrees);
	u = R * u;
	v = R * v;
	w = R * w;
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