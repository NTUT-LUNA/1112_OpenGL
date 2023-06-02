#include <algorithm>
#include "Vector.h"

void SetPixel(const int x, const int y, const Color3f color)
{
    glColor3f(color.r, color.g, color.b);
    glPointSize(15);
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}
void SetPixel(const int x, const int y)
{
    Color3f color(0.f, 1.f, 1.f);
    SetPixel(x, y, color);
}
GLdouble Min(const GLdouble a, const GLdouble b, const GLdouble c)
{
    return std::min(std::min(a, b), c);
}
GLdouble Max(const GLdouble a, const GLdouble b, const GLdouble c)
{
    return std::max(std::max(a, b), c);
}
