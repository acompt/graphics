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

struct MatList {
	Matrix T;
	Matrix S;
	Matrix R;
	Matrix Z;
	Matrix M;
};

struct objNode {
	PrimitiveType type;
	MatList ml;
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



void storeObj(PrimitiveType n_type, MatList ml, SceneMaterial n_material);
void addObject(SceneNode* node, MatList curMat);
double getShapeSpecIntersect(objNode* iter, Vector ray, int x, int y);
void putPixel(int i, int j, double smallest_t, Vector norm, objNode* obj, Point worldcord);
Vector getShapeSpecNormal(objNode* iter, Vector ray, double t);

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/general/test.xml";
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

	MatList idMat;

	idMat.T = Matrix();
	idMat.S = Matrix();
	idMat.R = Matrix();
	idMat.Z = Matrix();
	idMat.M = Matrix();

	head = NULL;
	tail = NULL;

	addObject(root, idMat);

	// objNode* cur = head;

	// while (cur != NULL) {

	// 	printf("primitive no: %d", cur->type);
	// 	Matrix M = cur -> tMat;

	// 	printf("Matrix is:\n %f, %f, %f, %f\n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f, \n",
	// 					M[0], M[4], M[8], M[12],
	// 					M[1], M[5], M[9], M[13], 
	// 					M[2], M[6], M[10], M[14],
	// 					M[3], M[7], M[11], M[15]);

	// 	cur = cur -> next;
	// }
}


void addObject(SceneNode* node, MatList curMat) {

	if (node == NULL) {
		return;
	}

	Matrix transLM = Matrix();
	Matrix scaleM = Matrix();
	Matrix rotM = Matrix();
	Matrix transFM = Matrix();

	Matrix newM = Matrix();

	int transforms = 0;
	Vector v;
	transforms = node->transformations.size();

	for(int i = 0; i < transforms; i++) {
		TransformationType type = node->transformations[i]->type;


		if (type == TRANSFORMATION_TRANSLATE){

			v = node->transformations[i]->translate;
			transLM = trans_mat(v) * transLM;

		} else if (type == TRANSFORMATION_SCALE) {
			v = node->transformations[i]->scale;
			scaleM = scale_mat(v) * scaleM;

		} else if (type == TRANSFORMATION_ROTATE) {
			v = node->transformations[i]->rotate;
			float angle = node->transformations[i]->angle;

			rotM = rot_mat(v, angle) * rotM;

		} else if (type == TRANSFORMATION_MATRIX) {

			transFM = node->transformations[i]->matrix * transFM;
		}	

	}

	// curMat.S = scaleM * curMat.S;
	// curMat.R = rotM * curMat.R;
	// curMat.T = transLM * curMat.T;
	// curMat.Z = transFM * curMat.Z;

	newM = transFM * transLM * rotM * scaleM;

	curMat.M = curMat.M * newM;

	int primitives = node->primitives.size();

	for(int i = 0; i < primitives; i++) {
		
		storeObj(node->primitives[i]->type, curMat, node->primitives[i]->material);
	}

	int size = node->children.size();

	for(int i=0; i < size; i++){
   		addObject(node->children[i], curMat);
	}
}

