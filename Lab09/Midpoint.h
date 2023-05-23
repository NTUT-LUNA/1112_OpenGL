#ifndef MIDPOINT_H
#define MIDPOINT_H

#include "Vector.h"

void WritePixel(float x, float y, float d);
void MidpointLow(Vec2 v1, Vec2 v2, int swap);
void MidpointHigh(Vec2 v1, Vec2 v2, int swap);
void Midpoint(Vec2 v1, Vec2 v2);

#endif // MIDPOINT_H
