#include <stdio.h> 
#include <stdlib.h>
/*** freeglut***/
#include <freeglut.h>

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void HandleKeyboard(unsigned char, int, int);
void HandleSpecial(int, int, int);

float tx, ty, tz;
float thetaX, thetaY, thetaZ;

int main(int argc, char** argv)
{
    tx = 0.f;
    ty = 0.f;
    tz = 0.f;

    thetaX = 0.f;
    thetaY = 0.f;
    thetaZ = 0.f;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Moving Cube");
    SetupRC();

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(HandleKeyboard);
    glutSpecialFunc(HandleSpecial);
    glutMainLoop();	//http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void RenderScene(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear content in buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

    // perform transformation for the cube
    glRotatef(thetaX, 1, 0, 0);
    glRotatef(thetaY, 0, 1, 0);
    glRotatef(thetaZ, 0, 0, 1);
    glTranslatef(tx, ty, tz);

    // draw x-axis, y-axis and z-axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(-10, 0, 0);
    glVertex3f(10, 0, 0);
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0, -10, 0);
    glVertex3f(0, 10, 0);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, -10);
    glVertex3f(0, 0, 10);
    glEnd();

    // cube
    glColor3f(1, 1, 0);
    glutSolidCube(6);
    glutSwapBuffers(); // swap front/back buffer
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
void HandleKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r': // reset translation & rotation
        thetaX = 0.f;
        thetaY = 0.f;
        thetaZ = 0.f;
        break;
    case 'a': // change rotation angle along x-axis
        thetaY -= 3.f;
        break;
    case 'd': // change rotation angle along x-axis
        thetaY += 3.f;
        break;
    case 'w': // change rotation angle along y-axis
        thetaX -= 3.f;
        break;
    case 's': // change rotation angle along y-axis
        thetaX += 3.f;
        break;
    case 'z': // change rotation angle along z-axis
        thetaZ += 3.f;
        break;
    case 'x': // change rotation angle along z-axis
        thetaZ -= 3.f;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void HandleSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: // change translation along x-axis
        tx -= 0.2f;
        break;
    case GLUT_KEY_RIGHT: // change translation along x-axis
        tx += 0.2f;
        break;
    case GLUT_KEY_UP: // change translation along x-axis
        ty += 0.2f;
        break;
    case GLUT_KEY_DOWN: // change translation along x-axis
        ty -= 0.2f;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}