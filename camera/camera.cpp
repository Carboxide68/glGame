#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 lookingDir) {

    m_Position = position;
    m_LookingDir = glm::normalize(lookingDir);
    FOV = 90.0f;

	m_PerspectiveMatrix = glm::perspective(FOV, (float)5/(float)3, 0.1f, 100.0f);
}

void Camera::lookAt(glm::vec3 position) {

    m_LookingDir = glm::normalize(position - m_Position);

}

glm::mat4 Camera::getViewMatrix() const {
    
    glm::vec3 sideDir;
    glm::vec3 upDir;

    if (abs(m_LookingDir.y) > 0.99f) {
        printf("Looking too much up\n");
    }

    sideDir = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_LookingDir));

    upDir = glm::normalize(glm::cross(m_LookingDir, sideDir));

    return glm::mat4 (

        sideDir.x, upDir.x, -m_LookingDir.x, 0.0f,
        sideDir.y, upDir.y, -m_LookingDir.y, 0.0f,
        sideDir.z, upDir.z, -m_LookingDir.z, 0.0f,
        -glm::dot(sideDir, m_Position), -glm::dot(upDir, m_Position), -glm::dot(m_LookingDir, m_Position), 1.0f

    );

}

glm::mat4 Camera::getPerspectiveMatrix() const {
    return m_PerspectiveMatrix;
}

glm::vec3 Camera::getPosition() const {
    return m_Position;
}

void Camera::setPosition(glm::vec3 position) {
    m_Position = position;
}

void Camera::move(glm::vec3 mag) {
    m_Position += mag;
}