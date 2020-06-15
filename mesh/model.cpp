#include "model.h"
#include "iostream"
#include "fstream"
#include "modelLoading.h"
#include <map>
#include <algorithm>

Model::Model() {
    // glGenVertexArrays(1, &m_VertexArrayID);
    // glGenBuffers(1, &m_ElementBufferID);
    // glGenBuffers(1, &m_VertexBufferID);
    // glBindVertexArray(m_VertexArrayID);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferID);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    // glBindVertexArray(0);
}

bool Model::loadModel(std::string path) {
    std::vector<glm::vec3> vertices = {glm::vec3(0)};
    std::vector<glm::vec2> texCoords = {glm::vec2(0)};
    std::vector<glm::vec3> normals = {glm::vec3(0)};
    std::vector<Face> faces;

    struct gr {
        std::string name; 
        uint loc;
    };

    struct mat {
        std::string name; 
        uint loc;
    };

    std::vector<gr> meshes; //Group name with a S after are smoothing groups
    std::vector<mat> usingMaterial;
    std::vector<std::string> materialLibs;

    std::vector<Polygon *> polygons;

    std::ifstream myFile;
    myFile.open(path);
    std::string line;
    if (myFile.is_open()) {
        for (std::getline(myFile, line); myFile.good(); std::getline(myFile, line)) {
            for (auto i = line.begin(); i != line.end(); i++) {
                if (*i == ' ') continue;
                switch (tolower(*i)) {
                    case '#':
                        break;
                    case 'v':
                        switch(*(i + 1)) { 
                            case ' ':
                                vertices.push_back(readVertex(line));
                                break;
                            case 't':
                                texCoords.push_back(readTexCoord(line));
                                break;
                            case 'n':
                                normals.push_back(readNormal(line));
                                break;
                        }
                        break;
                    case 'g':
                        meshes.push_back({readMeshName(line), faces.size()});
                        break;
                    case 'f':
                        faces.push_back(readFace(line));
                        break;
                    case 's':
                        if (readSmoothingGroup(line)) {
                            meshes.push_back({meshes.back().name + "S", faces.size()});
                        }
                        break;

                    case 'm':
                        if (line.find("mtllib") != std::string::npos) {
                            materialLibs.push_back(readMateriallib(line));
                        }
                        break;

                    case 'u':
                        if (line.find("usemtl") != std::string::npos) {
                            usingMaterial.push_back({readMaterial(line), faces.size()});
                        }
                        break;

                    default:
                        break;
                }
                break;
            }
        }
        if (myFile.eof()) {
            std::cout << "Successfully loaded model!" << std::endl;
        } else {
            std::cout << "Failed to load model! Error: " << myFile.rdstate() << std::endl;
            return false;
        }
    } else {
        std::cout << "Failed to open file the file " << path << "!" << std::endl;
        return false;
    }

    meshes.push_back({"placeholder", faces.size() - 1}); //Last element in faces
    usingMaterial.push_back({"placeholder", faces.size() - 1}); //Last element in faces
    for (int i = 0; i < meshes.size() - 1; i++) {
        std::vector<Face> meshFaces(faces.begin() + meshes[i].loc, faces.begin() + meshes[i + 1].loc);
        std::vector<glm::vec3> associatedVertices;
        std::vector<glm::vec3> associatedNormals;
        std::vector<glm::vec2> associatedTexCoords;
        std::vector<std::vector<uint>> vertexIndices;
        std::vector<std::vector<uint>> texIndices;
        for (int y = 0; y < meshFaces.size(); y++) {
            for (int x = 0; x < meshFaces[y].vertex.size(); x++) {
                associatedVertices.push_back(vertices[meshFaces[y].vertex[x]]);
                associatedNormals.push_back(normals[meshFaces[y].normal[x]]);
                associatedTexCoords.push_back(texCoords[meshFaces[y].texCoord[x]]);
            }
            vertexIndices.push_back(meshFaces[y].vertex);
            texIndices.push_back(meshFaces[y].texCoord);
        }
        m_Meshes.push_back(Mesh(m_LastID, associatedVertices, associatedTexCoords));
        auto tempPolygons = m_Meshes[i].createPolygons(vertexIndices, texIndices, associatedNormals);
        polygons.insert(polygons.end(), tempPolygons.begin(), tempPolygons.end());
        m_Meshes[i].setName(meshes[i].name);
        m_LastID.mesh += 1;
    }
    { //Change this for loading material libs.
    std::vector<std::string> names;
    for (int i = 0; i < usingMaterial.size(); i++) {
        names.push_back(usingMaterial[i].name);
    }
    std::sort(names.begin(), names.end());
    auto last = std::unique(names.begin(), names.end());
    names.erase(last, names.end());
    for (int i = 0; i < names.size(); i++) {
        m_Groups.push_back(Group(*this));
        m_Groups.back().setName(names[i]);
    }
    };
    std::map<std::string, Group*> groupMap;
    for (int i = 0; i < m_Groups.size(); i++) {
        groupMap[m_Groups[i].getName()] = &m_Groups[i];
    }
    for (int i = 0; i < usingMaterial.size() - 1; i++) {
        auto start = polygons.begin() + usingMaterial[i].loc;
        auto end = polygons.begin() + usingMaterial[i + 1].loc;
        auto polygonList = std::vector<Polygon*>(start, end);
        groupMap[usingMaterial[i].name]->addPolygons(polygonList);
    }
    return true;
}

void Model::draw() { //Assumes a shader is bound
    glBindVertexArray(m_VertexArrayID);
    uint drawPos = 0;
    for (int i = 0; i < m_Groups.size(); i++) {
        uint temp = m_Groups[i].indexCount();
        m_Groups[i].bindMaterial();
        glDrawElements(GL_TRIANGLES, temp, GL_UNSIGNED_INT, (void *)drawPos);
        drawPos += temp;
    }
}

void Model::update() {

    UpdateMeshMap();
    glBindVertexArray(m_VertexArrayID);
    std::vector<StandardVertex> vertices;
    std::vector<uint> indices;
    for (int i = 0; i < m_Groups.size(); i++) {
        auto temp = m_Groups[i].getIndices();
        indices.insert(indices.end(), temp.begin(), temp.end());
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), (void*)indices.data(), GL_STATIC_DRAW);
    for (int i = 0; i < m_Meshes.size(); i++) {
        auto temp = m_Meshes[i].getStandardVertices();
        vertices.insert(vertices.end(), temp.begin(), temp.end());
    }
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * STANDARD_VERTEX_BYTE_SIZE, (void*)vertices.data(), GL_STATIC_DRAW);
}

void Model::optimizieMeshes() {
    
}

void Model::UpdateMeshMap() {
    m_MeshMap.resize(m_Meshes.size() + 1);
    m_MeshMap[0] = 0;
    for (int i = 0; i < m_Meshes.size(); i++) {
        m_MeshMap[i + 1] = m_Meshes[i].m_PolygonMap.back();
    }
}