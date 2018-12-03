#pragma once
#include <vector>

#include "Hitables/Hitable.h"

class HitableList : public Hitable {
 public:
  ~HitableList() override = default;

  void append(const Hitable& hitable);

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

 private:
  std::vector<const Hitable*> hitables;
};