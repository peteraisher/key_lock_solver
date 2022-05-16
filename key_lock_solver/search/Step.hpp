//  Copyright (c) 2022 Peter Aisher
//
//  Step.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 17.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_STEP_HPP_
#define KEY_LOCK_SOLVER_SEARCH_STEP_HPP_

#include <iostream>
#include "../../key_lock_solver/search/KeyLockPuzzleState.hpp"

struct Step {
  Vec3 diff {};
  inline operator bool() const { return hasMove() || hasPieces();}
  bool operator==(const Step& other) const;
  inline Step() {}
  Step(const KeyLockPuzzleState& prev, const KeyLockPuzzleState& curr);
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

#endif  // KEY_LOCK_SOLVER_SEARCH_STEP_HPP_
