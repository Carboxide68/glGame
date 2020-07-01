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

struct UniformTexture {
    std::string path;
    std::string name;
};

struct Material {

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    uint illum;
    float specE;
    float opacity;
    float opticalDensity;
    std::vector<UniformTexture> textures;
};

const Material EMPTY_MATERIAL = {{0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, 1, 32.0f, 0.0f, 0.0f, {}};

struct StandardVertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 TexCoords;
};

const size_t STANDARD_VERTEX_BYTE_SIZE = sizeof(StandardVertex);