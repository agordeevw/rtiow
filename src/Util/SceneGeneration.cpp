#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Hitables/Sphere.h"
#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Scene.h"
#include "Util/MyRandom.h"
#include "Util/SceneGeneration.h"

Scene SceneGeneration::randomSpheres() {
  using MyRandom::randf;

  Scene scene;

  scene.createHitable<Sphere>(
      glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
      scene.createMaterial<Lambertian>(glm::vec3{0.5f, 0.5f, 0.5f}));

  const Material& glass = scene.createMaterial<Dielectric>(1.5f);

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float chooseMat = randf();
      glm::vec3 center(a + 0.9f * randf(), 0.2f, b + 0.9f * randf());
      if (glm::length(center - glm::vec3{4.0f, 0.2f, 0.0f}) > 0.9f) {
        if (chooseMat < 0.8f) {  // diffuse
          const Material& lambertian =
              scene.createMaterial<Lambertian>(glm::vec3{
                  randf() * randf(), randf() * randf(), randf() * randf()});
          scene.createHitable<Sphere>(center, 0.2f, lambertian);
        } else if (chooseMat < 0.95f) {  // metal
          const Material& metal = scene.createMaterial<Metal>(
              0.5f * (glm::vec3{randf(), randf(), randf()} + 1.0f),
              0.5f * randf());
          scene.createHitable<Sphere>(center, 0.2f, metal);
        } else {  // glass
          scene.createHitable<Sphere>(center, 0.2f, glass);
        }
      }
    }

    scene.createHitable<Sphere>(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, glass);

    scene.createHitable<Sphere>(
        glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Lambertian>(glm::vec3{0.4f, 0.2f, 0.1f}));

    scene.createHitable<Sphere>(
        glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Metal>(glm::vec3{0.7f, 0.6f, 0.5f}, 0.0f));
  }

  return scene;
}
