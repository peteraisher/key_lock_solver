//  Copyright (c) 2022 Peter Aisher
//
//  Step.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_STEP_H_
#define KEY_LOCK_SOLVER_SEARCH_STEP_H_

#include <iostream>
#include "../../key_lock_solver/search/State.h"

namespace key_lock_solver {
namespace impl {

struct Step {
  Vec3 diff {};
  inline operator bool() const { return hasMove() || hasPieces();}
  bool operator==(const Step& other) const;
  inline Step() {}
  Step(const State& prev, const State& curr);
  void printInstruction(std::ostream& out = std::cout) const;
  void removeCommonPieces(Step* other);

 private:
  std::array<bool, PIECE_COUNT> piece_flags {};
  static constexpr std::array<char, PIECE_COUNT + 1> names {"ABCDEFGHK"};
  bool hasPieces() const;
  inline bool hasMove() const { return !simd_equal(diff, Vec3{});}
  inline void setFlag(size_t i) {piece_flags[i] = true;}
  void printPieceNameList(std::ostream& out = std::cout) const;
  void printCoordinateDirection(std::ostream& out = std::cout) const;
};

}   // namespace impl
}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_STEP_H_
