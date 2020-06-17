#include "group.h"
#include "model.h"


Group::Group(Model &parent) :  m_ParentModel(parent) {}


Group::Group(Model &parent, std::vector<Polygon> polygons) : m_ParentModel(parent) {
    m_Polygons.reserve(polygons.size());
    for (int i = 0; i < polygons.size(); i++) {
        m_Polygons[i] = &polygons[i];
    }
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

void Group::bindMaterial() {
    
}

void Group::generateTextures() {
    
}

uint Group::textureCount() const {
    
}

void Group::EvaluateMaterialProperties() {

    memset(m_MaterialProperties.data(), (uint) 1, m_MaterialProperties.size() * sizeof(uint));

    if (m_Material.diffuse[0] > 1.0f) { //Colors will never be greater than 1;
        m_MaterialProperties[0] = 0;
    }
    if (m_Material.diffuse[0] > 1.0f) { //Colors will never be greater than 1;
        m_MaterialProperties[1] = 0;
    }
    if (m_Material.diffuse[0] > 1.0f) { //Colors will never be greater than 1;
        m_MaterialProperties[2] = 0;
    }
    if (m_Material.specStrength < 0.0f) {
        m_MaterialProperties[3] = 0;
    }
    if (m_Material.opacity < 0.0f) {
        m_MaterialProperties[4] = 0;
    }
    if (m_Material.ambientT == "") {
        m_MaterialProperties[5] = 0;
    }
    if (m_Material.diffuseT == "") {
        m_MaterialProperties[6] = 0;
    }
    if (m_Material.specularT == "") {
        m_MaterialProperties[7] = 0;
    }
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