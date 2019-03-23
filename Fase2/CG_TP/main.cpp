#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Library includes
#include <math.h>
#include <stdio.h>
#include <vector>
#include <tuple>

// Personal includes
#include "parser.h"
#include "extractor.h"

using namespace std;

typedef tuple<double, double, double> coords;

// Global variables
float alpha = 1, beta = 1, radius = 50;
double red = 0.6, green = 0.8, blue = 1;

vector<First*> files;

// Coords of all the triangles
vector<coords> triangles;


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

bool checkEnding(coords firstTriangle, coords secondTriangle, coords thirdTriangle)
{
    coords end = make_tuple(12345, 12345, 12345);

    return (firstTriangle == end && secondTriangle == end && thirdTriangle == end);
}


void drawTriangles(void)
{
    bool flag;
    vector<coords>::iterator i;
    vector<First*>::iterator iFirst;
    vector<Second*>::iterator iSecond;

    i = triangles.begin();
    iFirst = files.begin();

    while (i != triangles.end()) {

        flag = true;
        First* first = *iFirst;

        iSecond = first->transfs.begin();
        glPushMatrix();

        while (iSecond != first->transfs.end()) {

            Second* op = *iSecond;

            if (strcmp(op->transf, "scale") == 0)
                glScalef(op->params[0], op->params[1], op->params[2]);

            if (strcmp(op->transf, "rotate") == 0)
                glRotatef(op->params[3], op->params[0], op->params[1], op->params[2]);

            if (strcmp(op->transf, "translate") == 0)
                glTranslatef(op->params[0], op->params[1], op->params[2]);

            if (strcmp(op->transf, "color") == 0)
                glColor3f(op->params[0], op->params[1], op->params[2]);

            iSecond++;

        }

        while (i != triangles.end() && flag != false) {

            coords one = *i;
            i++;
            coords two = *i;
            i++;
            coords three = *i;
            i++;

            if (!checkEnding(one, two, three)) {

                glBegin(GL_TRIANGLES);

                glVertex3d(get<0>(one), get<1>(one), get<2>(one));
                glVertex3d(get<0>(two), get<1>(two), get<2>(two));
                glVertex3d(get<0>(three), get<1>(three), get<2>(three));

                glEnd();

            }
            else
                flag = false;
        }

        glPopMatrix();
        glColor3f(red, green, blue);
        iFirst++;

    }
}

void drawOrbits()
{
    int sides = 80;
    int a;
    double heading;

    glColor3f(0.2, 0.2, 0.2);

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 130 - 210, 0, sin(heading) * 130);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 145 - 210, 0, sin(heading) * 145);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 165 - 210, 0, sin(heading) * 165);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 185 - 210, 0, sin(heading) * 185);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 250 - 210, 0, sin(heading) * 250);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 320 - 210, 0, sin(heading) * 320);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 380 - 210, 0, sin(heading) * 380);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 415 - 210, 0, sin(heading) * 415);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (a = 0; a < 360; a += 360 / sides) {
        heading = a * M_PI / 180;
        glVertex3f(cos(heading) * 450 - 210, 0, sin(heading) * 450);
    }
    glEnd();

}

void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

    gluLookAt(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha),
              0.0, 2.0, 0.0,
              0.0f, 1.0f, 0.0f);

    // put drawing instructions here
    drawTriangles();

    drawOrbits();

	// End of frame
	glutSwapBuffers();
}

// Camera zoom
void processKeys(unsigned char key, int xx, int yy)
{

    // put code to process regular keys in here
    if(key == 'w') radius-=1;
    if(key == 's') radius+=1;

    glutPostRedisplay();

}

// Camera movement
void processSpecialKeys(int key, int xx, int yy)
{
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha += 0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            alpha -= 0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if(beta < M_PI/2)
                beta += 0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if(beta > -M_PI/2)
                beta -= 0.05;
            glutPostRedisplay();
            break;
        default:
            cout << "Command not recognized!" << "\n";
    }
}

int main(int argc, char** argv)
{
    parser("../config.xml", files);

    extractor(files, triangles);

    //debug(files);

    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CGTP - Fase 2");
		
    // Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // enter GLUT's main cycle
	glutMainLoop();

	return 1;

}
