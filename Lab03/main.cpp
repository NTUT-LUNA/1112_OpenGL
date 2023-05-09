#include <stdio.h> 
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
/*** freeglut***/
#include <freeglut.h>

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void HandleRotation(unsigned char, int, int);
void HandleRotation(int, int, int);

float tx, ty, tz;
float degX, degY, degZ;
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

void CalculateRotMatrix(GLfloat *matrix, float deg, int x, int y, int z)
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

void PrintMatrix(const GLfloat *matrix)
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

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Moving Cube");
    SetupRC();

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(HandleRotation);
    glutSpecialFunc(HandleRotation);
    glutMainLoop();	//http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
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
    default:
        break;
    }
    glutPostRedisplay();
}
void HandleRotation(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: // change translation along x-axis
        tx += 0.2f;
        break;
    case GLUT_KEY_RIGHT: // change translation along x-axis
        tx -= 0.2f;
        break;
    case GLUT_KEY_UP: // change translation along x-axis
        ty -= 0.2f;
        break;
    case GLUT_KEY_DOWN: // change translation along x-axis
        ty += 0.2f;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
