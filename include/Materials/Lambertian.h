#pragma once
#include "Material.h"

class Lambertian : public Material {
 public:
  Lambertian(const glm::vec3& a);

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  glm::vec3 albedo;
};