#include "model.h"
#include "iostream"
#include "fstream"
#include "modelLoading.h"
#include <map>
#include <algorithm>

Model::Model() {
    GLCall(glGenVertexArrays(1, &m_VertexArrayID));
    GLCall(glGenBuffers(1, &m_ElementBufferID));
    GLCall(glGenBuffers(1, &m_VertexBufferID));
    GLCall(glBindVertexArray(m_VertexArrayID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
    GLCall(glBindVertexArray(0));
    m_LastID.ID = 0;
}

bool Model::loadModel(std::string path) {
    std::vector<Polygon *> polygons;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    std::vector<gr> meshes; //Group name with a S after are smoothing groups
    std::vector<mat> usingMaterial;
    std::vector<std::string> materialLibs;

    LoadOBJ(vertices, normals, texCoords, faces, meshes, usingMaterial, materialLibs, path);

    LoadMTL(materialLibs);

    for (int i = 0; i < meshes.size() - 1; i++) {
        std::vector<Face> meshFaces(faces.begin() + meshes[i].loc, faces.begin() + meshes[i + 1].loc);

        std::vector<uint> uniqueVertexIndices;
        std::vector<uint> uniqueNormalIndices;
        std::vector<uint> uniqueTexIndices;
        uint vertexCounter = 0;
        uint normalCounter = 0;
        uint texCounter = 0;

        uniqueVertexIndices.resize(vertices.size(), -1);
        uniqueNormalIndices.resize(normals.size(), -1);
        uniqueTexIndices.resize(texCoords.size(), -1);

        std::vector<glm::vec3> associatedVertices;
        std::vector<glm::vec3> associatedNormals;
        std::vector<glm::vec2> associatedTexCoords;

        std::vector<std::vector<uint>> vertexIndices;
        std::vector<std::vector<uint>> texIndices;
        std::vector<std::vector<uint>> normalIndices;
        for (int y = 0; y < meshFaces.size(); y++) {
            vertexIndices.push_back({});
            texIndices.push_back({});
            normalIndices.push_back({});
            for (int x = 0; x < meshFaces[y].vertex.size(); x++) {
                uint &vertexRef = uniqueVertexIndices[meshFaces[y].vertex[x]];
                uint &normalRef = uniqueNormalIndices[meshFaces[y].normal[x]];
                uint &texRef = uniqueTexIndices[meshFaces[y].texCoord[x]];

                if (vertexRef > vertexCounter) { //This is only true if this is the first time we've encountered this index
                    vertexRef = vertexCounter;
                    vertexCounter++;
                    associatedVertices.push_back(vertices[meshFaces[y].vertex[x]]);
                }
                vertexIndices[y].push_back(vertexRef);

                if (normalRef > normalCounter) {
                    normalRef = normalCounter;
                    associatedNormals.push_back(normals[meshFaces[y].normal[x]]);
                    normalCounter++;
                }
                normalIndices[y].push_back(normalRef);

                if (texRef > texCounter) {
                    texRef = texCounter;
                    associatedTexCoords.push_back(texCoords[meshFaces[y].texCoord[x]]);
                    texCounter++;
                }
                texIndices[y].push_back(texRef);
            }
        }

        m_Meshes.push_back(Mesh(m_LastID, associatedVertices, associatedTexCoords));
        auto tempPolygons = m_Meshes[i].createPolygons(vertexIndices, texIndices, associatedNormals, normalIndices);
        polygons.insert(polygons.end(), tempPolygons.begin(), tempPolygons.end());
        m_Meshes[i].setName(meshes[i].name);
        m_LastID.mesh += 1;
    }
    
    std::map<std::string, Group*> groupMap;
    for (int i = 0; i < m_Groups.size(); i++) {
        groupMap[m_Groups[i].getName()] = &m_Groups[i];
    }
    for (int i = 0; i < usingMaterial.size() - 1; i++) {
        auto start = polygons.begin() + usingMaterial[i].loc;
        auto end = polygons.begin() + usingMaterial[i + 1].loc;
        auto polygonList = std::vector<Polygon*>(start, end);
        auto group = groupMap[usingMaterial[i].name];
        if (group == 0) {
            group = &m_Groups[0];
        }
        group->addPolygons(polygonList);
    }
    for (int i = 0; i < m_Meshes.size(); i++) {
        m_Meshes[i].update();
    }
    update();
    for (int i = 0; i < m_Groups.size(); i++) {
        m_Groups[i].update();
    }
    return true;
}

void Model::draw(Shader shader) { //Assumes a shader is bound
    GLCall(glBindVertexArray(m_VertexArrayID));
    uint drawPos = 0;
    for (int i = 0; i < m_Groups.size(); i++) {
        uint temp = m_Groups[i].indexCount();
        if (temp != 0) {
            m_Groups[i].bindMaterial(shader);
            GLCall(glDrawElements(GL_TRIANGLES, temp, GL_UNSIGNED_INT, (void *)drawPos));
            drawPos += temp * sizeof(uint);
            G_triangles += temp/3;
        }
    }
    GLCall(glBindVertexArray(0));
}

void Model::update() {

    UpdateMeshMap();
    
}

void Model::optimizieMeshes() {
    
}

void Model::UpdateMeshMap() {
    m_MeshMap.reserve(m_Meshes.size() + 1);
    m_MeshMap.push_back(0);
    uint tot = 0;
    for (int i = 0; i < m_Meshes.size(); i++) {
        tot += m_Meshes[i].m_PolygonMap.back();
        m_MeshMap.push_back(tot);
    }
}

void Model::loadToBuffer() {
    GLCall(glBindVertexArray(m_VertexArrayID));
    std::vector<StandardVertex> vertices;
    vertices.clear();
    std::vector<uint> indices;
    for (int i = 0; i < m_Groups.size(); i++) {
        m_Groups[i].update();
        auto temp = m_Groups[i].getIndices();
        indices.insert(indices.end(), temp.begin(), temp.end());
    }
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferID));
    printf("Allocating %luB of GPU memory!\n", indices.size() * sizeof(uint));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), (void*)indices.data(), GL_STATIC_DRAW));
    for (int i = 0; i < m_Meshes.size(); i++) {
        auto temp = m_Meshes[i].getStandardVertices();
        vertices.insert(vertices.end(), temp.begin(), temp.end());
    }
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
    printf("Allocating %luB of GPU memory!\n", vertices.size() * STANDARD_VERTEX_BYTE_SIZE);
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * STANDARD_VERTEX_BYTE_SIZE, (void*)vertices.data(), GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, STANDARD_VERTEX_BYTE_SIZE, 0));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, false, STANDARD_VERTEX_BYTE_SIZE, (void*)(3 * sizeof(float))));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, false, STANDARD_VERTEX_BYTE_SIZE, (void*)(6 * sizeof(float))));
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

