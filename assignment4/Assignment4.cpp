#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glui.h>
#include <math.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

using namespace std;

struct objNode {
	PrimitiveType type;
	Matrix M;
	SceneMaterial material;
	objNode* next;
};

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 30;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;


double windowXSize = 500;
double windowYSize = 500;



void storeObj(PrimitiveType n_type, Matrix curMat, SceneMaterial n_material);
void addObject(SceneNode* node, Matrix curMat);
double getShapeSpecIntersect(objNode* iter, Vector ray, int x, int y);
void putPixel(int i, int j, double smallest_t, Vector norm, objNode* obj, Point worldcord);
Vector getShapeSpecNormal(objNode* iter, Vector ray, double t);

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/general/robotS.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;

int pixelWidth = 500, pixelHeight = 500;
int screenWidth = 500, screenHeight = 500;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

objNode* head;
objNode* tail;



void createObjList(SceneNode* root) {

	Matrix M = Matrix();

	head = NULL;
	tail = NULL;

	addObject(root, M);

}




void addObject(SceneNode* node, Matrix curMat) {

	if (node == NULL) {
		return;
	}

	Matrix m_T = Matrix();
	Matrix m_S = Matrix();
	Matrix m_R = Matrix();
	Matrix m_Z = Matrix();

	Matrix newM = Matrix();

	int transforms = 0;
	Vector v;
	transforms = node->transformations.size();

	for(int i = 0; i < transforms; i++) {
		TransformationType type = node->transformations[i]->type;


		if (type == TRANSFORMATION_TRANSLATE){

			v = node->transformations[i]->translate;
			m_T = trans_mat(v);
			newM = newM * m_T;

		} else if (type == TRANSFORMATION_SCALE) {
			v = node->transformations[i]->scale;
			m_S = scale_mat(v);
			newM = newM * m_S;

		} else if (type == TRANSFORMATION_ROTATE) {
			v = node->transformations[i]->rotate;
			float angle = node->transformations[i]->angle;

			m_R = rot_mat(v, angle);
			newM = newM * m_R;

		} else if (type == TRANSFORMATION_MATRIX) {

			m_Z = node->transformations[i]->matrix;
			newM = newM * m_Z;
		}	

	}

	//newM = m_Z * m_T * m_R * m_S;

	curMat = curMat * newM;

	int primitives = node->primitives.size();

	for(int i = 0; i < primitives; i++) {
		
		storeObj(node->primitives[i]->type, curMat, node->primitives[i]->material);
	}

	int size = node->children.size();

	for(int i=0; i < size; i++){
   		addObject(node->children[i], curMat);
	}
}

void storeObj(PrimitiveType n_type, Matrix curMat, SceneMaterial n_material) {

	objNode* newNode = new objNode();

	newNode -> type = n_type;
	newNode -> M = curMat;
	newNode -> material = n_material;
	newNode -> next = NULL;

	if (head == NULL) {
		head = newNode;
		tail = newNode;
	} else {
		tail -> next = newNode;
		tail = newNode;
	}
}



void setupCamera();
void updateCamera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

Vector generateRay(int pixelX, int pixelY) {

	double a, b, w, h;
	Point q, s;
	Vector UVec, VVec, ray;
	double n = camera->GetNearPlane();
	double ratio = camera->GetScreenWidthRatio();
	double angle = camera->GetViewAngle();

	h = n * tan(angle/2);
	w = h * ratio;

	a = -w + (2*w)*((double)pixelX/(double)windowXSize);
	b = h - (2*h)*((double)pixelY/(double)windowYSize);
	q = camera->GetEyePoint() + n*camera->GetLookVector();

	UVec = camera->getU();
	VVec = camera->getV();


	s = q + a*UVec + b*VVec;

	ray = s - camera->GetEyePoint();

	ray.normalize();

	// printf("x,y: %d %d\n", pixelX, pixelY);
	// printf("Cast Ray: %f %f %f\n", ray[0], ray[1], ray[2]);


	return ray;
}

void callback_start(int id) {


	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	objNode* iter;
	Vector ray;
	Point worldcord;
	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {


			ray = generateRay(i, j);

			iter = head;
			double t = -1.0;
			double smallest_t = -1.0;
			Vector norm;
			objNode* theObj = NULL;

			while (iter != NULL) {

				t = getShapeSpecIntersect(iter, ray, i, j);

				if (((smallest_t == -1.0) && (t > 0)) || ((t > 0) && (t < smallest_t))) {
					smallest_t = t;
					norm = getShapeSpecNormal(iter, ray, smallest_t);
					theObj = iter;
					
					worldcord = camera->GetEyePoint() + smallest_t * ray;
					worldcord = iter->M * worldcord;
				}

				iter = iter->next;
			}


			worldcord = camera->GetEyePoint() + smallest_t * ray;
			putPixel(i, windowYSize - j, smallest_t, norm, theObj, worldcord);

		}
	}
	glutPostRedisplay();
}


