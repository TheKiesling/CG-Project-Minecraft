#pragma once

#include <glm/glm.hpp>
#include "material.h"
#include "intersect.h"

class Object {
public:
  Object(const Material& mat) : material(mat) {}
  virtual Intersect rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const = 0;
  
  Material material;
};