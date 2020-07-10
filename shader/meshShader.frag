#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

struct Light {

    vec3 color;
    vec3 position;

};

struct Material {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    uint illum;
    float specE;
    float opacity;
    float opticalDensity;

};

uniform Material material;

uniform vec3 playerPos;

uniform Light light;
uniform vec3 ambient;

void main() {

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 ambient = material.ambient * ambient;
    float diff = abs(dot(norm, lightDir));
    vec3 diffuse = light.color * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(playerPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(abs(dot(norm, halfwayDir)), material.specE);
    vec3 specular = light.color * (material.specular * spec);

    vec3 result = ambient + specular + diffuse;

    color = vec4(vec3(result), 1.0);
}