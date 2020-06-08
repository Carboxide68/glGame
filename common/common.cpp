#include "common.h"

#ifndef tris
#define tris
uint tris_per_frame = 0;
#endif

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

std::string loadFile(std::string fileSource) {
    //Loading file
    std::ifstream file(fileSource);

    std::string charFile = "";

    //If loading was successfull, we read the file and put it into a stringstream which in turn is converted to the target type
    if (file) {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        charFile = ss.str();
    } else {
        std::cout << "Failed to load file: " << fileSource << "!" << std::endl;
    }
    return charFile;
}

uint G_triangles;