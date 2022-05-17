//  Copyright (c) 2022 Peter Aisher
//
//  BoundingBox.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#include "../../key_lock_solver/spatial/BoundingBox.hpp"

bool BoundingBox::intersects(const BoundingBox& other, Vec3 offset) const {
  const Vec3 movedMax = max + offset;
  const Vec3 movedMin = min + offset;

  return !(simd_any(movedMax - other.min) || simd_any(other.max - movedMin));
}
