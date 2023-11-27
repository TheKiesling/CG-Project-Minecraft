#pragma once 
#include "../cube.h"
#include <cmath>

class Diamond : public Cube
{
public:
  Diamond(const glm::vec3 &minBound, const glm::vec3 &maxBound, const Material &mat)
      : Cube(minBound, maxBound, mat) {}

  Intersect rayIntersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const override
  {
    Intersect intersect = Cube::rayIntersect(rayOrigin, rayDirection);

    const float epsilon = 0.0001;
    if (glm::abs(intersect.point.y - maxBound.y) < epsilon)
    {
      Color c = loadTexture(std::abs(intersect.point.x - minBound.x), std::abs(intersect.point.z - minBound.z), "diamond");

      intersect.color = c;
      intersect.hasColor = true;
    }
    else if (glm::abs(intersect.point.z - maxBound.z) < epsilon || glm::abs(intersect.point.z - minBound.z) < epsilon){
      Color c = loadTexture(std::abs(intersect.point.x - minBound.x), std::abs(intersect.point.y - minBound.y), "diamond");

      intersect.color = c;
      intersect.hasColor = true;
    }
    else if (glm::abs(intersect.point.x - minBound.x) < epsilon || glm::abs(intersect.point.x - maxBound.x) < epsilon){
      Color c = loadTexture(std::abs(intersect.point.z - minBound.z), std::abs(intersect.point.y - minBound.y), "diamond");

      intersect.color = c;
      intersect.hasColor = true;
    }

    return intersect;
  };
};