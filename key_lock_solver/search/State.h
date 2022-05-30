//  Copyright (c) 2022 Peter Aisher
//
//  State.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_STATE_H_
#define KEY_LOCK_SOLVER_SEARCH_STATE_H_

#include <array>
#include <limits>
#include "../../key_lock_solver/spatial/vec.h"
#include "../../key_lock_solver/util/hash_combine.h"

namespace key_lock_solver {

constexpr size_t PIECE_COUNT = 9;

struct State {
  std::array<Vec3, PIECE_COUNT> positions {};
  bool canRemovePiece(size_t index) const;
  inline bool isRemovedPiece(size_t index) const {
    return simd_equal(positions[index], std::numeric_limits<Vec3>::max());
  }
  size_t removedCount() const;
  bool operator==(const State& other) const;

  /// State
  /// @param positions an array of vectors describing the offsets of the
  /// movable pieces from their original positions.
  explicit State(std::array<Vec3, PIECE_COUNT> positions)
    : positions(positions) {}
  State() {}
  static State solved();
};

}   // namespace key_lock_solver

namespace std {
template <>
struct hash<key_lock_solver::State> {
  size_t operator()(const key_lock_solver::State & s) const;
};
}  // namespace std

#endif  // KEY_LOCK_SOLVER_SEARCH_STATE_H_
