//  Copyright (c) 2022 Peter Aisher
//
//  BoundingBox.cc
//  key_lock_solver
//

#include "../../key_lock_solver/spatial/BoundingBox.h"

bool BoundingBox::intersects(const BoundingBox& other, Vec3 offset) const {
  const Vec3 movedMax = max + offset;
  const Vec3 movedMin = min + offset;

  return !(simd_any(movedMax - other.min) || simd_any(other.max - movedMin));
}
