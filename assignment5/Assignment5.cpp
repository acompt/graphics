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

#define eps (0.000001)


using namespace std;

struct objNode {
	PrimitiveType type;
	Matrix M;
	SceneMaterial material;
	objNode* next;
};

struct rgbf {
	double rf;
	double gf;
	double bf;
};

struct rgbi {
	int r;
	int g;
	int b;
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

int noRec = 1;
int specC = 100;
double arb_dist = 200;

double fix(double d);

bool notClear(Point me, Point light);
rgbf getColor(Point orig, Vector ray, int recLeft);
static bool isEqual(double i, double j);
void storeObj(PrimitiveType n_type, Matrix curMat, SceneMaterial n_material);
void addObject(SceneNode* node, Matrix curMat);
double getShapeSpecIntersect(Point orig, objNode* iter, Vector ray);
void putPixel(int i, int j, double smallest_t, Vector norm, objNode* obj, Point worldcord);
Vector getShapeSpecNormal(Point orig, objNode* iter, Vector ray, double t);
Point getTexture(objNode* iter, Point orig);
void getPPMFile(string file); 

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "./shinyballs.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;

int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;
vector<rgbi> ppmFile;
string filename = NULL;
int textwidth = 0;
int textheight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

objNode* head;
objNode* tail;

static bool isEqual(double i, double j) {

	if (fabsf(i - j) < eps) {
		return true;
	} else {
		return false;
	}
}

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

	int pm = node->primitives.size();

	for(int i = 0; i < pm; i++) {
		
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

	a = -w + (2*w)*((double)pixelX/(double)pixelWidth);
	b = h - (2*h)*((double)pixelY/(double)pixelHeight);
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

	// cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	Vector ray;

	rgbf rgbFinal;
	int redInt, greenInt, blueInt;

	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {

			ray = generateRay(i, j);

			rgbFinal = getColor(camera->GetEyePoint(), ray, noRec);

			redInt = round(255.0 * rgbFinal.rf);
			greenInt = round(255.0 * rgbFinal.gf);
			blueInt = round(255.0 * rgbFinal.bf);

			if (redInt > 255){
				redInt = 255;
			}
			if (greenInt > 255){
				greenInt = 255;
			}
			if (blueInt > 255){
				blueInt = 255;
			}

			setPixel(pixels, i, pixelHeight - j - 1, redInt, greenInt, blueInt);
		}
	}
	glutPostRedisplay();
}


rgbf getColor(Point orig, Vector ray, int recLeft) {

	SceneLightData data;
	SceneGlobalData global_data;
	SceneColor color;
	SceneFileMap* texture;

	double red, green, blue, redA, greenA, blueA, redD, greenD, blueD;
	double redS, greenS, blueS, redR, greenR, blueR, sumR, sumG, sumB;
	double dotProd_d, dotProd_s;
	double u, v, s, t1;
	Vector norm, Lm, Ri, Vhat;
	Point worldcord;

	int numLights = parser->getNumLights();
	objNode* iter = head;
	double t = -1.0;
	double smallest_t = -1.0;
	objNode* theObj = NULL;

	rgbf toR, toAdd;

	toR.rf = 0.0;
	toR.gf = 0.0;
	toR.bf = 0.0;

	if (recLeft == -1) {
		// if (orig == camera->GetEyePoint()){
		// 	printf("THE FUCK\n");
		// }
		// toR.rf = 1.0;
		return toR;
	}


	while (iter != NULL) {

		t = getShapeSpecIntersect(orig, iter, ray);

		if ((isEqual(smallest_t, -1.0) && (t > 0)) || ((t > 0) && (t < smallest_t))) {
			smallest_t = t;
			theObj = iter;

		}
		iter = iter->next;
	}

	if (theObj == NULL) {
		// toR.gf = 1.0;
		return toR;
	}

	norm = getShapeSpecNormal(orig, theObj, ray, smallest_t);

	worldcord = orig + smallest_t * ray;
	Point thanksRemco = worldcord + norm * eps;
	
	if (isEqual(smallest_t, -1.0)) {

		return toR;
	}


	parser->getGlobalData(global_data);

	float ka = global_data.ka;
	float kd = global_data.kd;
	float ks = global_data.ks;

	texture = theObj->material.textureMap;

	if(texture != NULL && texture->isUsed) {
		rgbf textR;

		textR.rf = 0.0;
		textR.gf = 0.0;
		textR.bf = 0.0;

		u = texture -> repeatU;
		v = texture -> repeatV;

		Point text = getTexture(theObj, orig);

		// Find out which block our point is in
		int u_block = floor(text[0] * u);
		int v_block = floor(text[1] * v);

		// Find out the width and height of SMALL blocks in terms of unit square
		double b_wid = 1.0 / float(u);
		double b_hei = 1.0 / float(v);

		// Find out x and y in single square in terms of unit square
		double b_x = text[0] - u_block * b_wid;
		double b_y = text[1] = v_block * b_hei;

		// Find s, t between 0.0 and 1.0, x and y values FOR ONE IMAGE.
		// Mult these by pixel width and height from image to get color.
		s = b_x / b_wid;
		t1 = b_y / b_hei;

		if(filename != texture->filename) {
			//getPPMFile(filename);
		}

		int w, h;

		w = textwidth * s;

		h = textheight* t1;			

		textR.rf = ppmFile[w*h].r;
		textR.gf = ppmFile[w*h].g;
		textR.bf = ppmFile[w*h].b;

		return textR;
	}

	redA = theObj->material.cAmbient.r;
	greenA = theObj->material.cAmbient.g;
	blueA = theObj->material.cAmbient.b;

	redD = theObj->material.cDiffuse.r;
	greenD = theObj->material.cDiffuse.g;
	blueD = theObj->material.cDiffuse.b;

	redS = theObj->material.cSpecular.r;
	greenS = theObj->material.cSpecular.g;
	blueS = theObj->material.cSpecular.b;

	redR = theObj->material.cReflective.r;
	greenR = theObj->material.cReflective.g;
	blueR = theObj->material.cReflective.b;

	sumR = 0.0;
	sumB = 0.0;
	sumG = 0.0;

	for (int m = 0; m < numLights; m++) {
		parser->getLightData(m, data);

		color = data.color;

		if (data.type == LIGHT_POINT){
			Lm = data.pos - worldcord;
			Lm.normalize();
			if (notClear(thanksRemco, data.pos)){
				continue;
			}
		} else if (data.type == LIGHT_DIRECTIONAL) {
			// We do enter this loop
			Lm = (-1) * data.dir;
			// This vector is right
			Lm.normalize();
			if (notClear(thanksRemco, thanksRemco + arb_dist * Lm)){
				continue;
			}
		}

		// Lm points FROM object TO light

		dotProd_d = dot(norm, Lm);

		if (dotProd_d < 0 ) {
			dotProd_d = 0;
		}


		Ri = Lm - 2 * (dotProd_d) * norm;
		Ri.normalize();

		Vhat = worldcord - orig;
		Vhat.normalize();

		dotProd_s = dot(Ri, Vhat);
		if (dotProd_s < 0 ) {
			dotProd_s = 0;
		}

		sumR += kd * redD * color.r * dotProd_d + ks * redS * pow(dotProd_s, specC);
		sumG += kd * greenD * color.g * dotProd_d + ks * greenS * pow(dotProd_s, specC);
		sumB += kd * blueD * color.b * dotProd_d + ks * blueS * pow(dotProd_s, specC);

	}

	red = ka * redA + sumR;
	green = ka * greenA + sumG;
	blue = ka * blueA + sumB;




	// TODO: CHECK TO SEE IF THESE VALUES ARE VERY LOW, IF
	// SO, DON'T BOTHER RECURSING FARTHER.


	double dp = dot(ray, norm);



	Vector nRay = ray - 2 * dp * norm;
	nRay.normalize();


	toAdd = getColor(thanksRemco, nRay, recLeft - 1);

	toR.rf = fix(red + ks * redR * toAdd.rf);
	toR.gf = fix(green + ks * greenR * toAdd.gf);
	toR.bf = fix(blue + ks * blueR * toAdd.bf);

	return toR;
}

