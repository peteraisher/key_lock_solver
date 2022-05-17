//  Copyright (c) 2022 Peter Aisher
//
//  BoundingBox.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_H_
#define KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_H_

#include <algorithm>

#include "../../key_lock_solver/spatial/vec.h"

namespace key_lock_solver {
namespace impl {

struct BoundingBox {
  bool intersects(const BoundingBox& other, Vec3 offset) const;
  inline BoundingBox() = default;
  inline BoundingBox(Vec3 min, Vec3 max) : min(min), max(max) {}
 private:
  friend struct Volume;
  Vec3 min {};
  Vec3 max {};
};

}   // namespace impl
}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SPATIAL_BOUNDINGBOX_H_
