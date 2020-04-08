#pragma once

#include "../common/common.h"
#include "node.h"

struct SurfaceGeometry {

    float node1[3];
    float norm1[3];
    float node2[3];
    float norm2[3];
    float node3[3];
    float norm3[3];

};

// TODO:
// Implement ability to relate multiple nodeLists for more versatile meshes

class Triangle {

public:

    Triangle(std::array<uint, 3> IDs, const std::vector<Node> &nodeList, std::array<glm::vec3, 3> normal);
    Triangle(std::array<uint, 3> IDs, const std::vector<Node> &nodeList);
    Triangle(std::array<uint, 3> IDs);

    SurfaceGeometry assembleGeometry(const std::vector<Node> &nodeList);

    ErrWithData<std::array<glm::vec3, 3>> getVertices();

    void updateVertices(const std::vector<Node> &nodeList);

    bool containsID(uint ID); //Returns a nullpointer if it doesn't contain that element
    void generateNormal(const std::vector<Node> &nodeList);
    void assignNormals(const std::array<glm::vec3, 3> normals);

private:

    std::array<uint, 3> m_IDs;

    std::array<glm::vec3, 3> m_Normal;

};