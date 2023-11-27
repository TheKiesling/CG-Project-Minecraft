#pragma once
#include <glm/glm.hpp>

class Camera {
public:
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 up;

  float rotationSpeed;

  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float rotationSpeed);

  void rotate(float deltaX, float deltaY);

  void zoom(float deltaZ);

  void moveX(float deltaX);

  void moveY(float deltaY);

  void moveZ(float deltaZ);
};