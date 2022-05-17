//  Copyright (c) 2022 Peter Aisher
//
//  BoundingBox.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_HPP_
#define KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_HPP_

#include <algorithm>

#include "../../key_lock_solver/spatial/vec.hpp"

struct BoundingBox {
  bool intersects(const BoundingBox& other, Vec3 offset) const;
  inline BoundingBox() = default;
  inline BoundingBox(Vec3 min, Vec3 max) : min(min), max(max) {}
 private:
  friend struct Volume;
  Vec3 min {};
  Vec3 max {};
};

#endif  // KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_HPP_
