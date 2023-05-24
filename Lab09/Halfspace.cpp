/*** freeglut***/
#include <freeglut.h>
#include "Helper.h"
#include "Halfspace.h"

float LineEq(Vec2 v1, Vec2 v2, int x, int y)
{
    return ((v2.x - v1.x) * (y - v1.y) - (v2.y - v1.y) * (x - v1.x));
}
void HalfSpace(Triangle triangle)
{
    float e1, e2, e3;
    int x, y;
    int xMin = Min(triangle.v1.x, triangle.v2.x, triangle.v3.x);
    int xMax = Max(triangle.v1.x, triangle.v2.x, triangle.v3.x);
    int yMin = Min(triangle.v1.y, triangle.v2.y, triangle.v3.y);
    int yMax = Max(triangle.v1.y, triangle.v2.y, triangle.v3.y);
    for (y = yMin; y <= yMax; y++)
    {
        for (x = xMin; x <= xMax; x++)
        {
            e1 = LineEq(triangle.v1, triangle.v2, x, y);
            e2 = LineEq(triangle.v2, triangle.v3, x, y);
            e3 = LineEq(triangle.v3, triangle.v1, x, y);
            if (e1 < 0 && e2 < 0 && e3 < 0)
            {
                SetPixel(x, y);
            }
        }
    }
}
void HalfSpace(Triangle triangle, std::vector<Vec2>& vec)
{
    float e1, e2, e3;
    int x, y;
    Vec2 v;
    int xMin = Min(triangle.v1.x, triangle.v2.x, triangle.v3.x);
    int xMax = Max(triangle.v1.x, triangle.v2.x, triangle.v3.x);
    int yMin = Min(triangle.v1.y, triangle.v2.y, triangle.v3.y);
    int yMax = Max(triangle.v1.y, triangle.v2.y, triangle.v3.y);
    for (y = yMin; y <= yMax; y++)
    {
        for (x = xMin; x <= xMax; x++)
        {
            e1 = LineEq(triangle.v1, triangle.v2, x, y);
            e2 = LineEq(triangle.v2, triangle.v3, x, y);
            e3 = LineEq(triangle.v3, triangle.v1, x, y);
            if (e1 < 0 && e2 < 0 && e3 < 0)
            {
                v.x = x;
                v.y = y;
                vec.push_back(v);
            }
        }
    }
}
