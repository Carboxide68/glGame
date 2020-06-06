#pragma once

#include "../common/common.h"
#include "light.h"
#include "../camera/camera.h"
#include "../mesh/mesh.h"

class PointLight {

public:

    PointLight();
    PointLight(const Light lightProfile);

    inline void movePosition(const glm::vec3 &magnitude) {m_LightInfo.position += magnitude;};
    inline void setPosition(const glm::vec3 &position) {m_LightInfo.position = position;};
    inline glm::vec3 getPosition() const {return m_LightInfo.position;};

    inline glm::vec3 *getPositionReference() {return &(m_LightInfo.position);}

    void updateShadowmap(const Mesh myMesh);

    void setResolution(const uint shadow_width, const uint shadow_height);

    void lightUniform(Shader *shader);

    inline void setColor(const glm::vec3 color) {m_LightInfo.color = color;};
    inline void setLuminosity(const float luminosity) {m_LightInfo.luminosity = luminosity;};

    inline glm::vec3 getColor() const {return m_LightInfo.color;};
    inline float getLuminosity() const {return m_LightInfo.luminosity;};

    inline void setLightProfile(const Light lightProfile) {m_LightInfo = lightProfile;};
    inline Light getLightProfile() const {return m_LightInfo;};

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