#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "polygon.h"

class Model;

class Mesh {
public:

    Mesh(ModelID ID, std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords);
    Mesh(ModelID ID, std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords);

    std::vector<Triangle> assembleToTriangleMesh();
    std::vector<StandardVertex> getStandardVertices() const;

    //Bytelocation of polygon in m_Polygons;
    inline size_t getPolygonLocation(const ModelID id) const {return m_PolygonMap[GetPolygonIndexFromID(id)];}

    inline std::vector<Polygon*> getPolygons(uint offset, uint end) {
        std::vector<Polygon*> pointerVector;
        for (auto i = m_Polygons.begin() + offset; i != m_Polygons.begin() + end; i++) {
            pointerVector.push_back(&*i);
        }
        return pointerVector;
    }

    Polygon* createPolygon(std::vector<uint> positionIndex, std::vector<uint> texIndex);
    std::vector<Polygon*> createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices = std::vector<std::vector<uint>>(), std::vector<std::vector<glm::vec3>> normals = std::vector<std::vector<glm::vec3>>());
    std::vector<Polygon*> createPolygons(std::vector<std::vector<uint>> positionIndices, std::vector<std::vector<uint>> texIndices, std::vector<glm::vec3> normals, std::vector<std::vector<uint>> normalIndices);

    inline const std::vector<uint>& getPolygonMap() const {return m_PolygonMap;}

    void update(bool force = false);

    std::string Name;

    friend class Model;

    std::vector<glm::vec3> vertices; //Vertex data is stored on a per-mesh basis
    std::vector<glm::vec2> texCoords;

private:

    //Function is used to update the internal buffermap of the mesh object. This should be done whenever you change something
    //in the object and want to load the changes into the GPU buffer;  If the GPU buffer and the MeshMap are not synched, this may 
    //lead to problems.
    void UpdatePolygonMap();
    void UpdatePolygonNormals(bool force);

    inline uint GetPolygonIndexFromID(ModelID id) const {return id.polygon;}

    inline ModelID GenerateID() {return m_LastID; m_LastID.polygon += 1;}

    std::vector<Polygon> m_Polygons;
    std::vector<uint> m_PolygonMap; //Index N is index (N - 1) + PolygonTriangleCount


    ModelID m_LastID;
};