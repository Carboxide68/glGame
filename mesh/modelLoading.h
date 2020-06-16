#pragma once

#include "../common/common.h"

//This file is used for utility functions when loading a model
//A function is passed a string "line" that is composed of a single line, that is already confirmed to be
//fitting to that specific function.
//The comment after these functions are the keywoards that are supposed to be sent to that function. This is just for
//remembering.

struct Face { //Indices
    std::vector<uint> vertex;
    std::vector<uint> texCoord;
    std::vector<uint> normal;
};

constexpr bool faceFilter[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //NULL
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //DLE
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //SPACE
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //@
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //P
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, //`, a
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  //p
};

const bool normalFilter[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //NULL
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //DLE
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //SPACE
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //@
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //P
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, //`, a
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0  //p
};
const bool vertexFilter[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //NULL
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //DLE
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //SPACE
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //@
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //P
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //`, a
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0  //p
};
const bool texFilter[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //NULL
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //DLE
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //SPACE
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //@
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //P
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //`, a
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0  //p
};
const bool ifSeperator[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, //NULL
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //DLE
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //SPACE
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //@
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //P
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //`, a
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  //p
};

glm::vec3 readVertex(const std::string line) { //v
    const size_t lineSize = line.size();
    std::array<std::string, 3> stringVertex;
    glm::vec3 vertex;
    int i = 0;
    for (; i < lineSize; i++) {
        if (vertexFilter[line[i]]) continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < lineSize; i++) {
            if (ifSeperator[line[i]]) break;
            stringVertex[x].push_back(line[i]);
        }
    }
    vertex[0] = std::stof(stringVertex[0]);
    vertex[1] = std::stof(stringVertex[1]);
    vertex[2] = std::stof(stringVertex[2]);
    return vertex;
}

//Currently only support 2D textures. Explore later
glm::vec2 readTexCoord(const std::string line) { //vt
    const size_t lineSize = line.size();
    std::array<std::string, 2> stringTexCoords;
    glm::vec2 texCoords;
    int i = 0;
    for (; i < lineSize; i++) {
        if (texFilter[line[i]]) continue;
        break;
    }
    for(int x = 0;x < 2;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < lineSize; i++) {
            if (ifSeperator[line[i]]) break;
            stringTexCoords[x].push_back(line[i]);
        }
    }
    texCoords[0] = stof(stringTexCoords[0]);
    texCoords[1] = stof(stringTexCoords[1]);
    return texCoords;
}

glm::vec3 readNormal(const std::string line) { //vn
    const size_t lineSize = line.size();
    std::array<std::string, 3> stringNormal;
    glm::vec3 normal;
    int i = 0;
    for (; i < lineSize; i++) {
        if (ifSeperator[line[i]] || faceFilter[line[i]]) continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < lineSize; i++) {
            if (ifSeperator[line[i]]) break;
            stringNormal[x].push_back(line[i]);
        }
    }
    normal[0] = std::stof(stringNormal[0]);
    normal[1] = std::stof(stringNormal[1]);
    normal[2] = std::stof(stringNormal[2]);
    return normal;
}

Face readFace(const std::string line) { //f
    const size_t lineSize = line.size();
    Face myFace;
    std::vector<std::string> faceString;
    int i = 0;
    for (; i < lineSize && faceFilter[line[i]]; i++);
    int x = 0;
    for(;i < lineSize;) {
        faceString.push_back("");
        for (;i < lineSize; i++) {
            if (ifSeperator[line[i]]) break;
            faceString[x].push_back(line[i]);
        }
        x++;
        for (;ifSeperator[line[i]]; i++) continue;
    }   
    size_t faceSize = faceString.size();
    myFace.normal.resize(faceSize);
    myFace.texCoord.resize(faceSize);
    myFace.vertex.resize(faceSize);

    for (int x = 0; x < faceSize; x++) {
        std::string tempS = "0";
        size_t faceSize = faceString[x].size();
        i = 0;
        for (;i < faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.vertex[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
        tempS = "0";
        i++;
        for (;i < faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.texCoord[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
        tempS = "0";
        i++;
        for (;i < faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.normal[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
    }
    return myFace;
}

std::string readMeshName(const std::string line) { //g
    const size_t lineSize = line.size();
    std::string name;
    int i = 0;
    bool passedg = false;
    for (; i < lineSize; i++) {
        char l = line[i];
        if (l == ' ') continue;
        if (l == 'g' && !passedg) { //Ignore the first g that comes up
            passedg = true;
            continue;
        }
        break;
    }
    for (;i < lineSize; i++) {
        char l = line[i];
        if (ifSeperator[l]) break;
        name.push_back(l);
    }
    return name;
}

bool readSmoothingGroup(const std::string line) {
    if (line.find("on") != std::string::npos) {
        return true;
    }
    return false;
}

std::string readMaterial(const std::string line) {
    const size_t lineSize = line.size();
    std::string name;
    int i = line.find("usemtl") + 6;
    for (; i < lineSize; i++) {
        if (line[i] == ' ') continue;
        break;
    }
    for (;i < lineSize; i++) {
        if (ifSeperator[line[i]]) break;
        name.push_back(line[i]);
    }
    return name;
}

std::string readMateriallib(const std::string line) {
    const size_t lineSize = line.size();
    std::string name;
    int i = line.find("mtllib") + 6;
    for (; i < lineSize; i++) {
        if (line[i] == ' ') continue;
        break;
    }
    for (;i < lineSize; i++) {
        if (ifSeperator[line[i]]) break;
        name.push_back(line[i]);
    }
    return name;
}