#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
/*** freeglut***/
#include <freeglut.h>

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void HandleRotation(unsigned char, int, int);
void HandleTranslation(int, int, int);

float tx, ty, tz;
float degX, degY, degZ, degC;

float length;
float cx, cy, cz;

GLfloat v1[3] = { 0 };
GLfloat v2[3] = { 0 };

// rotation matricies
GLfloat rotXMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};
GLfloat rotYMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};
GLfloat rotZMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};
// translation matrix
GLfloat translateMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};
// arbitrary axis rotation matrix
GLfloat multiMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

void CalculateRotMatrix(GLfloat* matrix, float deg, int x, int y, int z)
{
    double rad = deg * M_PI / 180.0;
    if (x)
    {
        matrix[5] = cos(rad);
        matrix[6] = -sin(rad);
        matrix[9] = sin(rad);
        matrix[10] = cos(rad);
    }
    else if (y)
    {
        matrix[0] = cos(rad);
        matrix[2] = sin(rad);
        matrix[8] = -sin(rad);
        matrix[10] = cos(rad);
    }
    else
    {
        matrix[0] = cos(rad);
        matrix[1] = -sin(rad);
        matrix[4] = sin(rad);
        matrix[5] = cos(rad);
    }
}

void CalculateTranslationMatrix(GLfloat* matrix, float x, float y, float z)
{
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}

void MatrixReset()
{
    for (int i = 0; i < 16; i++)
    {
        multiMatrix[i] = 0;
    }
    multiMatrix[0] = 1;
    multiMatrix[5] = 1;
    multiMatrix[10] = 1;
    multiMatrix[15] = 1;
}

void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z)
{
    MatrixReset();
    GLfloat Cos = cos(angle * M_PI / 180);
    GLfloat Sin = sin(angle * M_PI / 180);

    multiMatrix[0] = Cos + (1 - Cos) * X * X;
    multiMatrix[1] = (1 - Cos) * X * Y - (Sin * Z);
    multiMatrix[2] = (1 - Cos) * X * Z + (Sin * Y);

    multiMatrix[4] = (1 - Cos) * Y * X + (Sin * Z);
    multiMatrix[5] = Cos + (1 - Cos) * Y * Y;
    multiMatrix[6] = (1 - Cos) * Y * Z - (Sin * X);

    multiMatrix[8] = (1 - Cos) * Z * X - (Sin * Y);
    multiMatrix[9] = (1 - Cos) * Z * Y + (Sin * X);
    multiMatrix[10] = Cos + (1 - Cos) * Z * Z;

    glMultMatrixf(multiMatrix);
}

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

int main(int argc, char** argv)
{
    tx = 0.f;
    ty = 0.f;
    tz = 0.f;

    degX = 0.f;
    degY = 0.f;
    degZ = 0.f;

    // v1
    std::cout << "Please provide v1 (x):\n";
    std::cin >> v1[0];
    std::cout << "Please provide v1 (y):\n";
    std::cin >> v1[1];
    std::cout << "Please provide v1 (z):\n";
    std::cin >> v1[2];
    // v2
    std::cout << "Please provide v2 (x):\n";
    std::cin >> v2[0];
    std::cout << "Please provide v2 (y):\n";
    std::cin >> v2[1];
    std::cout << "Please provide v2 (z):\n";
    std::cin >> v2[2];
    // length vector
    cx = v2[0] - v1[0];
    cy = v2[1] - v1[1];
    cz = v2[2] - v1[2];
    // unit length
    length = pow(cx*cx + cy*cy + cz*cz, 0.5);
    cx /= length;
    cy /= length;
    cz /= length;
    // degC
    std::cout << "Please provide rotation degree along arbitrary axis:\n";
    std::cin >> degC;
    // print
    std::cout << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;
    std::cout << v2[0] << " " << v2[1] << " " << v2[2] << std::endl;
    std::cout << "deg: " << degC << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Arbitrary Cube");
    SetupRC();

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(HandleRotation);
    glutSpecialFunc(HandleTranslation);
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

    // arbitrary axis
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(v1[0], v1[1], v1[2]);
    glVertex3f(v2[0], v2[1], v2[2]);
    glEnd();

    // perform transformation for the cube with matrix calculation
    CalculateRotMatrix(rotXMatrix, degX, 1, 0, 0);
    CalculateRotMatrix(rotYMatrix, degY, 0, 1, 0);
    CalculateRotMatrix(rotZMatrix, degZ, 0, 0, 1);
    CalculateTranslationMatrix(translateMatrix, tx, ty, tz);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(rotXMatrix);
    glMultMatrixf(rotYMatrix);
    glMultMatrixf(rotZMatrix);
    glMultMatrixf(translateMatrix);
    
    // perform transformation for the cube with arbitrary axis
    RotateMatrix(degC, cx, cy, cz);

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
void HandleRotation(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r': // reset translation & rotation
        degX = 0.f;
        degY = 0.f;
        degZ = 0.f;
        degC = 0.f;
        break;
    case 'a': // change rotation angle along x-axis
        degY += 3.f;
        break;
    case 'd': // change rotation angle along x-axis
        degY -= 3.f;
        break;
    case 'w': // change rotation angle along y-axis
        degX += 3.f;
        break;
    case 's': // change rotation angle along y-axis
        degX -= 3.f;
        break;
    case 'z': // change rotation angle along z-axis
        degZ -= 3.f;
        break;
    case 'x': // change rotation angle along z-axis
        degZ += 3.f;
        break;
    case 'c': // change rotation angle along arbitrary axis
        degC -= 3.f;
        break;
    case 'v': // change rotation angle along arbitrary axis
        degC += 3.f;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void HandleTranslation(int key, int x, int y)
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