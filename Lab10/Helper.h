#ifndef HELPER_H
#define HELPER_H

#include <iostream>
/*** freeglut***/
#include <freeglut.h>
#include "Vector.h"

#define print(x) std::cout << x
#define println(x) std::cout << x << std::endl
#define rgb(vec) std::cout << vec.r << " " << vec.g << " " << vec.b << std::endl;
#define newline() std::cout << std::endl
#define xy(x, y) std::cout << "(" << x << ", " << y << ")" << std::endl

void SetPixel(const int x, const int y, const Color3f color);
void SetPixel(const int x, const int y);
GLdouble Min(const GLdouble a, const GLdouble b, const GLdouble c);
GLdouble Max(const GLdouble a, const GLdouble b, const GLdouble c);

#endif // HELPER_H
