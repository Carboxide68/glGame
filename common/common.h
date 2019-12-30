#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);

#define DEBUG

//#define GLCall(x) x;
// #define GLCall(x) GLClearErrors(); x; printf("%s\n\n", #x); ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#define GLCall(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearErrors();

bool GLLogCall(const char* function, const char* file, int line);