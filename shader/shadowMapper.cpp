#include "shadowMapper.h"

ShadowMapper::ShadowMapper(unsigned int shadowWidth, unsigned int shadowHeight) 
                        : SHADOW_WIDTH(shadowWidth), SHADOW_HEIGHT(shadowHeight) {

    GLCall(glGenTextures(1, &depthMap));
    GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


}

ShadowMapper::~ShadowMapper() {

}