#include "group.h"
#include "model.h"


Group::Group(Model &parent) :  m_ParentModel(parent) {
    material = EMPTY_MATERIAL;
}


Group::Group(Model &parent, std::vector<Polygon> polygons) : m_ParentModel(parent) {
    m_Polygons.reserve(polygons.size());
    for (int i = 0; i < (int)polygons.size(); i++) {
        m_Polygons[i] = &polygons[i];
    }

    material = EMPTY_MATERIAL;

}

void Group::addPolygons(std::vector<Polygon> &polygons) {
    m_Polygons.reserve(polygons.size());
    for (int i = 0; i < (int)polygons.size(); i++) {
        m_Polygons.push_back(&polygons[i]);
    }
}

void Group::addPolygons(std::vector<Polygon*> &polygons) {
    m_Polygons.insert(m_Polygons.end(), polygons.begin(), polygons.end());
}

void Group::bindMaterial(Shader shader) {
    
    shader.setUniform("material.ambient", material.ambient);
    shader.setUniform("material.diffuse", material.diffuse);
    shader.setUniform("material.specular", material.specular);
    shader.setUniform("material.illum", material.illum);
    shader.setUniform("material.specE", material.specE);
    shader.setUniform("material.opacity", material.opacity);
    shader.setUniform("material.opticalDensity", material.opticalDensity);
    //Implement binding of textures
}

void Group::UpdateIndices() {

    size_t polygonLoc;
    m_Indices.clear();

    for (uint i = 0; i < m_Polygons.size(); i++) {
        polygonLoc = m_ParentModel.getPolygonLocation(m_Polygons[i]->ID);
        std::vector<uint> tempIndices = m_Polygons[i]->assembleIndices();
        for (auto index = tempIndices.begin(); index != tempIndices.end(); index++) {
            m_Indices.push_back(polygonLoc + *(index));
        }
    }
}

void Group::update() {
    UpdateIndices();
}