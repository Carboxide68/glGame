#pragma once
#include <cstring>
#include <signal.h>
#include <vector>
#include <array>
#include <memory>
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);

#define DEBUG

// #define GLCall(x) x;
// #define GLCall(x) GLClearErrors(); x; printf("%s\n\n", #x); ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#define GLCall(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearErrors();

bool GLLogCall(const char* function, const char* file, int line);

std::string vectorToString(glm::vec3 v);

std::string matrixToString(glm::mat4 matrix);
std::string matrixToString(glm::mat3 matrix);

std::string loadFile(std::string fileSource);

static const std::string STANDARD_NAME = "_CO_STANDARD";

template <typename T>
struct ErrWithData {
    T data;
    int error;
};

extern uint G_triangles;