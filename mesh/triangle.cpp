#include "triangle.h"

Triangle::Triangle(std::array<uint, 3> IDs, glm::vec3 normal) {
    m_IDs = IDs;
    m_Normal = normal;
    m_VectorReferences = {};
}

int Triangle::updateIndices(std::vector<Node> &nodeList) {

    m_NodeListStart = &nodeList;
    uint c = 0;
    for (uint i = 0; i < nodeList.size(); i++) {
        int idExist = containsID(nodeList[i].ID);
        if (idExist >= 0) {
            m_VectorReferences[idExist] = &nodeList[i];
            c++;
        }
    }
    return c;
}

int Triangle::containsID(uint ID) {
    for (uint i = 0; i < 3; i++) {
        if (m_IDs[i] == ID) {
            return i;
        }
    }
    return -1;
}

ErrWithData<std::array<glm::vec3, 3>> Triangle::getVertices() {
    std::array<glm::vec3, 3> temp;
    for (uint i = 0; i < 3; i++) {
        if(containsID(m_VectorReferences[i]->ID) == i) {
            temp[i] = m_VectorReferences[i]->pos;
        }
        else {
            return {{}, -1};
        }
    }
    return {temp, 1};
}

ErrWithData<std::array<uint, 3>> Triangle::getIndices() {
        std::array<uint, 3> temp;
    for (uint i = 0; i < 3; i++) {
        if(m_VectorReferences[i]->ID == m_IDs[i]) {
            temp[i] = ((size_t)(m_VectorReferences[i]) - (size_t)(m_NodeListStart))/sizeof(Node);
        }
        else {
            return {{}, -1};
        }
    }
    return {temp, 1};
}

SurfaceGeometry Triangle::assembleGeometry(const std::vector<Node> &nodeList) {

    std::vector<std::array<float,3>> temp;

    for (uint i = 0; i < nodeList.size(); i++) {
        if (containsID(nodeList[i].ID) >= 0) {
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