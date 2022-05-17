//  Copyright (c) 2022 Peter Aisher
//
//  Step.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/Step.h"

namespace key_lock_solver {
namespace impl {

bool Step::hasPieces() const {
  return std::any_of(piece_flags.begin(), piece_flags.end(),
                     [](bool v){return v;});
}

void Step::removeCommonPieces(Step* other) {
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (piece_flags[i] && other->piece_flags[i]) {
      piece_flags[i] = false;
      other->piece_flags[i] = false;
    }
  }
}

bool Step::operator==(const Step& other) const {
  if (!simd_equal(diff, other.diff)) {return false;}
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (piece_flags[i] != other.piece_flags[i]) {
      return false;
    }
  }
  return true;
}

Step::Step(const KeyLockPuzzleState& prev, const KeyLockPuzzleState& curr) {
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (!simd_equal(prev.positions[i], curr.positions[i])) {
      if (!prev.isRemovedPiece(i) && !curr.isRemovedPiece(i)) {
        diff = curr.positions[i] - prev.positions[i];
      }
      setFlag(i);
    }
  }
}

void Step::printPieceNameList(std::ostream& out) const {
  size_t printed_count = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (piece_flags[i]) {
      if (printed_count) {
        out << ", ";
      }
      out << names[i];
      ++printed_count;
    }
  }
}

void Step::printCoordinateDirection(std::ostream& out) const {
  char d = 'x';
  char s = '+';
  char v = 0;
  if (diff.x) {
    v = diff.x;
  } else if (diff.y) {
    v = diff.y;
    d = 'y';
  } else if (diff.z) {
    v = diff.z;
    d = 'z';
  }
  if (v < 0) {
    s = '-';
    v = -v;
  }
  out << s << d << " dir";
  if (v > 1) {
    out << ' ' << int(v) << " units";
  }
}

void Step::printInstruction(std::ostream& out) const {
  if (!simd_equal(diff, {})) {
    out << "move ";
    printPieceNameList(out);
    out << " in ";
    printCoordinateDirection(out);
    out << std::endl;
  } else if (hasPieces()) {
    out << "remove ";
    printPieceNameList(out);
    out << std::endl;
  }
}

}   // namespace impl

}   // namespace key_lock_solver
