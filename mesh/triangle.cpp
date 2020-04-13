#include "triangle.h"



Triangle::Triangle(std::array<uint, 3> IDs, std::vector<Node> &nodeList, std::array<glm::vec3, 3> normal) {
    Triangle(IDs, normal);
    updateNodes(nodeList);
}

Triangle::Triangle(std::array<Node*, 3> nodes, std::array<glm::vec3, 3> normal) {
    m_NodeReferences[0] = nodes[0];
    m_NodeReferences[1] = nodes[1];
    m_NodeReferences[2] = nodes[2];
    m_IDs[0] = m_NodeReferences[0]->ID;
    m_IDs[1] = m_NodeReferences[1]->ID;
    m_IDs[2] = m_NodeReferences[2]->ID;
    m_Normal = normal;
}

Triangle::Triangle(std::array<Node*, 3> nodes) {
    m_NodeReferences[0] = nodes[0];
    m_NodeReferences[1] = nodes[1];
    m_NodeReferences[2] = nodes[2];
    m_IDs[0] = m_NodeReferences[0]->ID;
    m_IDs[1] = m_NodeReferences[1]->ID;
    m_IDs[2] = m_NodeReferences[2]->ID;
    m_Normal = {glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0)};
}

Triangle::Triangle(std::array<uint, 3> IDs, std::vector<Node> &nodeList) {
    Triangle(IDs, {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});
    updateNodes(nodeList);
}

Triangle::Triangle(std::array<uint, 3> IDs, std::array<glm::vec3, 3> normal) {
    m_IDs[0] = IDs[0];
    m_IDs[1] = IDs[1];
    m_IDs[2] = IDs[2];
    m_Normal = normal;
    m_NodeReferences = {};
}

Triangle::Triangle(std::array<uint, 3> IDs) {
    Triangle(IDs, {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});
}

void Triangle::updateNodes(std::vector<Node> &nodeList) {
    uint c = 0;

    m_NodeListStart = &nodeList;
    for (int i = 0; i < m_NodeReferences.size(); i++) {
        if(!((m_NodeReferences[i])->ID == m_IDs[i])) {
            c++;
            // printf("They are not the same, reference: %u; IDs: %u\n", m_VectorReferences[i]->ID, m_IDs[i]);
        }
    }

    if (c == 0) {
        return;
    }

    for (uint i = 0; i < nodeList.size(); i++) {

        int idExist = containsID(nodeList[i].ID);
        if (idExist != -1) {
            printf("Changed node id to %u from %u!\n", nodeList[i].ID, m_NodeReferences[idExist]->ID);
            m_NodeReferences[idExist] = &nodeList[i];
            c--;
        }
    }
    if (c != 0) {
        // printf("Didn't manage to fetch %d nodes from the nodeList!\n", c);
    }
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
        if ((m_NodeReferences[i])->ID == m_IDs[i]) {
            temp[i] = (m_NodeReferences[i])->pos;
        } else {
            printf("Get vertices failed!\n");
            return {{}, -1};
        }
    }
    return {temp, 1};
}

ErrWithData<std::array<uint, 3>> Triangle::getIndices() {
    std::array<uint, 3> temp;
    for (uint i = 0; i < 3; i++) {
        if(m_NodeReferences[i]->ID == m_IDs[i]) {
            temp[i] = ((size_t)(m_NodeReferences[i]) - (size_t)(m_NodeListStart))/sizeof(Node);
        }
        else {
            return {{}, -1};
        }
    }
    return {temp, 1};
}

SurfaceGeometry Triangle::assembleGeometry() {

    return {{m_NodeReferences[0]->pos[0], m_NodeReferences[0]->pos[1], m_NodeReferences[0]->pos[2]}, 
            {m_Normal[0].x, m_Normal[0].y, m_Normal[0].z}, 
            {m_NodeReferences[1]->pos[0], m_NodeReferences[1]->pos[1], m_NodeReferences[1]->pos[2]}, 
            {m_Normal[1].x, m_Normal[1].y, m_Normal[1].z},
            {m_NodeReferences[2]->pos[0], m_NodeReferences[2]->pos[1], m_NodeReferences[2]->pos[2]}, 
            {m_Normal[2].x, m_Normal[2].y, m_Normal[2].z}
    };
}

void Triangle::generateNormal() {
    ErrWithData<std::array<glm::vec3, 3>> err = getVertices();
    if (err.error > 0) {
        std::array<glm::vec3, 3> coords = err.data;
        glm::vec3 normal = glm::normalize(glm::cross(coords[1] - coords[0], coords[2] - coords[0]));
        m_Normal = {normal, normal, normal};
    } else {
        printf("Error while getting vertices when generating normals!\n");
    }
}