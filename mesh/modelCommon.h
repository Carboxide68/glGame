#pragma once

#include "../common/common.h"
#include "group.h"
#include "polygon.h"
#include "model.h"
#include "mesh.h"

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

union ModelID {
    struct {
        uint mesh;
        uint polygon;
    };
    ulong ID;
};

size_t TRIANGLE_SIZE = sizeof(Triangle);