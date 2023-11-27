#pragma once
#include <glm/glm.hpp>
#include "intersect.h"
#include "object.h"
#include <string>
#include "imageloader.h"


class Cube : public Object {
public:
  Cube(const glm::vec3& minBound, const glm::vec3& maxBound, const Material& mat)
    : minBound(glm::min(minBound, maxBound)), maxBound(glm::max(minBound, maxBound)), Object(mat) {}

  Intersect rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const override {

    glm::vec3 invRayDir = 1.0f / rayDirection;

    glm::vec3 t1 = (minBound - rayOrigin) * invRayDir;
    glm::vec3 t2 = (maxBound - rayOrigin) * invRayDir;

    glm::vec3 tmin = glm::min(t1, t2);
    glm::vec3 tmax = glm::max(t1, t2);

    float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
    float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

    if (tNear > tFar || tFar < 0) {
      return Intersect{false};
    }

    float dist = (tNear < 0) ? tFar : tNear;

    glm::vec3 point = rayOrigin + dist * rayDirection;

    const float epsilon = 0.0001;
    glm::vec3 normal(0.0f);
    if (glm::abs(point.x - minBound.x) < epsilon) normal.x = -1.0f;
    else if (glm::abs(point.x - maxBound.x) < epsilon) normal.x = 1.0f;
    else if (glm::abs(point.y - minBound.y) < epsilon) normal.y = -1.0f;
    else if (glm::abs(point.y - maxBound.y) < epsilon) normal.y = 1.0f;
    else if (glm::abs(point.z - minBound.z) < epsilon) normal.z = -1.0f;
    else if (glm::abs(point.z - maxBound.z) < epsilon) normal.z = 1.0f;

    return Intersect{true, dist, point, glm::normalize(normal), false};
  };

  Color loadTexture(float x, float y, const std::string texturekey) const{

    glm::vec2 tsize = ImageLoader::getImageSize(texturekey);

    int tx = static_cast<int>(std::fmod(x * tsize.x, tsize.x));
    int ty = static_cast<int>(std::fmod(y * tsize.y, tsize.y));

    Color c = ImageLoader::getPixelColor(texturekey, tx, ty);

    return c;

  };

  glm::vec3 minBound;
  glm::vec3 maxBound;
};