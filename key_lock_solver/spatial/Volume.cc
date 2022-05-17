//  Copyright (c) 2022 Peter Aisher
//
//  Volume.cc
//  key_lock_solver
//

#include "../../key_lock_solver/spatial/Volume.h"

namespace key_lock_solver {

namespace impl {

bool Volume::pointsIntersect(const Volume& other, Vec3 offset) const {
  return std::any_of(points.begin(), points.end(), [&](const auto& p) {
    return other.points.find(p + offset) != other.points.end();
  });
}

Volume::Volume(std::initializer_list<char> il)
  : boundingBox {std::numeric_limits<Vec3>::max(),
    std::numeric_limits<Vec3>::min()} {
  points.reserve(il.size() / 3);
    if (il.size() % 3) {
      throw std::runtime_error(
        "Size of initializer list must be a multiple of three.");
    }
  auto p = il.begin();
  const auto end = il.end();
  while (p < end) {
    char x = *(p++);
    char y = *(p++);
    char z = *(p++);
    Vec3 point = {x, y, z};
    points.insert(point);
    boundingBox.min = simd_min(boundingBox.min, point);
    boundingBox.max = simd_max(boundingBox.max, point);
  }
}

}   // namespace impl
}   // namespace key_lock_solver
