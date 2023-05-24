#ifndef HALFSPACE_H
#define HALFSPACE_H

#include <vector>
#include "Vector.h"

float LineEq(Vec2 v1, Vec2 v2, int x, int y);
void HalfSpace(Triangle triangle);
void HalfSpace(Triangle triangle, std::vector<Vec2>& vec);

#endif // HALFSPACE_H
