#include "mesh.h"


Mesh::Mesh(ModelID ID) {
    this->ID = ID;
    Enabled = true;
    char temp[32];
    sprintf(temp, "Mesh %d", (int)ID.mesh);
    Name = temp;
    m_GroupMap = {};
    m_PolygonMap = {};
    Polygons = {};
    Vertices = {};
    TexCoords = {};
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
            if (texMult) {
                tempTexCoords.push_back(&TexCoords[texIndices[i * texMult][x * texMult]]);
            }
        }
        if ((int)normals.size() != 0) {
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

    std::vector<Groupspan> tmp = {{std::shared_ptr<std::string>(new std::string(groupName)), begin, end}};

    for (int i = 0; i < (const int)m_GroupMap.size(); i++) {
        if (m_GroupMap[i].begin > begin) {
            m_GroupMap.insert(m_GroupMap.begin() + i - 1, tmp.begin(), tmp.end());
            return;
        }
    }
    m_GroupMap.insert(m_GroupMap.end(), tmp.begin(), tmp.end());
}

void Mesh::addGroup(std::string groupName) {
    m_GroupMap = {{std::shared_ptr<std::string>(new std::string(groupName)), 0, (uint)Polygons.size()}};
}

std::vector<Mesh::Groupspan> Mesh::getGroup(std::string name) {
    std::vector<Groupspan> group;
    if (name == STANDARD_NAME) {
        const size_t temp = m_GroupMap.size();
        if (temp == 0) {
            return {{std::make_shared<std::string>(STANDARD_NAME), 0, (uint)Polygons.size()}};
        }
        for (int i = 0; i < (int)temp; i++) {
            if (i == 0) {
                if (0 != m_GroupMap[i].begin) {
                    group.push_back({std::make_shared<std::string>(STANDARD_NAME), 0, m_GroupMap[0].begin});
                }
            }
            else if (i == (int)(temp - 1)) {
                if ((uint)Polygons.size() != m_GroupMap[i].end) {
                    group.push_back({std::make_shared<std::string>(STANDARD_NAME), m_GroupMap[i].end, (uint)Polygons.size()});
                }
            }
            else if (m_GroupMap[i].end != m_GroupMap[i+1].begin) {
                group.push_back({std::make_shared<std::string>(STANDARD_NAME), m_GroupMap[i].end, m_GroupMap[i+1].begin});
            }
        }
    } else {
        for (int i = 0; i < (const int)m_GroupMap.size(); i++) {
            if (*(m_GroupMap[i].name) == name) {
                group.push_back(m_GroupMap[i]);
            }
        }
    }
    return group;
}

std::vector<StandardVertex> Mesh::getStandardVertices() const {
    std::vector<StandardVertex> vertices;
    for (int i = 0; i < (int)Polygons.size(); i++) {
        std::vector<StandardVertex> tempVertices = Polygons[i].getStandardVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

void Mesh::updatePolygonMap() {
    const size_t polygonSize = Polygons.size();
    m_PolygonMap.reserve(polygonSize + 1);
    m_PolygonMap.push_back(0); //First index will be zero, second will be after the first is done etc
    uint offset = 0;
    for (int i = 0; i < (int)polygonSize; i++) {
        offset += Polygons[i].m_Pos.size();
        m_PolygonMap.push_back(offset); //Change this to a per-model standard
    }
}

void Mesh::updatePolygonNormals(bool force) {
    const size_t polygonSize = Polygons.size();
    for (int i = 0; i < (int)polygonSize; i++) {
        Polygons[i].generateNormal(force);
    }
}

void Mesh::update() {
    updatePolygonMap();
    updatePolygonNormals();
}