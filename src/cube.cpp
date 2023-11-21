#include "cube.h" 

Cube::Cube(const glm::vec3& center, float size, const Material& mat) : center(center), size(size), Object(mat) {}

Intersect Cube::rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
  float tmin = (center.x - size - rayOrigin.x) / rayDirection.x;
  float tmax = (center.x + size - rayOrigin.x) / rayDirection.x;

  if (tmin > tmax) {
    std::swap(tmin, tmax);
  }

  float tymin = (center.y - size - rayOrigin.y) / rayDirection.y;
  float tymax = (center.y + size - rayOrigin.y) / rayDirection.y;

  if (tymin > tymax) {
    std::swap(tymin, tymax);
  }

  if ((tmin > tymax) || (tymin > tmax)) {
    return Intersect{false};
  }

  if (tymin > tmin) {
    tmin = tymin;
  }

  if (tymax < tmax) {
    tmax = tymax;
  }

  float tzmin = (center.z - size - rayOrigin.z) / rayDirection.z;
  float tzmax = (center.z + size - rayOrigin.z) / rayDirection.z;

  if (tzmin > tzmax) {
    std::swap(tzmin, tzmax);
  }

  if ((tmin > tzmax) || (tzmin > tmax)) {
    return Intersect{false};
  }

  if (tzmin > tmin) {
    tmin = tzmin;
  }

  if (tzmax < tmax) {
    tmax = tzmax;
  }

  float dist = tmin;

  if (dist < 0) {
    dist = tmax;
    if (dist < 0) {
      return Intersect{false};
    }
  }

  glm::vec3 point = rayOrigin + dist * rayDirection;
  glm::vec3 normal = glm::normalize(point - center);
  return Intersect{true, dist, point, normal};
}
