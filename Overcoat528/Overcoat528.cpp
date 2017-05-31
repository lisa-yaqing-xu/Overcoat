// Overcoat528.cpp : Defines the entry point for the console application.
//
/*
Used https://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html 's 
simple Glut example as a jumping off point for the glut rendering structure
*/
#pragma once
#include "stdafx.h"
#include "data.h"
#include "fcn.h"
#include "octree.h"
#include "colors.h"

int LEFT_KEY = 0;
int RIGHT_KEY = 2;

int LEVEL_SET = 0;
int HAIR = 1;
int FEATHER = 2;
int SCULPT = 3;

int mousetype = LEFT_KEY;
int tooltype = LEVEL_SET;

int curr_mx = 0, curr_my = 0;

bool rot = false;

double tempx = 0;
double tempy = 0;
glm::vec3 dis;

ProxyMesh proxmesh;
//Octree octree(0, 0, 0, 5, int(16));
Camera cam(1, 1, 5);

bool showlastcam = false;
glm::vec3 camshow = getCurrCamPos();

bool drawvec = false;
vector<glm::vec3> testvec;
vector<Stroke> strokevec;

GLubyte *brushimg;

void drawTestLine(float magn, Colorf color){
	color.callGlColor3f();
	/*glBegin(GL_LINES);

	for (int i = 1; i <testvec.size(); i ++){
		glm::vec3 v = testvec[i-1];
		glVertex3f(v[0], v[1], v[2]);
		//cout << a << " " << b << " " << c << endl;	
		glm::vec3 w = testvec[i];
		glVertex3f(w[0], w[1], w[2]);
	}

	glEnd();*/
	glm::vec3 camv = getCurrCamPos();
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble mv[16], pm[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, pm);
	for (int i = 0; i <strokevec.size(); i++){
		strokevec[i].drawVertices(camv,mv,pm,viewport);
	}
}
//Resizing function
void onResize(int w, int h)
{
	// prevent divide/0 
	if (h == 0)
		h = 1;

	//set drawing region
	glViewport(0, 0, w, h);

	// projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// perspective projection
	gluPerspective(60, (float)w / h, 0.1, 100);

	// model view matrix
	glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------	Draw()
//
void onDraw() {

	// clear the screen & depth buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// clear the previous transform
	glLoadIdentity();
	
	//cout << light_pos[0] << endl;
	gluLookAt(cam.x, cam.y, cam.z,	//	eye pos
		0, 0, 0,	//	aim point
		0, 1, 0);	//	up direction
	glRotatef(RotZ, 1, 0, 0);
	glRotatef(-RotX, 0, 0, 1);
	glScalef(magnification, magnification, magnification);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 
	displayLoadedMesh(&proxmesh, magnification,COL_CUSTOM);
	drawTestLine(magnification, COL_RED);
	if (showlastcam){
		COL_WHITE.callGlColor3f();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(camshow.x, camshow.y, camshow.z);
		glEnd();
	}
	glutSwapBuffers();
}

void onMouse(int button, int state, int x,int y){
	mousetype = button;	
	//cout << tempx << "\t" << tempy << endl;
	if (mousetype == GLUT_RIGHT_BUTTON){
		curr_mx = x;
		curr_my = y;
		rot = true;
	}
	else{
		tempx = x;
		tempy = y;
		rot = false;
	}

	if (mousetype == GLUT_LEFT_BUTTON){
		if (drawvec){
			drawvec = false;
		}
		else{
			if (tooltype == SCULPT){
				sculpts.push_back(SculptStroke());
				drawvec = true;
			}
			else{
				strokevec.push_back(Stroke());
				strokevec.back().type = tooltype;
				drawvec = true;
			}

		}
	}
}

void onMouseMove(int x, int y){

	if (rot){
		RotX += (float)(x - curr_mx)*0.5f;
		RotZ += (float)(y - curr_my)*0.5f;
		curr_mx = x;
		curr_my = y;

		glutPostRedisplay();
	}
	else {//if (mousetype == GLUT_LEFT_BUTTON){
		glm::vec3 camv = getCurrCamPos();
		glm::vec3 v = getViewVector(x, y);
		//float t = getT(camv, v, &proxmesh)-(level*_levelmod);
		//cout << t << endl;
		if (tooltype == SCULPT){
			sculpt(&sculpts.back(), &proxmesh, tooltype, x, y);
		}
		else{
			optimize(&strokevec.back(), &proxmesh, tooltype, x, y);
		}
		
		//if (t > 0) strokevec.back().addVertex(glm::vec3(camv.x + v.x*t, camv.y + v.y*t, camv.z + v.z*t));
			//testvec.push_back(glm::vec3(camv.x + v.x*t, camv.y + v.y*t, camv.z + v.z*t));

		glutPostRedisplay();
	}
}


void init() {
	glClearColor(0.1, 0.1, 0.1, 0.0);
	glEnable(GL_DEPTH_TEST); 
}

void onExit() {
}

void dispHelpMenu(){
	cout << "Usage:\nL - Level Set \n"
		<< "H - Hair Tool \n"
		<< "F - Feather Tool \n"
		<< "S - Sculpt \n"
		<< "Q - Input Level Value\n"
		<< "W - Show Help Menu\n"
		<< "Right Click to Rotate"
		<< endl;
}
void toolSwitch(unsigned char key, int x, int y){
	if (key == 'q'){
		cout << "Please Set the Level" << endl;
		cin >> level;
		cout << "Current Level: " << level << endl;
	}
	else if (key == 'w'){
		dispHelpMenu();
	}
	else if (key == 't'){ 
		if (showlastcam){
			showlastcam = false;
			glutPostRedisplay();
		}
		else{
			showlastcam = true;
			camshow = getCurrCamPos();
			dis = getViewVector(tempx, tempy);
			glutPostRedisplay();
		}
		
	}
	else if (key == 'y'){ //debug code. resets rotation
		RotZ = 0.0;
		RotX = 0.0;
		glutPostRedisplay();
	}
	else if (key == 'l'){
		tooltype = LEVEL_SET;
		cout << "Current Tool: Level Set"<< endl;
	}
	else if (key == 'h'){
		tooltype = HAIR;
		cout << "Current Tool: Hair" << endl;
	}
	else if (key == 'f'){
		tooltype = FEATHER;
		cout << "Current Tool: Feather" << endl;
	}
	else if (key == 's'){
		tooltype = SCULPT;
		cout << "Current Tool: Sculpt" << endl;
	}
}


//------------------------------------------------------------	main()
//
int main(int argc, char** argv) {
	char* filename = new char[128];
	cout << "Enter a .obj file to load" << endl;
	cin >> filename;
	
	loadFile(filename, &proxmesh);
	//loadFile("sphere.obj", &proxmesh);// &octree);
	dispHelpMenu();

	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);


	glutInitWindowSize(640, 480);


	glutCreateWindow("Overcoat");


	glutDisplayFunc(onDraw);

	glutReshapeFunc(onResize);

	glutKeyboardFunc(toolSwitch);

	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMove);
	init();

	atexit(onExit);

	glutMainLoop();

	return 0;
}
