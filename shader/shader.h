#pragma once
#include "../common/common.h"

#include <iostream>

class Shader {

public:

	Shader(std::string vertexShaderSource, std::string fragmentShaderSource) {

		//Creating program
		GLCall(program = glCreateProgram());

		//Creating shaders
		GLCall(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
		GLCall(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

		//Reading VertexShader file and storing it in a stringstream
		std::string vertexShaderString = loadFile(vertexShaderSource);

		const GLchar *v[1] = {vertexShaderString.data()};

		//Attaching the source and compiling the shader
		GLCall(glShaderSource(vertexShader, 1, v, NULL));
		GLCall(glCompileShader(vertexShader));

		int success;
		char infoLog[512];

		//Compile error checking
		GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Reading FragmentShader file and storing it in a stringstream
		std::string fragmentShaderString = loadFile(fragmentShaderSource);

		const GLchar *f[1] = {fragmentShaderString.data()};

		//Attaching the source and compiling the shader
		GLCall(glShaderSource(fragmentShader, 1, f, NULL));
		GLCall(glCompileShader(fragmentShader));

		//Compile error checking
		GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Attaching and linking program
		GLCall(glAttachShader(program, vertexShader));
		GLCall(glAttachShader(program, fragmentShader));
		GLCall(glLinkProgram(program));

		//Linking error checking
		GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
		if(!success) {
			GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		GLCall(glDetachShader(program, vertexShader));
		GLCall(glDetachShader(program, fragmentShader));

		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));
	}

	inline void use() const {
		GLCall(glUseProgram(program));
	}

	GLuint getUniform(std::string name) const {
		GLCall(GLuint uniform = glGetUniformLocation(program, name.c_str()));
		return uniform; 
	}

	void setUniform(GLuint ID, float value) const {
		GLCall(glUniform1f(ID, value))
	}

	void setUniform(std::string name, float value) const {
		GLCall(setUniform(getUniform(name), value));
	}


	void setUniform(GLuint ID, int value) const {
		GLCall(glUniform1i(ID, value))
	}

	void setUniform(std::string name, int value) const {
		GLCall(setUniform(getUniform(name), value));
	}


	void setUniform(GLuint ID, bool value) const {
		GLCall(glUniform1i(ID, (int) value))
	}

	void setUniform(std::string name, bool value) const {
		GLCall(setUniform(getUniform(name), value));
	}

	void setUniform(GLuint ID, glm::vec3 value) {
		GLCall(glUniform3f(ID, value.x, value.y, value.z));
	}

	void setUniform(std::string name, glm::vec3 value) {
		GLCall(setUniform(getUniform(name), value));
	}
	
	void setUniform(GLuint ID, const glm::mat4 &value) const {
		GLCall(glUniformMatrix4fv(ID, 1, GL_FALSE, glm::value_ptr(value)));
	}

	void setUniform(std::string name, const glm::mat4 &value) const {
		GLCall(setUniform(getUniform(name), value));
	}

	void deleteShader() {
		
		GLCall(glDeleteProgram(program));
	}

private:

	GLuint program;

};