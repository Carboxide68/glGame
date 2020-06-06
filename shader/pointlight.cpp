#include "pointlight.h"

PointLight::PointLight() {

    m_SHADOW_HEIGHT = 1024;
    m_SHADOW_WIDTH = 1024;

    near = 0.1f;
    far = 100.0f;

    GLCall(glGenTextures(1, &m_DepthCubemap));
    GLCall(glGenFramebuffers(1, &m_DepthMapFBO));

}

PointLight::PointLight(Light lightProfile) {

    m_SHADOW_HEIGHT = 1024;
    m_SHADOW_WIDTH = 1024;

    near = 0.1f;
    far = 100.0f;

    m_LightInfo = lightProfile;

    GLCall(glGenTextures(1, &m_DepthCubemap));
    GLCall(glGenFramebuffers(1, &m_DepthMapFBO));

}

void PointLight::m_GenerateCubemap() {

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemap));
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     m_SHADOW_WIDTH, m_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void PointLight::m_GenerateFBO() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO));
    GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthCubemap, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PointLight::setResolution(const uint shadow_width, const uint shadow_height) {
    m_SHADOW_HEIGHT = shadow_height;
    m_SHADOW_WIDTH = shadow_width;
}

void PointLight::updateShadowmap(Mesh myMesh) { //Implement Scene object

    m_PrepareShadowmap();

    int formerHeight, formerWidth;
    {
        GLint viewPortArgs[4];
        glGetIntegerv(GL_VIEWPORT, viewPortArgs);
        formerWidth = viewPortArgs[2];
        formerHeight = viewPortArgs[3];
    }
    GLCall(glViewport(0, 0, m_SHADOW_WIDTH, m_SHADOW_HEIGHT));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO));
    glClear(GL_DEPTH_BUFFER_BIT);

    myMesh.setShader(m_PointShadowShader);
    m_PointShadowShader.use();
    m_PointShadowShader.setUniform("far_plane", far);
    m_PointShadowShader.setUniform("lightPos", m_LightInfo.position);
    for (int i = 0; i < 6; i++) {
        m_PointShadowShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
    }
    myMesh.draw(m_LightCam);

    myMesh.resetShader();
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glViewport(0, 0, formerWidth, formerHeight));
}

void PointLight::m_PrepareShadowmap() {

    m_GenerateCubemap();
    m_GenerateFBO();

    m_LightCam.updatePerspectiveMatrix(glm::radians(90.0f), (float)m_SHADOW_WIDTH/(float)m_SHADOW_HEIGHT, near, far);

    glm::mat4 shadowProj = m_LightCam.getPerspectiveMatrix();

    m_ShadowTransforms.clear();

    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    m_ShadowTransforms.push_back(shadowProj * 
                 glm::lookAt(m_LightInfo.position, m_LightInfo.position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

}

void PointLight::lightUniform(Shader *shader) {
    shader->use();
    shader->setUniform("light.luminosity", m_LightInfo.luminosity);
    shader->setUniform("light.color", m_LightInfo.color);
    shader->setUniform("light.position", m_LightInfo.position);
    shader->setUniform("far_plane", far);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemap);
}