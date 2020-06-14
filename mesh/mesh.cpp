#include "mesh.h"


Mesh::Mesh(ModelID ID, std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords) {
    m_MeshID = ID;
    m_LastID = ID;
    m_Positions = positions;
    m_TexCoords = texCoords;
}

Mesh::Mesh(ModelID ID, std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords) {
    m_MeshID = ID;
    m_LastID = ID;
    uint texMult = (texCoords.size() < 1) ? 0.0f : 1.0f;
    for (int i = 0; i < positions.size(); i++) {
        m_Positions[i] = glm::vec3(positions[i][0], positions[i][1], positions[i][2]);
        m_TexCoords[i * texMult] = glm::vec2(positions[i * texMult][0], positions[i * texMult][1]);
    }
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<glm::vec3> normals) {
    uint texMult = (texIndices.size() < positionIndices.size()) ? 0.0f : 1.0f;
    m_Polygons.reserve(positionIndices.size());
    auto polygonStart = m_Polygons.end();
    if (normals[0] == glm::vec3(0)) {
        for (int i = 0; i < positionIndices.size(); i++) {
            std::vector<glm::vec3> tempPositions;
            std::vector<glm::vec2> tempTexCoords;
            tempPositions.resize(positionIndices[i].size());
            tempTexCoords.resize(positionIndices[i].size());
            for (int x = 0; x < positionIndices[i].size(); x++) {
                tempPositions[x] = m_Positions[positionIndices[i][x]];
                tempTexCoords[x] = m_TexCoords[texIndices[i * texMult][x * texMult]];
            }
            m_Polygons.push_back(Polygon(tempPositions, tempTexCoords, normals[i]));
        }
    } else {
        for (int i = 0; i < positionIndices.size(); i++) {
            std::vector<glm::vec3> tempPositions;
            std::vector<glm::vec2> tempTexCoords;
            tempPositions.resize(positionIndices[i].size());
            tempTexCoords.resize(positionIndices[i].size());
            for (int x = 0; x < positionIndices[i].size(); x++) {
                tempPositions[x] = m_Positions[positionIndices[i][x]];
                tempTexCoords[x] = m_TexCoords[texIndices[i * texMult][x * texMult]];
            }
            m_Polygons.push_back(Polygon(tempPositions, tempTexCoords));
        }
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = polygonStart; i != m_Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

std::vector<Triangle> Mesh::assembleToTriangleMesh() {
    
}

std::vector<StandardVertex> Mesh::getStandardVertices() const {
    std::vector<StandardVertex> vertices;
    std::vector<StandardVertex> tempVertices;
    for (int i = 0; i < m_Polygons.size(); i++) {
        tempVertices = m_Polygons[i].getStandardVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

void Mesh::UpdatePolygonMap() {
    m_PolygonMap.reserve(m_Polygons.size() + 1);
    m_PolygonMap[0] = 0; //First index will be zero, second will be after the first is done etc
    for (int i = 0; i < m_Polygons.size(); i++) {
        m_PolygonMap[i + 1] = m_Polygons[i].m_Pos.size() * STANDARD_VERTEX_BYTE_SIZE; //Change this to a per-model standard
    }
}