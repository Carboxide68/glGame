#include "polygon.h"

Polygon::Polygon(std::vector<glm::vec3&> pos, std::vector<glm::vec2&> texCoords) : Polygon(pos, texCoords, {0, 0, 0}){}

Polygon::Polygon(std::vector<glm::vec3&> pos, std::vector<glm::vec2&> texCoords, glm::vec3 normal) {

    m_Pos = pos;
    m_TexCoords = texCoords;
    m_Normal = normal;

}

void Polygon::generateNormal() {
    if (!CheckForErrors) {
        return;
    }
    m_Normal = glm::normalize(glm::cross(m_Pos[1] - m_Pos[0], m_Pos[2] - m_Pos[0]));
}

bool Polygon::CheckForErrors() const {

    if (m_Pos.size() < 3) {
        printf("Polygon is not complete! Only %d corners!\n", m_Pos.size());
        return false;
    }

    if (m_TexCoords.size() != 0 && m_TexCoords.size() != m_Pos.size()) {
        printf("Texture Coordinates are don't match specifications!");
        return false;
    }
}