#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "../glObjects/vao.h"
#include "../shader/shader.h"
#include "mesh.h"
#include "group.h"

class NextGenShader{};

struct Face;

class Model {
public:

    Model();
    ~Model();

    void draw(Shader shader);

    bool loadModel(std::string path);

    inline size_t getPolygonLocation(const ModelID id) const {
        uint meshId = GetMeshIndexFromID(id);
        return m_MeshMap[meshId] + Meshes[meshId].getPolygonLocation(id);
    }

    inline size_t getMeshLocation(const ModelID id) const {return m_MeshMap[GetMeshIndexFromID(id)];}

    void optimizieMeshes(); //I have no idea how to do this so low priority

    void update();

    void loadToBuffer();
    void unLoad();

    std::vector<Mesh> Meshes; //The meshes used in the model
    std::vector<Group> Groups;//The groups used in the model

private:

    struct gr {
        std::string name; 
        uint loc;
    };

    struct mat {
        std::string name; 
        uint loc;
    };

    //Function is used to update the internal buffermap of the model object. This should be done whenever you change something
    //in the object and want to load this into the GPU buffer;  If the GPU buffer and the MeshMap are not synched, this may 
    //lead to problems.
    void UpdateMeshMap();

    inline uint GetMeshIndexFromID(ModelID id) const {return id.mesh;};

    inline ModelID GenerateID() {m_LastID.mesh += 1; return m_LastID;}

    bool LoadOBJ(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoords, std::vector<Face> &faces, 
        std::vector<gr> &meshes, std::vector<mat> &usingMaterial, std::vector<std::string> &materialLibs, std::string path);

    bool LoadMTL(const std::vector<std::string> &materialLibs);

    uint m_VertexArrayID;
    uint m_VertexBufferID;
    uint m_ElementBufferID;

    std::vector<uint> m_MeshMap; //Index N is index (N - 1) + MeshTriangleCount

    ModelID m_LastID;

    NextGenShader m_Shader;//Change this when Shader class is updated
};