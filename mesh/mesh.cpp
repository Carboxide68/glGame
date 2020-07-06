#include "mesh.h"


Mesh::Mesh(ModelID ID, std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords) {
    m_LastID = ID;
    vertices = positions;
    if (texCoords.size() == positions.size()) {
        this->texCoords = texCoords;
    } else {
        this->texCoords = {glm::vec2(0)};
    }
}

Mesh::Mesh(ModelID ID, std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords) {
    m_LastID = ID;
    const size_t posSize = positions.size();
    for (int i = 0; i < (int)posSize; i++) {
        this->vertices.push_back({positions[i][0], positions[i][1], positions[i][1]});
    }
    const size_t texSize = texCoords.size();
    for (int i = 0; i < (int)texSize; i++) {
        this->texCoords.push_back({texCoords[i][0], texCoords[i][1]});
    }
    if (texSize == 0) {
        this->texCoords = {glm::vec2(0)};
    }
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<std::vector<glm::vec3>> normals) {
    const size_t positionIndexSize1 = positionIndices.size();
    uint texMult = (texIndices.size() == positionIndexSize1) ? 1 : 0;
    m_Polygons.reserve(positionIndexSize1);
    auto polygonStart = m_Polygons.end() - m_Polygons.begin();
    
    for (int i = 0; i < (int)positionIndexSize1; i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        for (int x = 0; x < (int)positionIndexSize; x++) {
            tempPositions.push_back(&vertices[positionIndices[i][x]]);
            if (texMult) {
                tempTexCoords.push_back(&texCoords[texIndices[i][x]]);
            }
        }
        if ((int)normals.size() != 0) {
            m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords, normals[i]));
        } else {
            m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords));
        }
        m_LastID.polygon++;
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = m_Polygons.begin() + polygonStart; i != m_Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<glm::vec3> normals, std::vector<std::vector<uint>> normalIndices) {
    const size_t positionIndexSize1 = positionIndices.size();
    uint texMult = (texIndices.size() < positionIndexSize1) ? 0 : 1;
    m_Polygons.reserve(positionIndexSize1);
    auto polygonStart = m_Polygons.end() - m_Polygons.begin();
    
    for (int i = 0; i < (int)positionIndexSize1; i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        std::vector<glm::vec3> tempNormals;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        tempNormals.reserve(positionIndexSize);
        for (int x = 0; x < (int)positionIndexSize; x++) {
            tempPositions.push_back(&vertices[positionIndices[i][x]]);
            if (texMult) {
                tempTexCoords.push_back(&texCoords[texIndices[i][x]]);
            }
            tempNormals.push_back(normals[normalIndices[i][x]]);
        }
        m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords, tempNormals));
        m_LastID.polygon++;
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = m_Polygons.begin() + polygonStart; i != m_Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

std::vector<StandardVertex> Mesh::getStandardVertices() const {
    std::vector<StandardVertex> standardVertices;
    for (int i = 0; i < (int)m_Polygons.size(); i++) {
        std::vector<StandardVertex> tempVertices = m_Polygons[i].getStandardVertices();
        standardVertices.insert(standardVertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return standardVertices;
}

void Mesh::UpdatePolygonMap() {
    const size_t polygonSize = m_Polygons.size();
    m_PolygonMap.reserve(polygonSize + 1);
    m_PolygonMap.push_back(0); //First index will be zero, second will be after the first is done etc
    uint offset = 0;
    for (int i = 0; i < (int)polygonSize; i++) {
        offset += m_Polygons[i].m_Pos.size();
        m_PolygonMap.push_back(offset); //Change this to a per-model standard
    }
}

void Mesh::UpdatePolygonNormals(bool force) {
    const size_t polygonSize = m_Polygons.size();
    for (int i = 0; i < (int)polygonSize; i++) {
        m_Polygons[i].generateNormal(force);
    }
}

void Mesh::update(bool force) {
    UpdatePolygonMap();
    UpdatePolygonNormals(force);
}