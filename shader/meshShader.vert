#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 TexCoord;

uniform mat4 assembledMatrix;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoord;

void main() {
	gl_Position = assembledMatrix * vec4(vPos, 1.0);
	Normal = mat3(transpose(inverse(model))) * vNormal;
    FragPos = (model * vec4(vPos, 1.0)).xyz;
	texCoord = TexCoord;
}