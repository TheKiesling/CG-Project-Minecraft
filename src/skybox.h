#pragma once
#include <glm/glm.hpp>
#include "intersect.h"
#include "object.h"
#include <string>
#include "imageloader.h"
#include <cmath>

class Skybox
{

public:
  static Color getColor(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
  {

    glm::vec3 minBound(-100.0f, -50.0f, 100.0f);
    glm::vec3 maxBound(100.0f, 70.0f, -100.0f);

    glm::vec3 invRayDir = 1.0f / rayDirection;

    glm::vec3 t1 = (minBound - rayOrigin) * invRayDir;
    glm::vec3 t2 = (maxBound - rayOrigin) * invRayDir;

    glm::vec3 tmin = glm::min(t1, t2);
    glm::vec3 tmax = glm::max(t1, t2);

    float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
    float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

    if (tNear > tFar || tFar < 0)
    {
      return Color(173, 216, 230);
    }

    float dist = (tNear < 0) ? tFar : tNear;

    glm::vec3 point = rayOrigin + dist * rayDirection;

    // Calcula la normal basada en el lado del cubo
    const float epsilon = 0.0001;
    glm::vec3 normal(0.0f);
    if (glm::abs(point.x - minBound.x) < epsilon)
    {
      return loadTexture(std::abs(point.z - minBound.z), std::abs(point.y - minBound.y), std::abs(maxBound.z - minBound.z), std::abs(maxBound.y - minBound.y), "sideSky");
    }
    else if (glm::abs(point.x - maxBound.x) < epsilon)
    {
      return loadTexture(std::abs(point.z - minBound.z), std::abs(point.y - minBound.y), std::abs(maxBound.z - minBound.z), std::abs(maxBound.y - minBound.y), "sideSky");

    }
    else if (glm::abs(point.y - minBound.y) < epsilon)
    {
      return loadTexture(std::abs(point.x - minBound.x), std::abs(point.z - minBound.z), std::abs(maxBound.x - minBound.x), std::abs(maxBound.z- minBound.z), "floor");
    }
    else if (glm::abs(point.y - maxBound.y) < epsilon)
    {
      return loadTexture(std::abs(point.x - minBound.x), std::abs(point.z - minBound.z), std::abs(maxBound.x - minBound.x), std::abs(maxBound.z- minBound.z), "upSky");
    }
    else if (glm::abs(point.z - minBound.z) < epsilon)
    {
      return loadTexture(std::abs(point.x - minBound.x), std::abs(point.y - minBound.y), std::abs(maxBound.x - minBound.x), std::abs(maxBound.y - minBound.y), "sideSky");

    }
    else if (glm::abs(point.z - maxBound.z) < epsilon)
    {
      return loadTexture(std::abs(point.x - minBound.x), std::abs(point.y - minBound.y), std::abs(maxBound.x - minBound.x), std::abs(maxBound.y - minBound.y), "sideSky");

    }

    return Color(173, 216, 230);
  };

  static Color loadTexture(float x, float y, float surfaceWidth, float surfaceHeight, const std::string texturekey)
  {

    glm::vec2 tsize = ImageLoader::getImageSize(texturekey);

    float normalizedX = x / surfaceWidth;
    float normalizedY = y / surfaceHeight;

    int tx = static_cast<int>(std::fmod(normalizedX * tsize.x, tsize.x));
    int ty = static_cast<int>(std::fmod(normalizedY * tsize.y, tsize.y));

    Color c = ImageLoader::getPixelColor(texturekey, tx, ty);

    return c;
  };
};
