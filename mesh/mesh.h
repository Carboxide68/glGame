#pragma once

#include "../common/common.h"
#include "modelCommon.h"

class Mesh {
public:

    Mesh(ulong ID);

    std::vector<Triangle> assembleToTriangleMesh();

private:

    ModelID GenerateID();

    std::vector<Polygon> m_Polygons;
    std::vector<uint> m_PolygonMap; //Index N is index (N - 1) + PolygonTriangleCount

    std::vector<glm::vec3> m_Positions; //Vertex data is stored on a per-mesh basis
    std::vector<glm::vec2> m_Texcoords;

    ModelID m_MeshID;

};