#include "polygon.h"

Polygon::Polygon(std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords) : Polygon(pos, texCoords, {0, 0, 0}){}

Polygon::Polygon(std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords, glm::vec3 normal) {

    m_Pos.resize(pos.size());
    for (int i = 0; i < pos.size(); i++) {
        m_Pos[i] = &pos[i];
    }

    m_TexCoords.resize(texCoords.size());
    for (int i = 0; i < texCoords.size(); i++) {
        m_TexCoords[i] = &texCoords[i];
    }
    m_Normals.resize(1);
    m_Normals[0] = normal;
}

void Polygon::generateNormal() {
    if (!CheckForErrors()) {
        return;
    }
    m_Normals[0] = glm::normalize(glm::cross(*m_Pos[1] - *m_Pos[0], *m_Pos[2] - *m_Pos[0]));
}

bool Polygon::CheckForErrors() const {

    if (m_Pos.size() < 3) {
        printf("Polygon is not complete! Only %lu corners!\n", m_Pos.size());
        return false;
    }

    if (m_TexCoords.size() != 0 && m_TexCoords.size() != m_Pos.size()) {
        printf("Texture Coordinates don't match specifications!");
        return false;
    }
    return true;
}

std::vector<StandardVertex> Polygon::getStandardVertices() const {

    std::vector<StandardVertex> vertices;
    vertices.reserve(m_Pos.size());

    float usingTex = (m_TexCoords.size() < 1) ? 0.0f : 1.0f;
    for (int i = 0; i < m_Pos.size(); i++) {
        vertices.push_back({*m_Pos[i], m_Normals[0], *m_TexCoords[i * usingTex]});
    }
    return vertices;
}

std::vector<uint> Polygon::assembleIndices() {

    std::vector<uint> indices;

    if (!CheckForErrors()) {
        return {};
    }

    for (int i = 2; i < m_Pos.size(); i++) {
        indices.push_back(0);
        indices.push_back(i - 1);
        indices.push_back(i);
    }

    return indices;
}

std::vector<Triangle> Polygon::assembleTriangleMesh() {

    std::vector<Triangle> triangles;

    if (!CheckForErrors()) {
        return {};
    }

    float usingTex = (m_TexCoords.size() < 1) ? 0.0f : 1.0f;
    for (int i = 2; i < m_Pos.size(); i++) {
        triangles.push_back({
            *m_Pos[0],
            *m_TexCoords[0 * usingTex],
            m_Normals[0],
            *m_Pos[i - 1],
            *m_TexCoords[(i - 1) * usingTex],
            m_Normals[0],
            *m_Pos[i],
            *m_TexCoords[i * usingTex],
            m_Normals[0]
        });
    }
    return triangles;
}