double getShapeSpecIntersect(objNode* iter, Vector ray, int x, int y){

	Point ep = camera->GetEyePoint();
	Matrix M = iter->M; // Transformation FROM object TO world

	Matrix inv = invert(M);

	Point ep_obj = inv * ep;
	Vector ray_obj = inv * ray;

	double t;

	if (iter->type == SHAPE_CUBE) {

		t = cube->Intersect(ep_obj, ray_obj, M);
	}
	else if (iter->type == SHAPE_CYLINDER) {

		t = cylinder->Intersect(ep_obj, ray_obj, M);
	}
	else if (iter->type == SHAPE_CONE){

		t = cone->Intersect(ep_obj, ray_obj, M);

	}else if (iter->type == SHAPE_SPHERE){

		t = sphere->Intersect(ep_obj, ray_obj, M);

	} else {

		// Should never get here.
		t = -1.0;
	}

	// Point p_obj = ep_obj + ray_obj * t;
	// Point p_world = M * p_obj;
	// Vector dist_world = p_world - ep;
	// double toReturn = dist_world.length();
	// //return toReturn;
	return t;

}

Vector getShapeSpecNormal(objNode* iter, Vector ray, double t){
	Point ep = camera->GetEyePoint();
	Matrix m = iter->M; // Transformation FROM object TO world

	Matrix inv = invert(m); //

	Point ep_obj = inv * ep;
	Vector ray_obj = inv * ray;

	Matrix hgn = transpose(inv); // transpose of the inverst of W -> O
	Vector obj_normal, world_normal;


	if (iter->type == SHAPE_CUBE) {

		obj_normal = cube->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_CYLINDER) {

		obj_normal = cylinder->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_CONE){

		obj_normal =  cone->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_SPHERE){

		obj_normal = sphere->findIsectNormal(ep_obj, ray_obj, t);
	} 
	else {
		return Vector();
	}

	world_normal = hgn * obj_normal;
	world_normal.normalize();
	return world_normal;
	
}

void putPixel(int i, int j, double smallest_t, Vector norm, objNode* obj, Point worldcord) {
	
	if (smallest_t == -1.0) {
		setPixel(pixels, i, j, 0, 0, 0);
		return;
	} else  {

	}

	int numLights = parser->getNumLights();
	double red, green, blue, redA, greenA, blueA, redD, greenD, blueD, sumR, sumG, sumB;
	SceneLightData data;
	SceneGlobalData global_data;
	SceneColor color;
	double dotProd;
	int redInt, greenInt, blueInt;
	Vector Lm;

	parser->getGlobalData(global_data);

	float ka = global_data.ka;
	float kd = global_data.kd;

	redA = obj->material.cAmbient.r;
	greenA = obj->material.cAmbient.g;
	blueA = obj->material.cAmbient.b;

	redD = obj->material.cDiffuse.r;
	greenD = obj->material.cDiffuse.g;
	blueD = obj->material.cDiffuse.b;


	sumR = 0.0;
	sumB = 0.0;
	sumG = 0.0;

	for (int m = 0; m < numLights; m++) {
		parser->getLightData(m, data);

		color = data.color;

		if (data.type == LIGHT_POINT){
			Lm = data.pos - worldcord;
			Lm.normalize();
		} else if (data.type == LIGHT_DIRECTIONAL) {
			Lm = data.dir;
			Lm.normalize();
		} else { 
			// ERROR 
		}

		dotProd = dot(norm, Lm);

		if (dotProd < 0 ) {
			dotProd = 0;
		}

		sumR += kd * redD * color.r * dotProd;
		sumG += kd * greenD * color.g * dotProd;
		sumB += kd * blueD * color.b * dotProd;

	}

	red = ka * redA + sumR;
	blue = ka * blueA + sumB;
	green = ka * greenA + sumG;

	redInt = round(255*red);
	greenInt = round(255*green);
	blueInt = round(255*blue);

	if (redInt > 255){
		redInt = 255;
	}
	if (greenInt > 255){
		greenInt = 255;
	}
	if (blueInt > 255){
		blueInt = 255;
	}


	// redInt = round(red);
	// greenInt = round(green);
	// blueInt = round(blue);


	//printf("pixel, (%d, %d): r: %d g: %d b:%d \n", i, j, redInt, greenInt, blueInt );
	setPixel(pixels, i, j, redInt, greenInt, blueInt);

}


void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	SceneNode* root = parser->getRootNode();
	createObjList(root);
	setupCamera();
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);
	camera->SetViewAngle(viewAngle);
	Vector upV = camera->GetUpVector();
	camera->Orient(guiEye, guiLook, upV);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);
	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



