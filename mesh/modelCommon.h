#pragma once

#include "../common/common.h"

struct Triangle {
    glm::vec3 pos1;
    glm::vec2 TexCoord1;
    glm::vec3 normal1;
    glm::vec3 pos2;
    glm::vec2 TexCoord1;
    glm::vec3 normal2;
    glm::vec3 pos3;
    glm::vec2 TexCoord1;
    glm::vec3 normal3;
};

union modelID {
    struct {
        uint mesh;
        uint polygon;
    };
    ulong ID;
};