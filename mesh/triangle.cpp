#include "triangle.h"

Triangle::Triangle(std::array<uint, 3> IDs, std::array<glm::vec3, 3> normal) {
    m_IDs = IDs;
    m_Normal = normal;
}

Triangle::Triangle(std::array<uint, 3> IDs) {
    Triangle(IDs, {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});
}

SurfaceGeometry Triangle::assembleGeometry(const std::vector<Node> &nodeList) {

    std::vector<std::array<float,3>> temp;

    for (uint i = 0; i < nodeList.size(); i++) {
        if (containsID(nodeList[i].ID)) {
            temp.push_back({nodeList[i].pos[0], nodeList[i].pos[1], nodeList[i].pos[2]});
        }
    }
    if (temp.size() != 3) {
        printf("Geometry size is %d not 3! NodeList length is %d\n", temp.size(), nodeList.size());
        return {};
    }
    return {{temp[0][0], temp[0][1], temp[0][2]}, {m_Normal[0].x, m_Normal[0].y, m_Normal[0].z}, {temp[1][0], temp[1][1], temp[1][2]}, 
            {m_Normal[1].x, m_Normal[1].y, m_Normal[1].z}, {temp[2][0], temp[2][1], temp[2][2]}, {m_Normal[2].x, m_Normal[2].y, m_Normal[2].z}
    };
}

ErrWithData<std::array<uint, 3>> Triangle::getIndices(const std::vector<Node> &nodeList) {
    std::array<uint, 3> indices;
    uint c = 0;
    for (uint i = 0; i < nodeList.size(); i++) {
        if (containsID(nodeList[i].ID)) {
            if (c < 3) {
                indices[c] = i;
            }
            c++;
        }
    }
    if (c != 3)
        return {{0, 0, 0}, false};
        
    else 
        return {indices, true};
}

ErrWithData<std::array<glm::vec3, 3>> Triangle::getVertices(const std::vector<Node> &nodeList) {
    std::array<glm::vec3, 3> vertices;
    uint count = 0;
    for (auto i = nodeList.begin(); i < nodeList.end(); i++) {
        for (uint c = 0; c < 3; c++) {
            if ((*i).ID == m_IDs[c]) {
                vertices[c] = (*i).pos;
                count++;
            }
        }
    }

    if (count != 3)
        return {{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)}, false};
        
    else
        return {vertices, true};
}

bool Triangle::containsID(uint ID) {
    for (int i = 0; i < 3; i++) {
        if (m_IDs[i] == ID) {
            return true;
        }
    }
    return false;
}

void Triangle::generateNormal(const std::vector<Node> &nodeList) {
    std::array<glm::vec3, 3> coords = getVertices(nodeList).data;
    glm::vec3 normal = glm::normalize(glm::cross((coords[1] - coords[0]), (coords[2] - coords[0])));
    m_Normal = {normal, normal, normal};
}