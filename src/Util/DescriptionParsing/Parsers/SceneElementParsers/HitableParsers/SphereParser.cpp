#include <glm/vec3.hpp>

#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Hitables::Sphere;

const char* SceneElementParser<Type>::getTypeString() { return "sphere"; }

void SceneElementParser<Type>::parse(const Json& jHitable) {
  glm::vec3 center = parseVec3(jHitable["center"]);
  float radius = jHitable["radius"].get<float>();
  int materialId = jHitable["material"].get<int>();
  scene.createHitable<Type>(center, radius, scene.getMaterialById(materialId));
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util