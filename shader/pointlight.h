#pragma once

#include "../common/common.h"
#include "light.h"
#include "../camera/camera.h"
#include "../mesh/mesh.h"

class PointLight {

public:

    PointLight();
    PointLight(Light lightProfile);

    inline void changePosition(const glm::vec3 &magnitude) {m_LightInfo.position += magnitude;};
    inline void setPosition(const glm::vec3 &position) {m_LightInfo.position = position;};
    inline glm::vec3 getPosition() const {return m_LightInfo.position;};

    inline glm::vec3 *getPositionReference() {return &(m_LightInfo.position);}

    void updateShadowmap(Mesh myMesh);

    void changeResolution(const uint shadow_width, const uint shadow_height);

    void lightUniform(Shader *shader);

    float near;
    float far;

private:

    void m_GenerateCubemap();
    void m_GenerateFBO();

    void m_PrepareShadowmap();

    uint m_SHADOW_WIDTH;
    uint m_SHADOW_HEIGHT;

    uint m_DepthCubemap;
    uint m_DepthMapFBO;

    Light m_LightInfo;

    std::vector<glm::mat4> m_ShadowTransforms;
    
    Camera m_LightCam;
    Shader m_PointShadowShader = Shader("shader/pointShadow.vert",
                                        "shader/pointShadow.geom",
                                        "shader/pointShadow.frag");
};