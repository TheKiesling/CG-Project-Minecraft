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

  void move(float deltaZ);
};
