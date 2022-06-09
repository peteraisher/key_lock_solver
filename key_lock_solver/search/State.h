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
/*
 * State
 *
 * stores the positional offset of the nine movable pieces
 * relative to their starting positions.
 *
 * each position is stored in 12 bits, with the x, y, and z components 4 bits each
 * valid values for component values are -7 to +7
 *
 * component values are stored offset by +8, i.e.:
 * 0b0001 = -7
 * 0b1000 = 0
 * 0b1111 = 7
 *
 * Bit flags r0-8 indicating the removal of a piece are also stored
 *
 * 128 bits of storage arranged are as follows:
 *
 * storage[0]
 *  x0y0 z0x1 y1z1 x2y2 z2x3 y3z3 x8y8 z8--
 * |    |    |    |    |    |    |    |    |
 * 0    8    16   24   32   40   48   56   64
 *
 * storage[1]
 *  x4y4 z4x5 y5z5 x6y6 z6x7 y7z7 r0-7 r8--
 * |    |    |    |    |    |    |    |    |
 * 0    8    16   24   32   40   48   56   64
 */
struct State {
 private:
  friend struct std::hash<State>;
  size_t storage[2] {};
  size_t encodedBitsForPiece(size_t i) const;

  void resetPiecePosition(size_t index);

 public:
  bool canRemovePiece(size_t index) const;
  bool isRemovedPiece(size_t index) const;
  void removePiece(size_t index);
  size_t removedCount() const;
  bool operator==(const State& other) const;

  Vec3 getPosition(size_t i) const;
  void movePosition(size_t i, Vec3 diff);
  /// State
  /// @param positions an array of vectors describing the offsets of the
  /// movable pieces from their original positions.
//  explicit State(std::array<Vec3, PIECE_COUNT> positions);
  State() : storage{0x888888888888888, 0x888888888888} {}
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
