#include "Model.h"
#include "Camera.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <memory>

#include "../communication/ViewerClient.h"

GLuint model_list;
Model model_object;
Camera camera_object;
int winid;

std::shared_ptr<CanTouchThis::ViewerClient> clt_ptr;

void Model::load_model(std::string model) {
	std::ofstream intermediate("im.obj");
	intermediate << model;
	intermediate.close();

	std::ifstream fileObject("im.obj");
	std::string fileObjectLine;

	model_list = glGenLists(1);
	glNewList(model_list, GL_COMPILE);

	if (fileObject.is_open()) {
		while (! fileObject.eof() ){
			getline(fileObject, fileObjectLine);
			if (fileObjectLine.c_str()[0] == 'v') {
				float x, y, z;
				fileObjectLine[0] = ' ';
				sscanf(fileObjectLine.c_str(), "%f %f %f ", &x, &y, &z);
				model_object.vertex_list.push_back(x);
				model_object.vertex_list.push_back(y);
				model_object.vertex_list.push_back(z);
				continue;
			}
		}

		model_object.apply_transf_to_matrix();

		fileObject.clear();
		fileObject.seekg(0, std::ios::beg);

		while(!fileObject.eof()){
			getline(fileObject, fileObjectLine);
			
			if (fileObjectLine.c_str()[0] == 'f') {
				s_list.push_back(fileObjectLine);
				fileObjectLine[0] = ' ';

				std::istringstream iss(fileObjectLine);

				glBegin(GL_POLYGON);
				while(iss){
					int value;
					iss >> value;
					if (iss.fail()) break;
					glVertex3f(	model_object.vertex_list.at(3 * (value - 1) + 0),
								model_object.vertex_list.at(3 * (value - 1) + 1),
								model_object.vertex_list.at(3 * (value - 1) + 2));
				}
				glEnd();
			}			
		}		
	}
	glEndList();
}

void draw_model_transform() {
	glTranslatef(model_object.model_x, model_object.model_y, model_object.model_z);
	glTranslatef(0,0,-2);
	glRotatef(model_object.model_rotx,1,0,0);
	glRotatef(model_object.model_roty,0,1,0);
	glRotatef(model_object.model_rotz,0,0,1);
	glTranslatef(0,0,2);
}

void draw_camera_transform(){
	glTranslatef(camera_object.camera_x, camera_object.camera_y, camera_object.camera_z);
	glRotatef(camera_object.camera_rotx,1,0,0);
	glRotatef(camera_object.camera_roty,0,1,0);
	glRotatef(camera_object.camera_rotz,0,0,1);
}

void draw_scene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_FOG);
	glHint(GL_FOG_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	draw_camera_transform();
	draw_model_transform();

	glCallList(model_list);

	glutSwapBuffers();
}

void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void kb_input(unsigned char key, int x, int y) {
	std::string obj("");
	switch(key) {
		case 'w':
			model_object.model_rotx -= 10;
			if (model_object.model_rotx < 0)
				model_object.model_rotx = 350;
			glutPostRedisplay();
			break;
		case 's':
			model_object.model_rotx += 10;
			if (model_object.model_rotx > 360)
				model_object.model_rotx = 10;
			glutPostRedisplay();
			break;
		case 'a':
			model_object.model_roty -= 10;
			if (model_object.model_roty < 0)
				model_object.model_roty = 350;
			glutPostRedisplay();
			break;
		case 'd':
			model_object.model_roty += 10;
			if (model_object.model_roty > 360)
				model_object.model_roty = 10;
			glutPostRedisplay();
			break;
		case 'e':
			model_object.reset();
			camera_object.reset();
			glutPostRedisplay();
			break;
		case 'r':
			clt_ptr->getObjFile(obj);
			model_object.clear();
			model_object.load_model(obj);
			break;
		case 'q':
			exit(0);
			break;
		default: break;
	}
}

void Loop(int a) {
	std::cout << "Refreshing\n";
	glutTimerFunc(1000, Loop, 1);
	// glutDestroyWindow(winid);
	// glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// winid = glutCreateWindow("Viewer");


	// glClearColor(0.0, 0.0, 0.0, 0.0);
	// glutDisplayFunc(draw_scene); 
	// glutReshapeFunc(resize);  
	// glutKeyboardFunc(kb_input);

	// glutMainLoop();
}

int main(int argc, char **argv) {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(300, 300);
	winid = glutCreateWindow("Viewer");
	// glutTimerFunc(1000, Loop, 1);

	std::string obj("");
	clt_ptr.reset(new CanTouchThis::ViewerClient(CanTouchThis::LOCALHOST, CanTouchThis::PORT));

	while(true) {
		clt_ptr->getObjFile(obj);
		model_object.load_model(obj);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glutDisplayFunc(draw_scene);
		glutReshapeFunc(resize);
		glutKeyboardFunc(kb_input);
		glutMainLoop();
	}

	return 0;
}