#pragma once

#include "../common/common.h"
#include "node.h"
#include <random>

/* 
    This class makes a triangle based mesh.
    !Add more documentation!
*/

class Mesh {

public:

    Mesh();

    void addVertex(glm::vec3 vertex);

    void addVertices(std::vector<glm::vec3> vertices);

    void createTriangle(std::array<int, 3> indices);

    void createInstance(glm::mat4 modelMatrix);

    void draw();

private:

    struct VoidWithSize {
        void *data;
        uint size;
    };

    VoidWithSize m_ModelTextures;
                          
    std::vector<Node> m_Vertices;
    std::vector<std::array<uint, 3>> m_Surfaces;

    std::vector<glm::mat4> m_Instances;
    uint m_TextureId;

    std::mt19937 m_rGen;

    void assembleModelTexture();

    void setupTexture();

};