#pragma once

#include "../common/common.h"


class Triangle {

public:

    Triangle(std::array<uint, 3> IDs, glm::vec3 normal);
    
private:

    std::array<uint, 3> m_IDs;

    glm::vec3 m_Normal;

};