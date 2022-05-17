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

namespace key_lock_solver {

typedef simd_char3 Vec3;

}   // namespace key_lock_solver

namespace std {
template <> struct hash<key_lock_solver::Vec3> {
  size_t operator()(const key_lock_solver::Vec3 & x) const;
};
template <> struct equal_to<key_lock_solver::Vec3> {
  bool operator()(const key_lock_solver::Vec3& a,
                  const key_lock_solver::Vec3& b) const;
};

template <> struct numeric_limits<key_lock_solver::Vec3> {
  static constexpr key_lock_solver::Vec3 max() {
    constexpr char c = numeric_limits<char>::max();
    return {c, c, c};
  }
  static constexpr key_lock_solver::Vec3 min() {
    constexpr char c = numeric_limits<char>::min();
    return {c, c, c};
  }
};
}   // namespace std

#endif  // KEY_LOCK_SOLVER_SPATIAL_VEC_H_
