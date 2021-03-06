

#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {

	public:

		Camera();
		~Camera();
		void Reset();
		void Orient(Point& eye, Point& focus, Vector& up);
		void Orient(Point& eye, Vector& look, Vector& up);
		void SetViewAngle (double viewAngle);
		void SetNearPlane (double nearPlane);
		void SetFarPlane (double farPlane);
		void SetScreenSize (int screenWidth, int screenHeight);

		Matrix GetProjectionMatrix();
		Matrix GetModelViewMatrix();

		void RotateV(double angle);
		void RotateU(double angle);
		void RotateW(double angle);

		Matrix RotateArbVec(double angle, Vector a);
		Matrix RotateWforV(double angle, Vector a);

		Vector getV();
		Vector getU();


		void Rotate(Point p, Vector axis, double degree);
		void Translate(const Vector &v);

		Point GetEyePoint();
		Vector GetLookVector();
		Vector GetUpVector();
		double GetViewAngle();
		double GetNearPlane();
		double GetFarPlane();
		int GetScreenWidth();
		int GetScreenHeight();

		double GetFilmPlanDepth();
		double GetScreenWidthRatio();

	private:

		Matrix mvM;
		Matrix pjM;

		Matrix M1, M2, M3, M4;

		Vector u, v, w;

		Point eyePoint;
		Vector lookVector;
		Vector upVector;

		// Don't need all three of these
		double viewAngle;
		double viewHeightAngle;

		double nearPlane;
		double farPlane;

		double screenWidth;
		double screenHeight;

		double filmPlanDepth;
		double screenWidthRatio;

};
#endif
