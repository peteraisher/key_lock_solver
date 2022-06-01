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
  /*
   * x, y, z component of each position range from -7 to +7
   * encoded in 4 bits
   * 0b1000 = 0
   * 0b1111 = 7
   * 0b0001 = -7
   * 12 bits per piece
   *
   * 128 bits of storage in nibbles as follows:
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
  size_t storage[2] {};
  size_t encodedBitsForPiece(size_t i) const;
//  std::array<Vec3, PIECE_COUNT> positions {};
// public:
  bool canRemovePiece(size_t index) const;
  bool isRemovedPiece(size_t index) const;
  void removePiece(size_t index);
  size_t removedCount() const;
  bool operator==(const State& other) const;

  Vec3 getPosition(size_t i) const;
  void setPosition(size_t i, Vec3 pos);
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
