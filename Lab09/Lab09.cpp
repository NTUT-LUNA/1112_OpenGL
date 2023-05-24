#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
/*** freeglut***/
#include <freeglut.h>
// Custom
#include "Vector.h"
#include "Helper.h"
#include "Midpoint.h"
#include "Halfspace.h"
#include "Crow.h"

#define POLY_MAX 8 // max vertex for polygon

typedef unsigned char uchar;

void SelectMenu(int);
void SelectGridDimMenu(int);
void BuildPopupMenu();
void SetupRC();
void Idle();
void Timer(int);
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
int clickCount;
Vec2 vec;
Rect rect;
Triangle triangle;
std::vector<Vec2> poly;
Vec2 *vList;
// mode
bool debug = false;
// halfspace pixels
std::vector<Vec2> pixels;
int pIndex = 0;

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
    dimension = 20;
    padding = 1.f;
    clickCount = 0;
    vec = Vec2();
    rect = Rect();
}
int main(int argc, char** argv)
{
    srand(time(NULL)); // random seed
    ResetVariables(); // init global variables

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(800, 400);
    glutCreateWindow("2D Grid animated");

    BuildPopupMenu();
    SetupRC();

    glutReshapeFunc(MyReshape);
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMouseFunc(MyMouse);
    glutSpecialFunc(MySpecialKeys);
    glutIdleFunc(Idle);
    glutTimerFunc(0, Timer, 0); // First timer call immediately

    glutMainLoop();	// http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html

    return 0;
}
void AssignVList(const Rect& rect)
{
    vList = new Vec2[4];
    vList[0] = rect.v4;
    vList[1] = rect.v3;
    vList[2] = rect.v2;
    vList[3] = rect.v1;
}
void AssignVList(const Triangle& triangle)
{
    vList = new Vec2[3];
    vList[0] = triangle.v3;
    vList[1] = triangle.v2;
    vList[2] = triangle.v1;
}
void AssignVList(const std::vector<Vec2>& poly)
{
    int i;
    int n = poly.size();
    vList = new Vec2[n];
    for (i = 0; i < n; i++)
    {
        vList[i] = poly.at(n - i - 1);
    }
}
void Crow(Vec2 vList[], int n)
{
    int iMin = 0;
    for (int i = 1; i < n; i++)
    {
        if (vList[i].y < vList[iMin].y)
        {
            iMin = i;
        }
    }
    ScanY(vList, n, iMin);
    delete[] vList;
}
void Crow(const Rect& rect)
{
    AssignVList(rect);
    Crow(vList, 4);
}
void Crow(const Triangle& triangle)
{
    AssignVList(triangle);
    Crow(vList, 3);
}
void Crow(std::vector<Vec2> poly)
{
    if (poly.size() < 3) { return; } // less than 3 can't fill color
    AssignVList(poly);
    Crow(vList, poly.size());
}
void Midpoint(Rect& rect)
{
    print("v1v2: ");
    Midpoint(rect.v1, rect.v2);
    print("v2v3: ");
    Midpoint(rect.v2, rect.v3);
    print("v3v4: ");
    Midpoint(rect.v3, rect.v4);
    print("v4v1: ");
    Midpoint(rect.v4, rect.v1);
    newline();
}
void Midpoint(Triangle& triangle)
{
    print("v1v2: ");
    Midpoint(triangle.v1, triangle.v2);
    print("v2v3: ");
    Midpoint(triangle.v2, triangle.v3);
    print("v3v1: ");
    Midpoint(triangle.v3, triangle.v1);
    newline();
}
void Midpoint(std::vector<Vec2>& poly)
{
    if (poly.size() <= 0) { return; } // infinite loop guard
    int i;
    for (i = 0; i < poly.size() - 1; i++)
    {
        print("v"); print(i + 1); print("v"); print(i + 2);
        Midpoint(poly.at(i), poly.at(i + 1));
    }
    print("v"); print(i + 1); print("v1");
    Midpoint(poly.at(i), poly.at(0));
    newline();
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
void SelectModeMenu(int index)
{
    debug = index > 1;
    glutPostRedisplay();
}
void BuildPopupMenu()
{
    // build grid dimension menu
    int gridDimMenu = glutCreateMenu(SelectGridDimMenu);
    glutAddMenuEntry("10", 1);
    glutAddMenuEntry("15", 2);
    glutAddMenuEntry("20", 3);
    // build mode menu
    int modeMenu = glutCreateMenu(SelectModeMenu);
    glutAddMenuEntry("normal", 1);
    glutAddMenuEntry("debug", 2);
    // build main menu
    int menu = glutCreateMenu(SelectMenu);
    glutAddSubMenu("Dimension", gridDimMenu);
    glutAddSubMenu("Mode", modeMenu);
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
void Idle()
{
    glutPostRedisplay();
}
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(30, Timer, 0);
}
void MyReshape(int w, int h)
{
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
    if (debug)
    {
        glVertex2f(vec.x, vec.y);
    }
    else
    {
        //glVertex2f(rect.v1.x, rect.v1.y);
        //glVertex2f(rect.v2.x, rect.v2.y);
        //glVertex2f(rect.v3.x, rect.v3.y);
        //glVertex2f(rect.v4.x, rect.v4.y); // current point

        glVertex2f(triangle.v1.x, triangle.v1.y);
        glVertex2f(triangle.v2.x, triangle.v2.y);
        glVertex2f(triangle.v3.x, triangle.v3.y); // current point
        
        /*for (int i = 0; i < poly.size(); i++)
        {
            glVertex2f(poly.at(i).x, poly.at(i).y);
        }*/
    }
    glEnd();

    if (!debug)
    {
        // midpoint algorithm
        //Midpoint(rect);
        Midpoint(triangle);
        //Midpoint(poly);

        // Halfspace algorithm
        //HalfSpace(triangle);

        // Halfspace with animation
        if (pixels.size() > 0)
        {
            for (int i = 0; i < pixels.size(); i++)
            {
                if (i > pIndex) { break; }
                SetPixel(pixels.at(i).x, pixels.at(i).y);
            }
        }

        // Crow's algorithm
        //Crow(rect);
        //Crow(triangle);
        //Crow(poly);
    }
    
    // origin
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_POINTS);
    glVertex2f(0.f, 0.f);
    glEnd();

    // swap front/back buffer
    glutSwapBuffers();

    pIndex += 5;
}
void MyKeyboard(uchar key, int x, int y)
{
    if (key == 'r')
    {
        ResetVariables();
        poly.clear();
    }
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
    vec.x = round(xx * 21);
    vec.y = round(yy * 21);

    /*if (clickCount == 4)
    {
        clickCount = 0;
        rect = Rect();
    }
    rect.v1 = rect.v2;
    rect.v2 = rect.v3;
    rect.v3 = rect.v4;
    rect.v4 = vec;*/

    if (clickCount == 3)
    {
        clickCount = 0;
        triangle = Triangle();
        pIndex = 0;
        pixels.clear();
    }
    triangle.v1 = triangle.v2;
    triangle.v2 = triangle.v3;
    triangle.v3 = vec;
    
    /*if (clickCount == POLY_MAX)
    {
        clickCount = 0;
        poly.clear();
    }
    poly.push_back(vec);*/

    clickCount++;

    if (debug)
        printf("Mouse (%d, %d) --> origin (%.2f, %.2f) --> grid (%.2f, %.2f)\n", x, y, xx, yy, vec.x, vec.y);

    // push back pixels for drawing
    pixels.clear();
    HalfSpace(triangle, pixels);
    pIndex = 0;

    print("pixels size: ");
    println(pixels.size());

    glutPostRedisplay();
}
void MySpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}