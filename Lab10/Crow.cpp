#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
/*** freeglut***/
#include <freeglut.h>
#include "Helper.h"
#include "Crow.h"

void Inc(Vec2* edge, Vec2* delta)
{
    edge->x += delta->x;
    // color
    edge->r += delta->r;
    edge->g += delta->g;
    edge->b += delta->b;
}
void Diff(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, float d, float f)
{
    de->x = (v2->x - v1->x) / d;
    e->x = v1->x + (f * de->x);
    // color
    de->r = (v2->r - v1->r) / d;
    e->r = v1->r + f * de->r;
    de->g = (v2->g - v1->g) / d;
    e->g = v1->g + f * de->g;
    de->b = (v2->b - v1->b) / d;
    e->b = v1->b + f * de->b;
}
void DiffX(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int x)
{
    Diff(v1, v2, e, de, (v2->x - v1->x), (x - v1->x));
}
void DiffY(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int y)
{
    Diff(v1, v2, e, de, (v2->y - v1->y), (y - v1->y));
}
void ScanX(Vec2* l, Vec2* r, int y, std::vector<Vec2>* pixels)
{
    int x, lx, rx;
    Vec2 s, ds, pixel; // color
    lx = ceil(l->x);
    rx = ceil(r->x);
    if (lx < rx)
    {
        DiffX(l, r, &s, &ds, lx);
        for (x = lx; x < rx; x++)
        {
            if (pixels == nullptr)
            {
                // draw the span
                Color3f color(0.f, 1.f, 1.f);
                SetPixel(x, y, color);
            }
            else
            {
                pixel.x = x;
                pixel.y = y;
                pixel.r = s.r;
                pixel.g = s.g;
                pixel.b = s.b;
                pixels->push_back(pixel);
            }
            Inc(&s, &ds);
        }
    }
}
void ScanY(Vec2 vList[], int n, int i, std::vector<Vec2>* pixels)
{
    int li, ri; 	// left & right upper endpoint indices
    int ly, ry;	    // left & right upper endpoint y values
    Vec2 l, dl;	    // current left edge and delta
    Vec2 r, dr;	    // current right edge and delta
    int rem;	    // number of remaining vertices
    int y;		    // current scanline

    li = ri = i;
    ly = ry = y = ceil(vList[i].y);

    for (rem = n; rem > 0;) // (1)
    {
        // find appropriate left edge (2)
        while (ly <= y && rem > 0)
        {
            rem--;
            i = li - 1;
            if (i < 0) { // go clockwise
                i = n - 1;
            }

            ly = ceil(vList[i].y);
            if (ly > y) // replace left edge
            {
                DiffY(&vList[li], &vList[i], &l, &dl, y);
            }
            li = i;  // index of the left endpoint
        }
        // find appropriate right edge
        while (ry <= y && rem > 0)
        {
            rem--;
            i = ri + 1;
            if (i == n) { // go anti-clockwise
                i = 0;
            }

            ry = ceil(vList[i].y);
            if (ry > y) // replace right edge
            {
                DiffY(&vList[ri], &vList[i], &r, &dr, y);
            }
            ri = i;  // index of the right endpoint
        }
        // while l & r span y (the current scanline) (3)
        for (; y < ly && y < ry; y++)
        {
            // scan and interpolate edges
            ScanX(&l, &r, y, pixels);
            Inc(&l, &dl);
            Inc(&r, &dr);
        }
    }
}