bool Model::LoadOBJ(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoords,
        std::vector<Face> &faces, std::vector<gr> &meshes, std::vector<mat> &usingMaterial, std::vector<std::string> &materialLibs, std::string path) {
    meshes.push_back({"_CO_STANDARD", 0});
    usingMaterial.push_back({"_CO_STANDARD", 0});
    std::string directoryPath;
    std::string tempString = "";
    for (auto i = path.begin(); i != path.end(); i++) {
        tempString.push_back(*i);
        if (*i == '/') {
            directoryPath += tempString;
            tempString = "";
        }
    }
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
                            materialLibs.push_back(directoryPath + readMateriallib(line));
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
            myFile.close();
        } else {
            std::cout << "Failed to load model! Error: " << myFile.rdstate() << std::endl;
            myFile.close();
            return false;
        }
    } else {
        std::cout << "Failed to open file the file " << path << "!" << std::endl;
        return false;
    }
    if (vertices.size() < 1) {
        vertices.push_back({glm::vec3(0)});
    }
    if (normals.size() < 1) {
        normals.push_back({glm::vec3(0)});
    }
    if (texCoords.size() < 1) {
        texCoords.push_back({glm::vec2(0)});
    }

    meshes.push_back({"placeholder", faces.size()}); //Last element in faces
    usingMaterial.push_back({"placeholder", faces.size()}); //Last element in faces
}

