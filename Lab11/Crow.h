#ifndef CROW_H
#define CROW_H

#include <vector>
#include "Vector.h"

void Inc(Vec2* edge, Vec2* delta);
void Diff(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, float d, float f);
void DiffX(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int x);
void DiffY(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int y);
void ScanX(Vec2* l, Vec2* r, int y, std::vector<Vec2>* pixels);
void ScanY(Vec2 vList[], int n, int i, std::vector<Vec2>* pixels);

#endif // CROW_H
