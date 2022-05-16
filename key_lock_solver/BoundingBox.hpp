//
//  BoundingBox.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include "vec.hpp"

struct BoundingBox {
  Vec3 min {};
  Vec3 max {};

  inline bool intersects(const BoundingBox& other, Vec3 offset) const {
    const Vec3 movedMax = max + offset;
    const Vec3 movedMin = min + offset;

    return !(simd_any(movedMax - other.min) || simd_any(other.max - movedMin));
  }

  inline BoundingBox() = default;
  inline BoundingBox(Vec3 min, Vec3 max) : min(min), max(max) {}
};

#endif /* BoundingBox_hpp */
