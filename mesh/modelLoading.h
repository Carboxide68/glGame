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

glm::vec3 readVertex(std::string line) { //v
    std::array<std::string, 3> stringVertex;
    glm::vec3 vertex;
    int i = 0;
    for (; i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == 'v') continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
            stringVertex[x].push_back(line[i]);
        }
    }
    vertex[0] = std::stof(stringVertex[0]);
    vertex[1] = std::stof(stringVertex[1]);
    vertex[2] = std::stof(stringVertex[2]);
    return vertex;
}

//Currently only support 2D textures. Explore later
glm::vec2 readTexCoord(std::string line) { //vt
    std::array<std::string, 2> stringTexCoords;
    glm::vec2 texCoords;
    int i = 0;
    for (; i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == 'v' || line[i] == 't') continue;
        break;
    }
    for(int x = 0;x < 2;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
            stringTexCoords[x].push_back(line[i]);
        }
    }
    texCoords[0] = stof(stringTexCoords[0]);
    texCoords[1] = stof(stringTexCoords[1]);
    return texCoords;
}

glm::vec3 readNormal(std::string line) { //vn
    std::array<std::string, 3> stringNormal;
    glm::vec3 normal;
    int i = 0;
    for (; i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r' || line[i] == 'v' || line[i] == 'n') continue;
        break;
    }
    for(int x = 0;x < 3;x++) {
        for (; line[i] == ' '; i++) continue;
        for (;i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
            stringNormal[x].push_back(line[i]);
        }
    }
    normal[0] = std::stof(stringNormal[0]);
    normal[1] = std::stof(stringNormal[1]);
    normal[2] = std::stof(stringNormal[2]);
    return normal;
}

Face readFace(std::string line) { //f
    Face myFace;
    std::vector<std::string> faceString;
    int i = 0;
    for (; i < (line.size() - 1); i++) {
        if (line[i] == ' ' || line[i] == 'f') continue;
        break;
    }
    for(;i < (line.size() - 1);) {
        for (;line[i] == ' '; i++) continue;
        faceString.push_back("");
        for (;i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
            faceString.back().push_back(line[i]);
        }
    }
    myFace.normal.resize(faceString.size());
    myFace.texCoord.resize(faceString.size());
    myFace.vertex.resize(faceString.size());

    std::vector<uint> offsets;
    offsets.resize(faceString.size());

    for (int x = 0; x < faceString.size(); x++) {
        std::string tempS = "0";
        i = 0;
        for (;i < faceString[x].size(); i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.vertex[x] = stoi(tempS);
        tempS = "0";
        i++;
        for (;i < faceString[x].size(); i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.texCoord[x] = stoi(tempS);
        tempS = "0";
        i++;
        for (;i < faceString[x].size(); i++) {
            if (faceString[x][i] == '/') break;
            tempS.push_back(faceString[x][i]);
        }
        myFace.normal[x] = stoi(tempS);
    }
    return myFace;
}

std::string readMeshName(std::string line) { //g
    std::string name;
    int i = 0;
    bool passedg = false;
    for (; i < line.size(); i++) {
        if (line[i] == ' ') continue;
        if (line[i] == 'g' && !passedg) { //Ignore the first g that comes up
            passedg = true;
            continue;
        }
        break;
    }
    for (;i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
        name.push_back(line[i]);
    }
    return name;
}

bool readSmoothingGroup(std::string line) {
    if (line.find("on") != std::string::npos) {
        return true;
    }
    return false;
}

std::string readMaterial(std::string line) {
    std::string name;
    int i = line.find("usemtl") + 6;
    for (; i < line.size(); i++) {
        if (line[i] == ' ') continue;
        break;
    }
    for (;i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
        name.push_back(line[i]);
    }
    return name;
}

std::string readMateriallib(std::string line) {
    std::string name;
    int i = line.find("mtllib") + 6;
    for (; i < line.size(); i++) {
        if (line[i] == ' ') continue;
        break;
    }
    for (;i < line.size(); i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') break;
        name.push_back(line[i]);
    }
    return name;
}