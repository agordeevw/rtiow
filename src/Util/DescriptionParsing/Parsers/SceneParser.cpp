#include <type_traits>

#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Textures/CheckerTexture.h"
#include "PathTracing/Textures/ConstantTexture.h"
#include "PathTracing/Textures/ImageTexture.h"
#include "PathTracing/Textures/PerlinNoiseTexture.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"
#include "Util/DescriptionParsing/Parsers/SceneParser.h"

#define TEXTURE_TYPES                            \
  PathTracing::Textures::ConstantTexture,        \
      PathTracing::Textures::CheckerTexture,     \
      PathTracing::Textures::PerlinNoiseTexture, \
      PathTracing::Textures::ImageTexture
#define MATERIAL_TYPES                                                    \
  PathTracing::Materials::Dielectric, PathTracing::Materials::Lambertian, \
      PathTracing::Materials::Metal, PathTracing::Materials::DiffuseLight
#define HITABLE_TYPES \
  PathTracing::Hitables::Sphere, PathTracing::Hitables::MovingSphere

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
namespace detail {
using Json = SceneParser::Json;

template <class BaseType, class T, class... Ts>
void tryParseSceneElementOfType(const Json& jSceneElement,
                                PathTracing::Scene& scene) {
  static_assert(std::is_base_of_v<BaseType, T>);

  if (jSceneElement["type"] == SceneElementParser<T>::getTypeString()) {
    SceneElementParser<T> elemParser(scene);
    elemParser.parse(jSceneElement);
    return;
  }

  if constexpr (sizeof...(Ts) > 0) {
    tryParseSceneElementOfType<BaseType, Ts...>(jSceneElement, scene);
  } else {
    throw std::runtime_error("Unknown type: " +
                             jSceneElement["type"].get<std::string>());
  }
}

template <class BaseType, class... Ts>
void parseSceneGroupOfType(const Json& jSceneGroup, PathTracing::Scene& scene) {
  static_assert(sizeof...(Ts) > 0);

  for (auto& jSceneElement : jSceneGroup)
    tryParseSceneElementOfType<BaseType, Ts...>(jSceneElement, scene);
}
}  // namespace detail

void SceneParser::parse(const Json& jScene) {
  try {
    detail::parseSceneGroupOfType<PathTracing::Texture, TEXTURE_TYPES>(
        jScene["textures"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse textures from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<PathTracing::Material, MATERIAL_TYPES>(
        jScene["materials"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse materials from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<PathTracing::Hitable, HITABLE_TYPES>(
        jScene["hitables"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse hitables from JSON file. Details: ") +
        e.what());
  }
}

}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util