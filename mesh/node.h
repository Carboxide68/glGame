#pragma once

#include "../common/common.h"

class Node {

public:

    Node(uint id, glm::vec3 vector);

    uint getId();

    glm::vec3 getVertex();

private:

    uint m_Id;
    glm::vec3 m_Vector; 

};