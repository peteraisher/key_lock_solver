//  Copyright (c) 2022 Peter Aisher
//
//  KeyLockPuzzleState.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_KEYLOCKPUZZLESTATE_HPP_
#define KEY_LOCK_SOLVER_SEARCH_KEYLOCKPUZZLESTATE_HPP_

#include <array>
#include <limits>
#include "../../key_lock_solver/spatial/vec.hpp"
#include "../../key_lock_solver/util/hash_combine.h"

constexpr size_t PIECE_COUNT = 9;

struct KeyLockPuzzleState {
  std::array<Vec3, PIECE_COUNT> positions {};
  bool canRemovePiece(size_t index) const;
  inline bool isRemovedPiece(size_t index) const {
    return simd_equal(positions[index], std::numeric_limits<Vec3>::max());
  }
  size_t removedCount() const;
  bool operator==(const KeyLockPuzzleState& other) const;
  explicit KeyLockPuzzleState(std::array<Vec3, PIECE_COUNT> positions)
    : positions(positions) {}
  KeyLockPuzzleState() {}
};

namespace std {
template <>
struct hash<KeyLockPuzzleState> {
  size_t operator()(const KeyLockPuzzleState & s) const;
};
}  // namespace std

#endif  // KEY_LOCK_SOLVER_SEARCH_KEYLOCKPUZZLESTATE_HPP_
