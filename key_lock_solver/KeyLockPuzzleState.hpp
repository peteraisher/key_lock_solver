//
//  KeyLockPuzzleState.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KeyLockPuzzleState_hpp
#define KeyLockPuzzleState_hpp

#include <array>
#include "vec.hpp"
#include "hash_combine.h"

constexpr size_t PIECE_COUNT = 9;

struct KeyLockPuzzleState {
  std::array<Vec3, PIECE_COUNT> positions {};
  inline bool canRemovePiece(size_t index) {
    switch (index >> 2) {
      case 0: return abs(positions[index].x) >= 7;
      case 1: return abs(positions[index].y) >= 7;
      case 2: return abs(positions[index].z) >= 7;
      default:
        assert(0);
    }
  }
  inline bool isRemovedPiece(size_t index) const {
    constexpr char c_max = std::numeric_limits<char>::max();
    constexpr Vec3 v_max {c_max, c_max, c_max};
    return simd_equal(positions[index], v_max);
  }
  inline size_t removedCount() const {
    size_t tot = 0;
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (isRemovedPiece(i)) { ++tot; }
    }
    return tot;
  }
  inline bool operator==(const KeyLockPuzzleState& other) const {
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (!simd_equal(positions[i], other.positions[i])) {return false;}
    }
    return true;
  }
  explicit KeyLockPuzzleState(std::array<Vec3, PIECE_COUNT> positions)
  : positions(positions) {}
  KeyLockPuzzleState() {};
};

namespace std {
template <>
struct hash<KeyLockPuzzleState> {
  size_t operator()(const KeyLockPuzzleState & s) const {
    size_t seed = std::hash<Vec3>()(s.positions[0]);
    hash_combine(seed, s.positions[1]);
    hash_combine(seed, s.positions[2]);
    hash_combine(seed, s.positions[3]);
    hash_combine(seed, s.positions[4]);
    hash_combine(seed, s.positions[5]);
    hash_combine(seed, s.positions[6]);
    hash_combine(seed, s.positions[7]);
    hash_combine(seed, s.positions[8]);
    return seed;
  }
};
}

#endif /* KeyLockPuzzleState_hpp */
