#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "polygon.h"

class Model;

class Group {
public:

    Group(Model &parent);
    Group(Model &parent, std::vector<Polygon> polygons);

    inline void setName(const std::string name) {m_MaterialName = name;}
    inline std::string getName() const {return m_MaterialName;}

    std::vector<uint> getIndices();

    inline void addPolygon(Polygon polygon) {m_Polygons.push_back(&polygon);};
    inline void addPolygon(Polygon* polygon) {m_Polygons.push_back(polygon);};
    void addPolygons(std::vector<Polygon> &polygons);
    void addPolygons(std::vector<Polygon*> &polygons);

    void bindMaterial();
    void generateTextures();

    uint textureCount() const;
    inline uint indexCount() const {return m_Indices.size();};

private:

    void EvaluateMaterialProperties();

    void UpdateIndices();

    Model &m_ParentModel;

    std::vector<Polygon*> m_Polygons;
    std::vector<uint> m_Indices; //Internally stored indices for omptimized assembling

    std::string m_MaterialName;
    Material m_Material;
    std::array<uint, 8> m_MaterialProperties;
    std::array<uint, 3> m_Textures;
};