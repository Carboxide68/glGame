#pragma once
#include "../common/common.h"

#include <iostream>

class Shader {

public:


	Shader(std::string vSS, std::string fSS) : Shader(vSS, "", fSS){}

	Shader(std::string vSS, std::string gSS, std::string fSS) {

		//Creating program
		program = glCreateProgram();
		GLuint vertexShader, geometryShader, fragmentShader;
		//Creating shaders
		GLCall(vertexShader = glCreateShader(GL_VERTEX_SHADER));
		
		if (gSS != "") {
			GLCall(geometryShader = glCreateShader(GL_GEOMETRY_SHADER));
		}

		GLCall(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

		//Reading VertexShader file and storing it in a stringstream
		std::string vertexShaderString = loadFile(vSS);

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

		if (gSS != "") {
			std::string geoemtryShaderString = loadFile(gSS);

			const GLchar *f[1] = {geoemtryShaderString.data()};

			//Attaching the source and compiling the shader
			GLCall(glShaderSource(geometryShader, 1, f, NULL));
			GLCall(glCompileShader(geometryShader));

			memset(infoLog, 0, 512);
			//Compile error checking
			GLCall(glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success));
			if (!success) {
				GLCall(glGetShaderInfoLog(geometryShader, 512, NULL, infoLog));
				std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		//Reading FragmentShader file and storing it in a stringstream
		std::string fragmentShaderString = loadFile(fSS);

		const GLchar *f[1] = {fragmentShaderString.data()};

		//Attaching the source and compiling the shader
		GLCall(glShaderSource(fragmentShader, 1, f, NULL));
		GLCall(glCompileShader(fragmentShader));

		memset(infoLog, 0, 512);

		//Compile error checking
		GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Attaching and linking program
		GLCall(glAttachShader(program, vertexShader));
		if (gSS != "") {
			GLCall(glAttachShader(program, geometryShader));
		}
		GLCall(glAttachShader(program, fragmentShader));
		GLCall(glLinkProgram(program));

		memset(infoLog, 0, 512);

		//Linking error checking
		GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
		if (!success) {
			GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		GLCall(glDetachShader(program, vertexShader));
		if (gSS != "") {
			GLCall(glDetachShader(program, geometryShader));
		}

		GLCall(glDetachShader(program, fragmentShader));

		GLCall(glDeleteShader(vertexShader));
		if (gSS != "") {
			GLCall(glDeleteShader(geometryShader));
		}

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
		GLCall(glUniform1f(ID, value));
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

	void setUniform(GLuint ID, uint value) const {
		GLCall(glUniform1ui(ID, value))
	}

	void setUniform(std::string name, uint value) const {
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