#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/geometric.hpp>

#include "Camera.h"
#include "Util/MyRandom.h"

Camera::Camera(const CameraParameters& params) {
  lensRadius = params.aperture * 0.5f;
  float theta = params.fov * float(M_PI) / 180.0f;
  float halfHeight = tanf(theta * 0.5f);
  float halfWidth = params.aspectRatio * halfHeight;
  origin = params.lookFrom;
  w = glm::normalize(params.lookFrom - params.lookAt);
  u = glm::normalize(glm::cross(params.up, w));
  v = glm::cross(w, u);
  lowerLeftCorner =
      origin - params.focusDist * (halfWidth * u + halfHeight * v + w);
  horizontal = 2.0f * halfWidth * params.focusDist * u;
  vertical = 2.0f * halfHeight * params.focusDist * v;
}

Ray Camera::getRay(float s, float t) const {
  glm::vec3 rd = lensRadius * MyRandom::randInUnitDisk();
  glm::vec3 offset = u * rd.x + v * rd.y;
  return {origin + offset,
          lowerLeftCorner + s * horizontal + t * vertical - (origin + offset)};
}
