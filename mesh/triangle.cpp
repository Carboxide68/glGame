#include "triangle.h"

Triangle::Triangle(std::array<uint, 3> IDs, glm::vec3 normal) {
    m_IDs = IDs;
    m_Normal = normal;
}

SurfaceGeometry Triangle::assembleGeometry(const std::vector<Node> &nodeList) {

    std::vector<std::array<float,3>> temp;

    for (uint i = 0; i < nodeList.size(); i++) {
        if (containsID(nodeList[i].ID)) {
            temp.push_back({nodeList[i].pos[0], nodeList[i].pos[1], nodeList[i].pos[2]});
        }
    }
    if (temp.size() != 3) {
        return {};
    }
    return {{temp[0][0], temp[0][1], temp[0][2]}, {m_Normal.x, m_Normal.y, m_Normal.z}, {temp[1][0], temp[1][1], temp[1][2]}, 
            {m_Normal.x, m_Normal.y, m_Normal.z}, {temp[2][0], temp[2][1], temp[2][2]}, {m_Normal.x, m_Normal.y, m_Normal.z}
    };
}