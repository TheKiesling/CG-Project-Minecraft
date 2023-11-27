#include "camera.h"
#include <glm/gtc/quaternion.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float rotationSpeed) 
  : position(position), target(target), up(up), rotationSpeed(rotationSpeed)
{}

void Camera::rotate(float deltaX, float deltaY) {
  glm::quat quatRotY = glm::angleAxis(glm::radians(deltaX * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat quatRotX = glm::angleAxis(glm::radians(deltaY * rotationSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

  position = target + quatRotY * (position - target);
  position = target + quatRotY * (position - target);
}

void Camera::zoom(float deltaZ) {
  glm::vec3 dir = glm::normalize(target - position);
  position += dir * deltaZ;
}

void Camera::moveX(float deltaX) {
    position.x +=  deltaX;
    target.x += deltaX;
}

void Camera::moveY(float deltaY) {
    position.y += deltaY;
    target.y += deltaY;
}

void Camera::moveZ(float deltaZ) {
    position.z += deltaZ;
    target.z += deltaZ;
}