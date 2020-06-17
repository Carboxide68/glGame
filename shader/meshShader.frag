#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

struct Light {

    vec3 color;
    vec3 position;

};

uniform vec3 playerPos;
uniform vec3 objColor;
uniform float shininess;

uniform Light light;
uniform vec3 ambient;

float specularStrength = 0.5;

void main() {

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 ambient = ambient;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff;

    // specular
    vec3 viewDir = normalize(playerPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * light.color;

    vec3 result = ambient + specular + diffuse;

    // color = vec4(vec3(1/(mod(gl_PrimitiveID,100) + 1), 10/(mod(gl_PrimitiveID,1000) + 1), 100/(mod(gl_PrimitiveID,10000) + 1)), 1.0);
    color = vec4(norm, 1.0);
}