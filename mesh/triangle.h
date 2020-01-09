#pragma once

#include "../common/common.h"


class MeshTriangle {

public:

    MeshTriangle(std::array<uint, 3> IDs);

private:

    std::array<uint, 3> m_IDs;

};