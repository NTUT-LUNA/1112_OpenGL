#ifndef MIDPOINT_H
#define MIDPOINT_H

#include "Vector.h"

void PrintColorXY(float x, float y, float d);
void PrintRegion(Vec2 v1, Vec2 v2);
void WritePixel(float x, float y, float d);
void WritePixel(float x, float y, Color3f color);
void WritePixel(float x, float y, Vec2 v1, Vec2 v2, bool isOnX);
void MidpointLow(Vec2 v1, Vec2 v2, int swap);
void MidpointHigh(Vec2 v1, Vec2 v2, int swap);
void Midpoint(Vec2 v1, Vec2 v2);

#endif // MIDPOINT_H
