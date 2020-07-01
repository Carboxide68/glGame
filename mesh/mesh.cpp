#include "mesh.h"


Mesh::Mesh(ModelID ID, std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords) {
    m_LastID = ID;
    m_Vertices = positions;
    m_TexCoords = texCoords;
}

Mesh::Mesh(ModelID ID, std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords) {
    m_LastID = ID;
    const size_t posSize = positions.size();
    for (int i = 0; i < posSize; i++) {
        m_Vertices.push_back({positions[i][0], positions[i][1], positions[i][1]});
    }
    const size_t texSize = texCoords.size();
    for (int i = 0; i < texSize; i++) {
        m_TexCoords.push_back({texCoords[i][0], texCoords[i][1]});
    }
    if (texSize < 1) {
        m_TexCoords.push_back({0, 0});
    }
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<std::vector<glm::vec3>> normals) {
    uint texMult = (texIndices.size() < positionIndices.size()) ? 0.0f : 1.0f;
    m_Polygons.reserve(positionIndices.size());
    auto polygonStart = m_Polygons.end();
    
    for (int i = 0; i < positionIndices.size(); i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        for (int x = 0; x < positionIndexSize; x++) {
            tempPositions.push_back(&m_Vertices[positionIndices[i][x]]);
            tempTexCoords.push_back(&m_TexCoords[texIndices[i * texMult][x * texMult]]);
        }
        if (normals[0][0] != glm::vec3(0)) {
            m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords, normals[i]));
        } else {
            m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords));
        }
        m_LastID.polygon++;
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = polygonStart; i != m_Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<glm::vec3> normals, std::vector<std::vector<uint>> normalIndices) {
    uint texMult = (texIndices.size() < positionIndices.size()) ? 0.0f : 1.0f;
    m_Polygons.reserve(positionIndices.size());
    auto polygonStart = m_Polygons.end();
    
    for (int i = 0; i < positionIndices.size(); i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        std::vector<glm::vec3> tempNormals;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        tempNormals.reserve(positionIndexSize);
        for (int x = 0; x < positionIndexSize; x++) {
            tempPositions.push_back(&m_Vertices[positionIndices[i][x]]);
            tempTexCoords.push_back(&m_TexCoords[texIndices[i * texMult][x * texMult]]);
            tempNormals.push_back(normals[normalIndices[i][x]]);
        }
        m_Polygons.push_back(Polygon(m_LastID, tempPositions, tempTexCoords, tempNormals));
        m_LastID.polygon++;
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
    for (int i = 0; i < m_Polygons.size(); i++) {
        std::vector<StandardVertex> tempVertices = m_Polygons[i].getStandardVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

void Mesh::UpdatePolygonMap() {
    const size_t polygonSize = m_Polygons.size();
    m_PolygonMap.reserve(polygonSize + 1);
    m_PolygonMap.push_back(0); //First index will be zero, second will be after the first is done etc
    uint offset = 0;
    for (int i = 0; i < polygonSize; i++) {
        offset += m_Polygons[i].m_Pos.size();
        m_PolygonMap.push_back(offset); //Change this to a per-model standard
    }
}

void Mesh::UpdatePolygonNormals() {
    const size_t polygonSize = m_Polygons.size();
    for (int i = 0; i < polygonSize; i++) {
        m_Polygons[i].generateNormal();
    }
}

void Mesh::update() {
    UpdatePolygonMap();
    UpdatePolygonNormals();
}