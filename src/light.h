#pragma once

#include <glm/glm.hpp>
#include "color.h"

struct Light {
  glm::vec3 position;
  float intensity;
  Color color;
};