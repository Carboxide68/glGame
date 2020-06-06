#pragma once

#include "../common/common.h"

class Polygon {
public:

    Polygon(std::vector<glm::vec3&> pos, std::vector<glm::vec2&> texCoords);
    Polygon(std::vector<glm::vec3&> pos, std::vector<glm::vec2&> texCoords, glm::vec3 normal);

    inline void assignNormal(const glm::vec3 normal) {m_Normal = normal;};

    void generateNormal();

private:

    bool CheckForErrors() const;

    std::vector<glm::vec2&> m_TexCoords;    //If there are TexCoords the count should be the same as
    std::vector<glm::vec3&> m_Pos;          //the number of positions

    glm::vec3 m_Normal;                     //Normal is assumed to be 0, 0, 0

};