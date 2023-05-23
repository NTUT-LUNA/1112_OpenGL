#ifndef VECTOR_H
#define VECTOR_H

/*** freeglut***/
#include <freeglut.h>

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
struct Triangle
{
    Vec2 v1;
    Vec2 v2;
    Vec2 v3;
    Triangle() : v1(Vec2()), v2(Vec2()), v3(Vec2()) {}
};
struct Color3f
{
    GLdouble r;
    GLdouble g;
    GLdouble b;
    Color3f() : r(0), g(0), b(0) {}
    Color3f(float r, float g, float b) : r(r), g(g), b(b) {}
};

#endif // VECTOR_H
