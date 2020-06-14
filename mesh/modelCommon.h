#pragma once

#include "../common/common.h"

struct Triangle {
    glm::vec3 pos1;
    glm::vec2 TexCoord1;
    glm::vec3 normal1;
    glm::vec3 pos2;
    glm::vec2 TexCoord2;
    glm::vec3 normal2;
    glm::vec3 pos3;
    glm::vec2 TexCoord3;
    glm::vec3 normal3;
};

const size_t TRIANGLE_SIZE = sizeof(Triangle);

union ModelID {
    struct {
        uint mesh;
        uint polygon;
    };
    ulong ID;
};

struct Material {

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float specStrength;
    float opacity;
    std::string ambientT;
    std::string diffuseT;
    std::string specularT;
};
const uint MATERIAL_PROPERTY_COUNT = 8; //The number of arguments in Material

struct StandardVertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 TexCoords;
};

const size_t STANDARD_VERTEX_BYTE_SIZE = sizeof(StandardVertex);