void storeObj(PrimitiveType n_type, MatList n_ml, SceneMaterial n_material) {

	objNode* newNode = new objNode();

	newNode -> type = n_type;
	newNode -> ml = n_ml;
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
					worldcord = iter->ml.M * worldcord;
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
	MatList ml = iter->ml; // Transformation FROM object TO world

	//TODO add Z matrix
	//Matrix inv = invert(ml.S) * invert(ml.R) * invert(ml.T) * invert(ml.Z);

	Matrix inv = invert(ml.M);

	Point ep_obj = inv * ep;
	Vector ray_obj = inv * ray;

	// printf("x, y: %d, %d\n", x, y);
	// printf("Obj Ray: %f %f %f\n", ray_obj[0], ray_obj[1], ray_obj[2]);
	// printf("Wor Ray: %f %f %f\n", ray[0], ray[1], ray[2]);

	// printf("Wor EP: %f %f %f\n", ep[0], ep[1], ep[2]);
	// printf("Obj EP: %f %f %f\n", ep_obj[0], ep_obj[1], ep_obj[2]);

	// printf("Matrix is:\n %f, %f, %f, %f\n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f, \n \n",
	// 						ml.M[0], ml.M[4], ml.M[8], ml.M[12],
	// 						ml.M[1], ml.M[5], ml.M[9], ml.M[13], 
	// 						ml.M[2], ml.M[6], ml.M[10], ml.M[14],
	// 						ml.M[3], ml.M[7], ml.M[11], ml.M[15]);

	double t;

	if (iter->type == SHAPE_CUBE) {

		t = cube->Intersect(ep_obj, ray_obj, ml.M);
	}
	else if (iter->type == SHAPE_CYLINDER) {

		t = cylinder->Intersect(ep_obj, ray_obj, ml.M);
	}
	else if (iter->type == SHAPE_CONE){

		t = cone->Intersect(ep_obj, ray_obj, ml.M);

	}else if (iter->type == SHAPE_SPHERE){

		t = sphere->Intersect(ep_obj, ray_obj, ml.M);

	} else {

		// Should never get here.
		t = -1.0;
	}

	Point p_obj = ep_obj + ray_obj * t;
	Point p_world = ml.M * p_obj;
	Vector dist_world = p_world - ep;
	double toReturn = dist_world.length();
	//return toReturn;
	return t;

}

Vector getShapeSpecNormal(objNode* iter, Vector ray, double t){
	Point ep = camera->GetEyePoint();
	Matrix m = iter->ml.M; // Transformation FROM object TO world

	Matrix inv = invert(m);

	Point ep_obj = inv * ep;
	Vector ray_obj = inv * ray;

	Matrix hgn = transpose(m);
	Vector toR, toRR;


	if (iter->type == SHAPE_CUBE) {

		toR = cube->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_CYLINDER) {

		toR = cylinder->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_CONE){

		toR =  cone->findIsectNormal(ep_obj, ray_obj, t);
	}
	else if (iter->type == SHAPE_SPHERE){

		toR = sphere->findIsectNormal(ep_obj, ray_obj, t);
	} 
	else {
		return Vector();
		// ERROR MESSAGE?
	}

	toRR = hgn * toR;
	toRR.normalize();
	return toRR;
	//return Vector(1,0,0);
}

void putPixel(int i, int j, double smallest_t, Vector norm, objNode* obj, Point worldcord) {
	
	if (smallest_t == -1.0) {
		setPixel(pixels, i, j, 0, 0, 0);
		return;
	} else  {
		// printf("!!! HIT !!!\n");
		// Point ep = camera->GetEyePoint();
		// Vector look = camera->GetLookVector();
		// Vector up = camera->GetUpVector();
		// printf("EP: %f %f %f\n", ep[0], ep[1], ep[2]);
		// printf("Look: %f %f %f\n", look[0], look[1], look[2]);
		// printf("Up: %f %f %f\n", up[0], up[1], up[2]);
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
			Lm = worldcord - data.pos;
			Lm.normalize();
		} else if (data.type == LIGHT_DIRECTIONAL) {
			Lm = data.dir;
			Lm.normalize();
		} else { 
			// ERROR 
		}

		dotProd = dot(norm, Lm);

		sumR += kd * redD * color.r * dotProd;
		sumG += kd * greenD * color.g * dotProd;
		sumB += kd * blueD * color.b * dotProd;

	}

	red = ka * redA + sumR;
	blue = ka * blueA + sumB;
	green = ka * greenA + sumG;

	// TODO CHECK THIS. PROB DON'T NEED 255
	redInt = round(255*red);
	greenInt = round(255*green);
	blueInt = round(255*blue);

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



