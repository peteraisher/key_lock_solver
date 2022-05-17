//  Copyright (c) 2022 Peter Aisher
//
//  vec.cc
//  key_lock_solver
//

#include "../../key_lock_solver/spatial/vec.h"

namespace std {
size_t hash<Vec3>::operator()(const Vec3 & x) const {
  union { uint32_t i; Vec3 v; } u = { .v = x };
  return hash<uint32_t>()(u.i);
}
bool equal_to<Vec3>::operator()(const Vec3& a, const Vec3& b) const {
  return simd_equal(a, b);
}
}   // namespace std
