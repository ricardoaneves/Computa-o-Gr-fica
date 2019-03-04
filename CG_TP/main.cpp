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
float alpha = 1, beta = 1, radius = 20;
double red = 0.6, green = 0.8, blue = 1;

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


void drawTriangles(void)
{
    vector<coords>::iterator i;
    i = triangles.begin();

    while (i != triangles.end()) {

        coords point1 = *i;
        i++;
        coords point2 = *i;
        i++;
        coords point3 = *i;
        i++;

        glBegin(GL_TRIANGLES);
            glColor3f(red, green, blue);
            glVertex3d(get<0>(point1), get<1>(point1), get<2>(point1));
            glVertex3d(get<0>(point2), get<1>(point2), get<2>(point2));
            glVertex3d(get<0>(point3), get<1>(point3), get<2>(point3));
 		glEnd();
    }
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
    char** files = parser("../config.xml");
    if (files == nullptr)
        cout << "Error parsing the file!" << "\n";
    else
        extractor(files, triangles);

    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CGTP - Fase 1");
		
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
