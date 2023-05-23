#ifndef CROW_H
#define CROW_H

#include "Vector.h"

void Diff(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, float d, float f);
void DiffX(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int x);
void DiffY(Vec2* v1, Vec2* v2, Vec2* e, Vec2* de, int y);
void Inc(Vec2* edge, Vec2* delta);
void ScanX(Vec2* l, Vec2* r, int y);
void ScanY(Vec2 vList[], int n, int i);
void Crow(Vec2 vList[], int n);

#endif // CROW_H
