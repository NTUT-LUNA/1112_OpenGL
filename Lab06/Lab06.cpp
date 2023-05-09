#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
/*** freeglut***/
#include <freeglut.h>

#define print(x) std::cout << x << std::endl
#define xy(x, y) std::cout << "(" << x << ", " << y << ")" << std::endl

typedef unsigned char uchar;

void SelectMenu(int);
void SelectGridDimMenu(int);
void BuildPopupMenu();
void SetupRC();
void MyReshape(int, int);
void MyDisplay(void);
void MyKeyboard(uchar, int, int);
void MySpecialKeys(int, int, int);
void MyMouse(int, int , int, int);

float dimension;
// rotation
float degX, degY, degZ, degC;
// translation
float tx, ty, tz;
// window
float ww, wh;
float padding;
// mouse
float mx, my;

void PrintMatrix(const GLfloat* matrix)
{
    for (int i = 0; i < 16; i++)
    {
        if (i % 4 == 0)
        {
            printf("%f", matrix[i]);
        }
        else
        {
            printf(", %f", matrix[i]);
        }
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}
void ResetVariables()
{
    tx = ty = tz = 0.f;
    degX = degY = degZ = 0.f;
    degC = 0.f;
    dimension = 10;
    padding = 1.f;
    mx = my = 0.0f;
}
float CalculatePadded()
{
    return dimension + padding;
}
int main(int argc, char** argv)
{
    srand(time(NULL)); // random seed
    ResetVariables(); // init global variables

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(800, 400);
    glutCreateWindow("2D Grid");

    BuildPopupMenu();
    SetupRC();

    glutReshapeFunc(MyReshape);
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMouseFunc(MyMouse);
    glutSpecialFunc(MySpecialKeys);

    glutMainLoop();	// http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html

    return 0;
}
void SelectMenu(int index)
{
    ;
}
void SelectGridDimMenu(int index)
{
    dimension = 5 + (5 * index);
    MyReshape(ww, wh);
    glutPostRedisplay();
}
void BuildPopupMenu()
{
    // build grid dimension menu
    int gridDimMenu = glutCreateMenu(SelectGridDimMenu);
    glutAddMenuEntry("10", 1);
    glutAddMenuEntry("15", 2);
    glutAddMenuEntry("20", 3);
    // build main menu
    int menu = glutCreateMenu(SelectMenu);
    glutAddSubMenu("Grid Dim", gridDimMenu);
    // attach main manu to key
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void SetupRC()
{
    // Light values and coordinates
    GLfloat whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
    GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat lightPos[] = { 0.f, 25.0f, 20.0f, 0.0f };
    // Enable lighting
    glEnable(GL_LIGHTING);
    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}
void MyReshape(int w, int h)
{
    float padded = CalculatePadded();
    glutReshapeWindow(w, w);
    ww = w; wh = w;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-21, 21, -21, 21, -20, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void MyDisplay(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear content in buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

    // render
    float halfPadding = padding / 2;
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_LINES);
    float length = dimension + 1;
    for (float i = -dimension; i <= length; i++)
    {
		glVertex2f(i - halfPadding, -dimension - halfPadding);
		glVertex2f(i - halfPadding, length - halfPadding);

		glVertex2f(-dimension - halfPadding, i - halfPadding);
		glVertex2f(length - halfPadding, i - halfPadding);
	}
    glEnd();

    glColor3f(1.f, 0.f, 0.f);
    glPointSize(15);
    glBegin(GL_POINTS);
    glVertex2f(0.f, 0.f);
    glVertex2f(mx, my);
    glEnd();

    // swap front/back buffer
    glutSwapBuffers();
}
void MyKeyboard(uchar key, int x, int y)
{
    glutPostRedisplay();
}
void MyMouse(int button, int state, int x, int y)
{
    // check mouse pressed
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) { return; }
    float xx, yy, ratio, gridWidth;
    ratio = 1 / (ww / 2.0f);
    gridWidth = ww / ((20 * 2) + 1);
	xx = (x - (ww / 2)) * ratio;
	yy = (y - (wh / 2)) * -ratio;
    mx = round(xx * 21);
    my = round(yy * 21);
    //xy(xx, yy);
    //xy(mx, my);
    //mx = xx;
    //my = yy;

    printf("Mouse (%d, %d) --> origin (%.2f, %.2f) --> grid (%.2f, %.2f)\n", x, y, xx, yy, mx, my);

    glutPostRedisplay();
}
void MySpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}