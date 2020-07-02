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

std::string readName(const std::string before, const std::string line) {
    const size_t lineSize = line.size();
    std::string name;
    int i = line.find(before) + before.size();
    for (; i < (int)lineSize; i++) {
        if (line[i] == ' ') continue;
        break;
    }
    for (;i < (int)lineSize; i++) {
        if (ifSeperator[(int)line[i]]) break;
        name.push_back(line[i]);
    }
    return name;
}

float readFloat(const std::string before, const std::string line) {
    const size_t lineSize = line.size();
    int i = (int)(line.find(before) + before.size());
    std::string stringFloat;
    for (; line[i] == ' '; i++) continue;
    for (;i < (int)lineSize; i++) {
        if (ifSeperator[(int)line[i]]) break;
        stringFloat.push_back(line[i]);
    }
    return stof(stringFloat);
}

std::array<float, 3> read3f(const std::string before, const std::string line) {
    const size_t lineSize = line.size();
    std::array<float, 3> floats;
    std::array<std::string, 3> stringVertex;
    int i = (int)(line.find(before) + before.size());
    for (; line[i] == ' '; i++) continue;
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < (int)lineSize; i++) {
            if (ifSeperator[(int)line[i]]) break;
            stringVertex[x].push_back(line[i]);
        }
    }
    floats[0] = std::stof(stringVertex[0]);
    floats[1] = std::stof(stringVertex[1]);
    floats[2] = std::stof(stringVertex[2]);
    return floats;
}

glm::vec3 readVertex(const std::string line) { //v
    const size_t lineSize = line.size();
    std::array<std::string, 3> stringVertex;
    glm::vec3 vertex;
    int i = 0;
    for (; i < (int)lineSize; i++) {
        if (vertexFilter[(int)line[i]]) continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < (int)lineSize; i++) {
            if (ifSeperator[(int)line[i]]) break;
            stringVertex[x].push_back(line[i]);
        }
    }
    vertex[0] = std::stof(stringVertex[0]);
    vertex[1] = std::stof(stringVertex[1]);
    vertex[2] = std::stof(stringVertex[2]);
    return vertex;
}

//Currently only support 2D textures. Expr lore later
glm::vec2 readTexCoord(const std::string line) { //vt
    const size_t lineSize = line.size();
    std::array<std::string, 2> stringTexCoords;
    glm::vec2 texCoords;
    int i = 0;
    for (; i < (int)lineSize; i++) {
        if (texFilter[(int)line[i]]) continue;
        break;
    }
    for(int x = 0;x < 2;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < (int)lineSize; i++) {
            if (ifSeperator[(int)line[i]]) break;
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
    for (; i < (int)lineSize; i++) {
        if (ifSeperator[(int)line[i]] || normalFilter[(int)line[i]]) continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < (int)lineSize; i++) {
            if (ifSeperator[(int)line[i]]) break;
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
    for (; i < (int)lineSize && faceFilter[(int)line[i]]; i++);
    int x = 0;
    for(;i < (int)lineSize;) {
        faceString.push_back("");
        for (;i < (int)lineSize; i++) {
            if (ifSeperator[(int)line[i]]) break;
            faceString[x].push_back(line[i]);
        }
        x++;
        for (;line[i] == ' '; i++) continue;
        if (ifSeperator[(int)line[i]]) break;
    }   
    size_t faceSize = faceString.size();
    myFace.normal.resize(faceSize);
    myFace.texCoord.resize(faceSize);
    myFace.vertex.resize(faceSize);

    for (int x = 0; x < (int)faceSize; x++) {
        std::string tempS = "0";
        size_t faceSize = faceString[x].size();
        i = 0;
        for (;i < (int)faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.vertex[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
        tempS = "0";
        i++;
        for (;i < (int)faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.texCoord[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
        tempS = "0";
        i++;
        for (;i < (int)faceSize; i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.normal[x] = (tempS == "0") ? 0 : stoi(tempS) - 1;
    }
    return myFace;
}

std::string readMeshName(const std::string line) { //g
    return readName("g", line);
}

bool readSmoothingGroup(const std::string line) {
    if (line.find("on") != std::string::npos) {
        return true;
    }
    return false;
}

std::string readMaterial(const std::string line) {
    return readName("usemtl", line);
}

std::string readMateriallib(const std::string line) {
    return readName("mtllib", line);
}