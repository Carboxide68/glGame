#include "mesh.h"


Mesh::Mesh(ModelID ID) {
    this->ID = ID;
    char temp[32];
    sprintf(temp, "Mesh %d", (int)ID.mesh);
    Name = temp;
    m_GroupMap.push_back({"_CO_STANDARD", 0});
}

void Mesh::loadData(std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords) {
    Vertices = positions;
    TexCoords = texCoords;
}

void Mesh::loadData(std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords) {
    const size_t posSize = positions.size();
    for (int i = 0; i < (int)posSize; i++) {
        Vertices.push_back({positions[i][0], positions[i][1], positions[i][1]});
    }
    const size_t texSize = texCoords.size();
    for (int i = 0; i < (int)texSize; i++) {
        TexCoords.push_back({texCoords[i][0], texCoords[i][1]});
    }
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<std::vector<glm::vec3>> normals) {
    const size_t positionIndexSize1 = positionIndices.size();
    uint texMult = (texIndices.size() < positionIndexSize1) ? 0.0f : 1.0f;
    Polygons.reserve(positionIndexSize1);
    auto polygonStart = Polygons.end() - Polygons.begin();
    
    for (int i = 0; i < (int)positionIndexSize1; i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        for (int x = 0; x < (int)positionIndexSize; x++) {
            tempPositions.push_back(&Vertices[positionIndices[i][x]]);
            tempTexCoords.push_back(&TexCoords[texIndices[i * texMult][x * texMult]]);
        }
        if (normals[0][0] != glm::vec3(0)) {
            Polygons.push_back(Polygon(GenerateID(), tempPositions, tempTexCoords, normals[i]));
        } else {
            Polygons.push_back(Polygon(GenerateID(), tempPositions, tempTexCoords));
        }
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = Polygons.begin() + polygonStart; i != Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

std::vector<Polygon*> Mesh::createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<glm::vec3> normals, std::vector<std::vector<uint>> normalIndices) {
    const size_t positionIndexSize1 = positionIndices.size();
    uint texMult = (texIndices.size() < positionIndexSize1) ? 0.0f : 1.0f;
    Polygons.reserve(positionIndexSize1);
    auto polygonStart = Polygons.end() - Polygons.begin();
    
    for (int i = 0; i < (int)positionIndexSize1; i++) {
        const size_t positionIndexSize = positionIndices[i].size();
        std::vector<glm::vec3*> tempPositions;
        std::vector<glm::vec2*> tempTexCoords;
        std::vector<glm::vec3> tempNormals;
        tempPositions.reserve(positionIndexSize);
        tempTexCoords.reserve(positionIndexSize);
        tempNormals.reserve(positionIndexSize);
        for (int x = 0; x < (int)positionIndexSize; x++) {
            tempPositions.push_back(&Vertices[positionIndices[i][x]]);
            tempTexCoords.push_back(&TexCoords[texIndices[i * texMult][x * texMult]]);
            tempNormals.push_back(normals[normalIndices[i][x]]);
        }
        Polygons.push_back(Polygon(GenerateID(), tempPositions, tempTexCoords, tempNormals));
    }
    std::vector<Polygon*> pointerVector;
    for (auto i = Polygons.begin() + polygonStart; i != Polygons.end(); i++) {
        pointerVector.push_back(&(*i));
    }
    return pointerVector;
}

void Mesh::addGroup(std::string groupName, uint begin, uint end) {

    std::vector<std::pair<std::string, uint>> tmp = {{groupName, begin}, {"_CO_STANDARD", end}};

    for (int i = 0; i < m_GroupMap.size(); i++) {
        if (m_GroupMap[i].second > begin) {
            m_GroupMap.insert(m_GroupMap.begin() + i - 1, tmp.begin(), tmp.end());
            return;
        }
    }
    m_GroupMap.insert(m_GroupMap.end(), tmp.begin(), tmp.end());
}

void Mesh::addGroup(std::string groupName) {
    m_GroupMap = {{"_CO_STANDARD", 0}, {groupName, 0}, {"_CO_STANDARD", Polygons.size()}};
}

std::vector<StandardVertex> Mesh::getStandardVertices() const {
    std::vector<StandardVertex> vertices;
    for (int i = 0; i < (int)Polygons.size(); i++) {
        std::vector<StandardVertex> tempVertices = Polygons[i].getStandardVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

void Mesh::UpdatePolygonMap() {
    const size_t polygonSize = Polygons.size();
    m_PolygonMap.reserve(polygonSize + 1);
    m_PolygonMap.push_back(0); //First index will be zero, second will be after the first is done etc
    uint offset = 0;
    for (int i = 0; i < (int)polygonSize; i++) {
        offset += Polygons[i].m_Pos.size();
        m_PolygonMap.push_back(offset); //Change this to a per-model standard
    }
}

void Mesh::UpdatePolygonNormals() {
    const size_t polygonSize = Polygons.size();
    for (int i = 0; i < (int)polygonSize; i++) {
        Polygons[i].generateNormal();
    }
}

void Mesh::UpdateGroupMap() {

    if (m_GroupMap.size() == 1) {
        m_GroupMap.push_back({"_CO_STANDARD", Polygons.size()});
    } else if (m_GroupMap.back().first == "_CO_STANDARD") {
        m_GroupMap.back().second = Polygons.size();
    } else {
        m_GroupMap.push_back({"_CO_STANDARD", Polygons.size()});
    }
}

void Mesh::update(bool force) {
    UpdatePolygonMap();
    UpdatePolygonNormals();
    UpdateGroupMap();
}