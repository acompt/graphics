#include "Camera.h"

Camera::Camera() {

	// mvM = new Matrix();
	// pjM = new Matrix();

	// eyePoint = new Point();
	// lookVector = new Vector();
	// upVector = new Vector();

	// viewAngle = 0;
	// nearPlane = 0;
	// farPlane = 0;

	// screenWidth = 0;
	// screenHeight = 0;

	// filmPlanDepth = 0;
	// screenWidthRatio = 0;

}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {

	this -> eyePoint = eye;
	this -> upVector = up;

	Matrix M1, M2, M3, M4;

	double c = (-1) * (this -> nearPlane / this -> farPlane);

	// The unhinge matrix
	// TODO: Should we calculate this elsewhere? And store it as member?
	M1 = Matrix (1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, (1 / (1 + c)), ((-1) * (c / (1 + c))),
				 0, 0, (-1), 0);

	// Scale matrix
	//M2 = 




}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
}

Matrix Camera::GetProjectionMatrix() {


	return pjM;
}


void Camera::SetViewAngle (double viewAngle) {

	this -> viewAngle = viewAngle;

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
	



	return mvM;
}

void Camera::RotateV(double angle) {
}

void Camera::RotateU(double angle) {
}

void Camera::RotateW(double angle) {
}

void Camera::Translate(const Vector &v) {
}


void Camera::Rotate(Point p, Vector axis, double degrees) {


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