bool notClear(Point me, Point light){

	objNode* iter = head;
	objNode* theObj = NULL;
	double t;

	Vector dir = me - light;
	double dist = dir.length();
	dir.normalize();

	while (iter != NULL) {

		// t is the dist from the light to cur object
		t = getShapeSpecIntersect(light, iter, dir);
		if (isEqual(t, -1.0)){
			iter = iter->next;
			continue;
		}

		if (t < dist){
			return true;
		}
		iter = iter->next;
	}

	return false;
}

// void getPPMFile(string file) {

// 	vector<rgbi> newFile;
// 	string s, crap;
// 	int r, g, b;

// 	ifstream f(file.c_str(), ios::binary);
//     if (f.fail())
//     {
//         cout << "Could not open file: " << file << endl;
//         return;
//     }

//     f >> s >> crap >> textwidth >> textheight;
	
// 	while (f >> r >> g >> b){
// 		rgbi pixel;
// 		pixel.r = r;
// 		pixel.g = g;
// 		pixel.b = b;
// 	    newFile.push_back(pixel);
// 	}
// 	f.close();
// 	ppmFile = newFile;
// }

double fix(double d){
	if (d > 1.0){
		return 1.0;
	}
	if (d < 0.0){
		return 0.0;
	}
	return d;
}

Point getTexture(objNode* iter, Point orig) {

	Point toR;
	Point pnt = orig;
	Matrix M = iter->M; // Transformation FROM object TO world

	Matrix inv = invert(M);

	Point pnt_obj = inv * pnt;

	if (iter->type == SHAPE_CUBE) {
		toR = cube->getTextureMap(pnt_obj);
	}
	else if (iter->type == SHAPE_CYLINDER) {
		toR = cylinder->getTextureMap(pnt_obj);
	}
	else if (iter->type == SHAPE_CONE){
		toR = cone->getTextureMap(pnt_obj);
	}
	else if (iter->type == SHAPE_SPHERE){
		toR = sphere->getTextureMap(pnt_obj);
	}
	return toR;
}

double getShapeSpecIntersect(Point orig, objNode* iter, Vector ray){

	Point ep = orig;
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

Vector getShapeSpecNormal(Point orig, objNode* iter, Vector ray, double t){

	Point ep = orig;
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
	camera->SetViewAngle(DEG_TO_RAD(viewAngle));

	Vector upV = camera->GetUpVector();
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
	camera->Orient(guiEye, guiLook, upV);

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

	objNode* temp = head;
	while (head != NULL){
		temp = head->next;
		delete head;
		head = temp;
	}


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

	main_window = glutCreateWindow("COMP 175 Assignment 5");
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

	GLUI_Panel *rec_panel = glui->add_panel("Assignment 5");
	(new GLUI_Spinner(rec_panel, "Recurse:", &noRec))
		->set_int_limits(0, 4);
	(new GLUI_Spinner(rec_panel, "Spec Const:", &specC))
		->set_int_limits(1, 500);
	
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



