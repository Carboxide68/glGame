#include "mesh.h"

Mesh::Mesh() {

    m_rGen.seed = floor(glfwGetTime() * 100000);
    m_rGen.max = 1000000;
    m_rGen.min = 0;

}

void Mesh::addVertex(glm::vec3 vertex) {
    
    m_Vertices.push_back(Node(m_rGen(), vertex));

}

void Mesh::addVertices(std::vector<glm::vec3> vertices) {
    for (auto i = vertices.begin(); i != vertices.end(); i++) {
        m_Vertices.push_back(Node(m_rGen(), (*i)));
    }
}

void Mesh::createTriangle(std::array<int, 3> indices) {

    std::array<uint, 3> IDs;

    for (int i = 0; i < indices.size(); i++) {
        IDs[i] = m_Vertices[i].getId();
    }

    m_Surfaces.push_back({IDs[0], IDs[1], IDs[2]});
}

void Mesh::assembleModelTexture() {

    m_ModelTextures = {};
    {
    std::vector<float*> tempdata;
    for (auto i = m_Instances.begin(); i != m_Instances.end(); i++) {
        m_ModelTextures.size += 4;
        tempdata.push_back((float*)value_ptr(*i));
    }
    m_ModelTextures.data = (void *)tempdata.data();
    }
}

void Mesh::setupTexture() {


}