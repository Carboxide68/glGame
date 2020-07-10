#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "polygon.h"
#include "../shader/shader.h"

class Model;

class Group {
public:

    Group(Model *parent);

    void setName(std::string name, bool update = true);
    inline const std::string& getName() const {return m_Name;};

    inline const std::vector<uint>& getIndices() const {return m_Indices;}

    void bindMaterial(Shader *shader);
    void generateTextures();

    uint textureCount() const;
    inline uint indexCount() const {return m_Indices.size();}

    void update();

    Material material;
    
    friend class Model;

private:

    std::string m_Name;

    void UpdateIndices();

    Model *m_ParentModel;

    std::vector<uint> m_Indices; //Internally stored indices for omptimized assembling

    std::array<uint, 8> m_MaterialProperties;
    std::array<uint, 3> m_Textures;
};