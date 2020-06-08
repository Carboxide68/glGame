#pragma once

#include "../common/common.h"
#include "modelCommon.h"

class NextGenShader{};

class Model {
public:

    Model();

    void loadModel(std::string path);

    void draw();

    size_t getPolygonBufferLocation(ModelID id);

    void optimizieMeshes(); //I have no fucking idea how to do this so low priority

private:

    uint GetMeshIndexFromID(ModelID id);

    ModelID GenerateID();

    std::vector<Mesh> m_Meshes; //The meshes used in the model
    std::vector<Group> m_Groups;//The groups used in the model

    std::vector<uint> m_MeshMap; //Index N is index (N - 1) + MeshTriangleCount

    ModelID lastID;

    NextGenShader m_Shader;//Change this when Shader class is updated

};