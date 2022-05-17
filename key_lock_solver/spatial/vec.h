//  Copyright (c) 2022 Peter Aisher
//
//  vec.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_VEC_H_
#define KEY_LOCK_SOLVER_SPATIAL_VEC_H_

#include <simd/simd.h>
#include <limits>
#include <functional>

typedef simd_char3 Vec3;

namespace std {
template <> struct hash<Vec3> {
  size_t operator()(const Vec3 & x) const;
};
template <> struct equal_to<Vec3> {
  bool operator()(const Vec3& a, const Vec3& b) const;
};

template <> struct numeric_limits<Vec3> {
  static constexpr Vec3 max() {
    constexpr char c = numeric_limits<char>::max();
    return {c, c, c};
  }
  static constexpr Vec3 min() {
    constexpr char c = numeric_limits<char>::min();
    return {c, c, c};
  }
};
}   // namespace std

#endif  // KEY_LOCK_SOLVER_SPATIAL_VEC_H_
