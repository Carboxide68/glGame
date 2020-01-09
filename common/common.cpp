#include "common.h"

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

std::string vectorToString(glm::vec3 v) {
    std::string output =  std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z);
    return output;
}

std::string matrixToString(const glm::mat4 matrix) {
    std::string output = "";
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			output += std::to_string(matrix[y][x]);
		}
		output += "\n";
	}
    return output;
}

std::string matrixToString(const glm::mat3 matrix) {
    std::string output = "";
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			output += std::to_string(matrix[y][x]) + " ";
		}
		output += "\n";
	}
    return output;
}