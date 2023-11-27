#pragma once

#include <glm/glm.hpp>
#include "color.h"

struct Intersect {
  bool isIntersecting = false;
  float dist = 0.0f;
  glm::vec3 point;
  glm::vec3 normal;
  bool hasColor;
  Color color;
};