#pragma once

#include "../common/common.h"
#include "modelCommon.h"

class Polygon {
public:

    Polygon(std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords);
    Polygon(std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords, glm::vec3 normal);

    inline void assignNormal(const glm::vec3 normal) {m_Normals[0] = normal;};

    void generateNormal();

    std::vector<uint> assembleIndices();
    std::vector<Triangle> assembleTriangleMesh();

    std::vector<StandardVertex> getStandardVertices() const;

    inline uint getTriangleCount() const {return m_Pos.size() - 2;} 

    ModelID ID;

    friend class Mesh;

private:

    bool CheckForErrors() const;

    std::vector<glm::vec2*> m_TexCoords;    //If there are TexCoords the count should be the same as
    std::vector<glm::vec3*> m_Pos;          //the number of positions

    std::vector<glm::vec3> m_Normals;       //Only using first position of normal for now, will make some changes in future

};