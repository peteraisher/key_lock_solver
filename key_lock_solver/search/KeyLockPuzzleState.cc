//  Copyright (c) 2022 Peter Aisher
//
//  State.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/State.h"

namespace key_lock_solver {

bool State::canRemovePiece(size_t index) const {
  switch (index >> 2) {
    case 0: return abs(positions[index].x) >= 7;
    case 1: return abs(positions[index].y) >= 7;
    case 2: return abs(positions[index].z) >= 7;
    default:
      assert(0);
  }
}

size_t State::removedCount() const {
  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (isRemovedPiece(i)) { ++tot; }
  }
  return tot;
}

bool State::operator==(const State& other) const {
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (!simd_equal(positions[i], other.positions[i])) {return false;}
  }
  return true;
}

State State::solved() {
  State result {};
  for (auto& vec : result.positions) {
    vec = std::numeric_limits<Vec3>::max();
  }
  return result;
}

}   // namespace key_lock_solver

namespace std {
size_t hash<key_lock_solver::State>::operator()
(const key_lock_solver::State& s) const {
  using key_lock_solver::impl::hash_combine;
  size_t seed = std::hash<key_lock_solver::Vec3>()(s.positions[0]);
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
