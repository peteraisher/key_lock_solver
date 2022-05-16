//  Copyright (c) 2022 Peter Aisher
//
//  vec.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_VEC_HPP_
#define KEY_LOCK_SOLVER_SPATIAL_VEC_HPP_

#include <immintrin.h>
#include <limits>
#include <functional>

#if 0
typedef __attribute__((__ext_vector_type__(3))) char Vec3;
#else
#include <simd/simd.h>
typedef simd_char3 Vec3;

namespace std {
template <> struct hash<Vec3> {
  size_t operator()(const Vec3 & x) const {
    union { uint32_t i; Vec3 v; } u = { .v = x };
    return hash<uint32_t>()(u.i);
  }
};
template <> struct equal_to<Vec3> {
  size_t operator()(const Vec3& a, const Vec3& b) const {
    return simd_equal(a, b);
  }
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

#endif

#endif  // KEY_LOCK_SOLVER_SPATIAL_VEC_HPP_
