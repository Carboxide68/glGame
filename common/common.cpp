#include "common.h"
#include <iostream>

void GLClearErrors() {
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

    while(GLenum error = glGetError()) {
        std::cout << "[OpenGL error] (0x" << std::hex << error << ")" << " in file " << file << " in function " << function << " on line " << line << std::endl;
        return false;
    }
    return true;
}