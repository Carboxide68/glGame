#pragma once

#include "../common/common.h"

class Camera {

public:

    Camera(glm::vec3 position, glm::vec3 lookingDir);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getPerspectiveMatrix() const;

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 position);
    void move(glm::vec3 mag);

    void setFOV();

    void lookAt(glm::vec3 position);


private:

    float FOV;

    glm::vec3 m_Position;
    glm::vec3 m_LookingDir;

    glm::mat4 m_PerspectiveMatrix;

};