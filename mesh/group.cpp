#include "group.h"
#include "model.h"


Group::Group(Model &parent) : m_ParentModel(parent) {
    material = EMPTY_MATERIAL;
    char temp[32];
    sprintf(temp, "Group %d", (int)parent.Groups.size());
    m_Name = temp;
}

void Group::setName(std::string name, bool update) {
    if (update) {
        for (int i = 0; i < (int)m_ParentModel.Meshes.size(); i++) {
            std::vector<std::pair<std::string, uint>> groupMap = m_ParentModel.Meshes[i].getGroupMap();
            for (int j = 1; j < (int)groupMap.size() - 1; j++) {
                if (groupMap[j].first == m_Name) {
                    groupMap[j].first = name;
                }
            }
        }
    }
    m_Name = name;
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

    for (int i = 0; i < (int)m_ParentModel.Meshes.size(); i++) {
        std::vector<std::pair<std::string, uint>> groupMap = m_ParentModel.Meshes[i].getGroupMap();
        for (int j = 0; j < (int)groupMap.size() - 1; j++) {
            if (groupMap[j].first == m_Name) {
                uint begin = groupMap[j].second;
                uint end = groupMap[j + 1].second;
                for (int k = begin; k < end; k++) {
                    std::vector<uint> tempIndices = m_ParentModel.Meshes[i].Polygons[k].assembleIndices();
                    polygonLoc = m_ParentModel.Meshes[i].getPolygonLocation(m_ParentModel.Meshes[i].Polygons[k].ID);
                    for (auto index = tempIndices.begin(); index != tempIndices.end(); index++) {
                        m_Indices.push_back(polygonLoc + *(index));
                    }
                }
            }
        }
    }
    return;
}

void Group::update() {
    UpdateIndices();
}