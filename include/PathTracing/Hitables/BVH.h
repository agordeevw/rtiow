#pragma once
#include <vector>

#include "PathTracing/AABB.h"
#include "PathTracing/Hitable.h"

namespace PathTracing {
class AABB;

namespace Hitables {
class BVH : public Hitable {
 public:
  BVH();
  BVH(Hitable** begin, Hitable** end, float time0, float time1);
  BVH(BVH&&) = default;
  BVH& operator=(BVH&&) = default;
  ~BVH() override = default;

  BVH(const BVH&) = delete;
  BVH& operator=(const BVH&) = delete;

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
  bool boundingBox(float t0, float t1, AABB& box) const override;

 private:
  class BVHNode : public Hitable {
   public:
    BVHNode(Hitable* left, Hitable* right, float time0, float time1);
    BVHNode(BVHNode&&) = default;
    BVHNode& operator=(BVHNode&&) = default;
    ~BVHNode() override = default;

    BVHNode(const BVHNode&) = delete;
    BVHNode& operator=(const BVHNode&) = delete;

    bool hit(const Ray& r, float tMin, float tMax,
             HitRecord& rec) const override;
    bool boundingBox(float t0, float t1, AABB& box) const override;

   private:
    friend class BVH;

    Hitable* left;
    Hitable* right;
    AABB box;
  };

  BVHNode* createNode(Hitable** begin, Hitable** end, float time0, float time1);

  BVHNode* root;
  std::vector<BVHNode> nodes;
};
}  // namespace Hitables
}  // namespace PathTracing