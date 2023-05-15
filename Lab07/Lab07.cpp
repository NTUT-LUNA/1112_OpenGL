#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
/*** freeglut***/
#include <freeglut.h>

#define print(x) std::cout << x
#define println(x) std::cout << x << std::endl
#define newline() std::cout << std::endl
#define xy(x, y) std::cout << "(" << x << ", " << y << ")" << std::endl

typedef unsigned char uchar;

struct Vec2
{
    GLdouble x;
    GLdouble y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
};
struct Rect
{
    Vec2 v1;
    Vec2 v2;
    Vec2 v3;
    Vec2 v4;
    Rect() : v1(Vec2()), v2(Vec2()), v3(Vec2()), v4(Vec2()) {}
};

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
int clickCount;
Vec2 vec;
Rect rect;
// mode
bool debug = false;

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
void WritePixel(float x, float y, float d)
{
    if (d > 0)
    {
        glColor3f(0.f, 0.f, 1.f);
        print("[Blue ] ");
        xy(x, y);
    }
    else
    {
        glColor3f(0.f, 1.f, 0.f);
        print("[Green] ");
        xy(x, y);
    }
    glPointSize(15);
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}
void MidpointLow(Vec2 v0, Vec2 v1, int swap)
{
    float dx, dy, yi, d, x, y;
    dx = v1.x - v0.x;
    dy = v1.y - v0.y;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
        if (swap)
        {
            println("region 4");
        }
        else
        {
            println("region 8");
        }
    }
    else
    {
        yi = 1;
        if (swap)
        {
            println("region 5");
        }
        else
        {
            println("region 1");
        }
    }
    d = 2 * dy - dx;
    y = v0.y;
    for (x = v0.x; x < v1.x; x++)
    {
        WritePixel(x, y, d);
        if (d > 0)
        {
            y = y + yi;
            d = d + (2 * (dy - dx));
        }
        else
        {
            d = d + 2 * dy;
        }
    }
}
void MidpointHigh(Vec2 v0, Vec2 v1, int swap)
{
    float dx, dy, xi, d, x, y;
    dx = v1.x - v0.x;
    dy = v1.y - v0.y;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
        if (swap)
        {
            println("region 7");
        }
        else
        {
            println("region 3");
        }
    }
    else
    {
        xi = 1;
        if (swap)
        {
            println("region 6");
        }
        else
        {
            println("region 2");
        }
    }
    d = 2 * dx - dy;
    x = v0.x;
    for (y = v0.y; y < v1.y; y++)
    {
        WritePixel(x, y, d);
        if (d > 0)
        {
            x = x + xi;
            d = d + (2 * (dx - dy));
        }
        else
        {
            d = d + 2 * dx;
        }
    }
}
void Midpoint(Vec2 v0, Vec2 v1)
{
    float x0, y0, x1, y1;
    x0 = v0.x;
    y0 = v0.y;
    x1 = v1.x;
    y1 = v1.y;
    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            MidpointLow(v1, v0, 1);
        }
        else
        {
            MidpointLow(v0, v1, 0);
        }
    }
    else
    {
        if (y0 > y1)
        {
            MidpointHigh(v1, v0, 1);
        }
        else
        {
            MidpointHigh(v0, v1, 0);
        }
    }
}
void Midpoint(Rect rect)
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

    if (debug)
    {
        glColor3f(1.f, 0.f, 0.f);
        glPointSize(15);
        glBegin(GL_POINTS);
        glVertex2f(vec.x, vec.y);
        glEnd();
    }
    else
    {
        // rect points
        glColor3f(1.f, 0.f, 0.f);
        glPointSize(15);
        glBegin(GL_POINTS);
        glVertex2f(rect.v1.x, rect.v1.y);
        glVertex2f(rect.v2.x, rect.v2.y);
        glVertex2f(rect.v3.x, rect.v3.y);
        glVertex2f(rect.v4.x, rect.v4.y); // current point
        glEnd();

        // midpoint algorithm
        Midpoint(rect);

        // origin
        glColor3f(1.f, 1.f, 1.f);
        glBegin(GL_POINTS);
        glVertex2f(0.f, 0.f);
        glEnd();
    }

    // swap front/back buffer
    glutSwapBuffers();
}
void MyKeyboard(uchar key, int x, int y)
{
    if (key == 'r')
    {
        ResetVariables();
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

    if (clickCount == 4)
    {
        clickCount = 0;
        rect = Rect();
    }
    rect.v1 = rect.v2;
    rect.v2 = rect.v3;
    rect.v3 = rect.v4;
    rect.v4 = vec;
    clickCount++;

    if (debug)
        printf("Mouse (%d, %d) --> origin (%.2f, %.2f) --> grid (%.2f, %.2f)\n", x, y, xx, yy, vec.x, vec.y);

    glutPostRedisplay();
}
void MySpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}