bool Model::LoadMTL(const std::vector<std::string> &materialLibs) {

    std::string line;
    std::ifstream myFile;
    m_Groups.push_back(Group(*this));
    m_Groups[0].setName("_CO_STANDARD");
    for (int z = 0; z < materialLibs.size(); z++) {
        uint y = 0;
        line.clear();
        Material tempMat = EMPTY_MATERIAL;
        myFile.open(materialLibs[z]);
        if (myFile.is_open()) {
            for (std::getline(myFile, line); myFile.good(); std::getline(myFile, line)) {
                if (line.find("newmtl") != std::string::npos) {
                    m_Groups[y].setMaterial(tempMat);
                    tempMat = {};
                    y++;
                    m_Groups.push_back(*this);
                    m_Groups[y].setName(readName("newmtl", line));
                    continue;
                } else if (line.find("illum") != std::string::npos) {
                    tempMat.illum = (uint)readFloat("illum", line);
                    continue;
                } else if (line.find("bump") != std::string::npos) {
                    tempMat.textures.push_back({readName("bump", line), "material.bumpMap"});

                } else if (line.find("map") != std::string::npos) {

                    if (line.find("map_Ka") != std::string::npos) {
                        tempMat.textures.push_back({readName("map_Ka", line), "material.ambientTexture"});
                    } else if (line.find("map_Kd") != std::string::npos) {
                        tempMat.textures.push_back({readName("map_Kd", line), "material.diffuseTexture"});
                    } else if (line.find("map_Ks") != std::string::npos) {
                        tempMat.textures.push_back({readName("map_Ks", line), "material.specularTexture"});
                    } else if (line.find("map_Ns") != std::string::npos) {
                        tempMat.textures.push_back({readName("map_Ns", line), "material.specularETexture"});
                    }

                } else {
                    auto i = line.begin();
                    for (;i != line.end();) {if (!(*i == ' ' || *i == '\t')) break; i++;}
                    if (i == line.end()) continue;
                    switch (*i)
                    {
                    
                    case '#':
                        break;

                    case 'K':
                        {
                        std::array<float, 3> floats;
                        switch(*(i + 1)) {
                            case 'a':
                                {
                                floats = read3f("Ka", line);
                                tempMat.ambient = {floats[0], floats[1], floats[2]};;
                                break;
                                }
                            
                            case 'd':
                                {
                                floats = read3f("Kd", line);
                                tempMat.diffuse = {floats[0], floats[1], floats[2]};;
                                break;
                                }

                            case 's':
                                {
                                floats = read3f("Ks", line);
                                tempMat.specular = {floats[0], floats[1], floats[2]};
                                break;
                                }
                            
                            default: 
                                break;
                        }
                        break;
                        }

                    case 'N':

                        if (*(i + 1) == 's') {
                            tempMat.specE = readFloat("Ns", line);
                        }
                        else if (*(i + 1) == 'i') {
                            tempMat.opticalDensity = readFloat("Ni", line);
                        }

                        break;
                    
                    case 'T':
                        if (*(i + 1) == 'r') {
                            tempMat.opacity = 1 - readFloat("Tr", line);
                        }
                        break;

                    case 'd':
                        tempMat.opacity = readFloat("d", line);
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            if (myFile.eof()) {
                std::cout << "Successfully loaded materials!" << std::endl;
                myFile.close();
            } else {
                std::cout << "Failed to read material file " << materialLibs[z] << "! Error: " << myFile.rdstate() << std::endl;
                myFile.close();
                return false;
            }
        } else {
            std::cout << "Failed to open file the file " << materialLibs[z] << "!" << std::endl;
            return false;
        }
    }
    return true;
}