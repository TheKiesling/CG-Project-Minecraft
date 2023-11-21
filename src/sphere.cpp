#include "sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, const Material& mat)
  : center(center), radius(radius), Object(mat) {}

Intersect Sphere::rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
  glm::vec3 oc = rayOrigin - center;

  float a = glm::dot(rayDirection, rayDirection);
  float b = 2.0f * glm::dot(oc, rayDirection);
  float c = glm::dot(oc, oc) - radius * radius;

  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return Intersect{false};
  }

  float dist = (-b - sqrt(discriminant)) / (2.0f * a);

  if (dist < 0) {
    return Intersect{false};
  }

  glm::vec3 point = rayOrigin + dist * rayDirection;
  glm::vec3 normal = glm::normalize(point - center);
  return Intersect{true, dist, point, normal};
}

