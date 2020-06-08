#pragma once

#include "../common/common.h"

struct Material {

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 specStrength;
    float opacity;
    std::string ambientT;
    std::string diffuseT;
    std::string specularT;

};

class Group {
public:

    Group(Model parent);
    Group(Model parent, std::vector<Polygon> polygons);

    std::vector<GLuint> getIndices();

    inline void addPolygon(Polygon polygon) {m_Polygons.push_back(polygon);};
    inline void addPolygons(const std::vector<Polygon> &polygons) {m_Polygons.insert(m_Polygons.end(), polygons.begin(), polygons.end());}

    void bindMaterial(const uint bindNumber);
    void createTextures();

    uint textureCount() const;

private:

    void EvaluateMaterialProperties();

    void UpdateIndices();

    Model &m_ParentModel;

    std::vector<Polygon&> m_Polygons;
    std::vector<uint> m_Indices(); //Internally stored indices for omptimized returning

    Material m_Material;

    std::array<uint, 8> materialProperties;

    std::array<uint, 3> m_Textures;
};