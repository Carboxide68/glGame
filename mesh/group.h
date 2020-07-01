#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "polygon.h"
#include "../shader/shader.h"

class Model;

class Group {
public:

    Group(Model &parent);
    Group(Model &parent, std::vector<Polygon> polygons);

    inline const std::vector<uint>& getIndices() const {return m_Indices;}

    inline void addPolygon(Polygon polygon) {m_Polygons.push_back(&polygon);};
    inline void addPolygon(Polygon* polygon) {m_Polygons.push_back(polygon);};
    void addPolygons(std::vector<Polygon> &polygons);
    void addPolygons(std::vector<Polygon*> &polygons);

    void bindMaterial(Shader shader);
    void generateTextures();

    uint textureCount() const;
    inline uint indexCount() const {return m_Indices.size();}

    void update();

    Material Material;
    std::string Name;

private:


    void UpdateIndices();

    Model &m_ParentModel;

    std::vector<Polygon*> m_Polygons;
    std::vector<uint> m_Indices; //Internally stored indices for omptimized assembling

    std::array<uint, 8> m_MaterialProperties;
    std::array<uint, 3> m_Textures;
};