#pragma once

#include "../common/common.h"
#include "shader.h"

class ShadowMapper {

public:

    ShadowMapper(unsigned int shadowWidth, unsigned int shadowHeight) : SHADOW_WIDTH(shadowWidth), SHADOW_HEIGHT(shadowHeight);
    ~ShadowMapper();

private:

    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;

};