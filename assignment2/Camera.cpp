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



}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {

	this -> eyePoint = eye;
	this -> upVector = up;


	// Time to set the u, v, w vectors
	w = (-1) * look;
	w.normalize();

	u = cross(up, w);
	u.normalize();

	v = cross(w, u);
	


	double c = (-1) * (this -> nearPlane / this -> farPlane);
	// The unhinge matrix
	// TODO: Should we calculate this elsewhere? And store it as member?
	M1 = Matrix (1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, (1 / (1 + c)), ((-1) * (c / (1 + c))),
				 0, 0, (-1), 0);

	// Scale matrix
	double m2e11, m2e22, m2e33;
	m2e11 = 1 / (this->farPlane * tan(viewWidthAngle / 2));
	m2e22 = 1 / (this->farPlane * tan(viewHeightAngle / 2));
	m2e33 = 1 / farPlane;
	M2 = Matrix (m2e11, 0, 0, 0,
				  0, m2e22, 0, 0,
				  0, 0, m2e33, 0,
				  0, 0, 0, 1);


	// Rotation matrix
	M3 = Matrix ( u[0], u[1], u[2], 0,
					v[0], v[1], v[2], 0,
					w[0], w[1], w[2], 0,
					0, 0, 0, 1);


	// Translation matrix
	M4 = Matrix (1, 0, 0, (-1) * eye[0],
				0, 1, 0, (-1) * eye[1],
				0, 0, 1, (-1) * eye[2],
				0, 0, 0, 1);

	mvM = M1 * M2 * M3 * M4;

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
