//  Copyright (c) 2022 Peter Aisher
//
//  KeyLockPuzzleState.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#include "../../key_lock_solver/search/KeyLockPuzzleState.hpp"

bool KeyLockPuzzleState::canRemovePiece(size_t index) const {
  switch (index >> 2) {
    case 0: return abs(positions[index].x) >= 7;
    case 1: return abs(positions[index].y) >= 7;
    case 2: return abs(positions[index].z) >= 7;
    default:
      assert(0);
  }
}

size_t KeyLockPuzzleState::removedCount() const {
  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (isRemovedPiece(i)) { ++tot; }
  }
  return tot;
}

bool KeyLockPuzzleState::operator==(const KeyLockPuzzleState& other) const {
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (!simd_equal(positions[i], other.positions[i])) {return false;}
  }
  return true;
}

namespace std {
size_t hash<KeyLockPuzzleState>::operator()(const KeyLockPuzzleState& s) const {
  size_t seed = std::hash<Vec3>()(s.positions[0]);
  hash_combine(&seed, s.positions[1]);
  hash_combine(&seed, s.positions[2]);
  hash_combine(&seed, s.positions[3]);
  hash_combine(&seed, s.positions[4]);
  hash_combine(&seed, s.positions[5]);
  hash_combine(&seed, s.positions[6]);
  hash_combine(&seed, s.positions[7]);
  hash_combine(&seed, s.positions[8]);
  return seed;
}
}  // namespace std
