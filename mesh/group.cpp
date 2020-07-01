#include "group.h"
#include "model.h"


Group::Group(Model &parent) :  m_ParentModel(parent) {
    Material = EMPTY_MATERIAL;
}


Group::Group(Model &parent, std::vector<Polygon> polygons) : m_ParentModel(parent) {
    m_Polygons.reserve(polygons.size());
    for (int i = 0; i < polygons.size(); i++) {
        m_Polygons[i] = &polygons[i];
    }

    Material = EMPTY_MATERIAL;

}

void Group::addPolygons(std::vector<Polygon> &polygons) {
    m_Polygons.reserve(polygons.size());
    for (int i = 0; i < polygons.size(); i++) {
        m_Polygons.push_back(&polygons[i]);
    }
}

void Group::addPolygons(std::vector<Polygon*> &polygons) {
    m_Polygons.insert(m_Polygons.end(), polygons.begin(), polygons.end());
}

void Group::bindMaterial(Shader shader) {
    
    shader.setUniform("material.ambient", Material.ambient);
    shader.setUniform("material.diffuse", Material.diffuse);
    shader.setUniform("material.specular", Material.specular);
    shader.setUniform("material.illum", Material.illum);
    shader.setUniform("material.specE", Material.specE);
    shader.setUniform("material.opacity", Material.opacity);
    shader.setUniform("material.opticalDensity", Material.opticalDensity);
    //Implement binding of textures
}

void Group::generateTextures() {
    
}

uint Group::textureCount() const {
    
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