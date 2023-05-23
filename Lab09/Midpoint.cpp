#include <cmath>
/*** freeglut***/
#include <freeglut.h>
#include "Helper.h"
#include "Midpoint.h"

void WritePixel(float x, float y, float d)
{
    Color3f color;
    if (d > 0)
    {
        color.b = 1.f;
        print("[Blue ] ");
    }
    else
    {
        color.g = 1.f;
        print("[Green] ");
    }
    xy(x, y);
    SetPixel(x, y, color);
}
void MidpointLow(Vec2 v1, Vec2 v2, int swap)
{
    float dx, dy, yi, d, x, y;
    dx = v2.x - v1.x;
    dy = v2.y - v1.y;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
        if (swap) { println("region 4"); }
        else { println("region 8"); }
    }
    else
    {
        yi = 1;
        if (swap) { println("region 5"); }
        else { println("region 1"); }
    }
    d = 2 * dy - dx;
    y = v1.y;
    for (x = v1.x; x < v2.x; x++)
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
void MidpointHigh(Vec2 v1, Vec2 v2, int swap)
{
    float dx, dy, xi, d, x, y;
    dx = v2.x - v1.x;
    dy = v2.y - v1.y;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
        if (swap) { println("region 7"); }
        else { println("region 3"); }
    }
    else
    {
        xi = 1;
        if (swap) { println("region 6"); }
        else { println("region 2"); }
    }
    d = 2 * dx - dy;
    x = v1.x;
    for (y = v1.y; y < v2.y; y++)
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
void Midpoint(Vec2 v1, Vec2 v2)
{
    float x1, y1, x2, y2;
    x1 = v1.x;
    y1 = v1.y;
    x2 = v2.x;
    y2 = v2.y;
    if (abs(y2 - y1) < abs(x2 - x1))
    {
        if (x1 > x2) { MidpointLow(v2, v1, 1); }
        else { MidpointLow(v1, v2, 0); }
    }
    else
    {
        if (y1 > y2) { MidpointHigh(v2, v1, 1); }
        else { MidpointHigh(v1, v2, 0); }
    }
}
