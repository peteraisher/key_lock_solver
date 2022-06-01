//  Copyright (c) 2022 Peter Aisher
//
//  State.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/State.h"

namespace {
enum {
  kBitsPerVector = 12,
  kSlotTwoOffset = 48
};
}   // namespace

static inline int8_t halfCharExpand(size_t bits) {
  assert(bits);
  return static_cast<int8_t>(bits - 8);
}

static inline size_t storageIndexForPiece(size_t i) {
  return (i >> 2) & 1;
}

static inline size_t storagePartForPiece(size_t i) {
  return (i >> 3);
}

static inline size_t storagePartOffsetForPiece(size_t i) {
  return (i & 0x3) * kBitsPerVector;
}

static inline size_t bitIndexForPieceVector(size_t i) {
  return storagePartForPiece(i) * kSlotTwoOffset + storagePartOffsetForPiece(i);
}


namespace key_lock_solver {


static inline void validate(const State& s) {
  size_t enc = s.storage[0];
  for (size_t i = 0; i < 15; ++i) {
    assert(enc & 0xf);
    enc >>= 4;
  }
  enc = s.storage[1];
  for (size_t i = 0; i < 12; ++i) {
    assert(enc & 0xf);
    enc >>= 4;
  }
}

bool State::canRemovePiece(size_t index) const {
  Vec3 pos = getPosition(index);
  return abs(pos.x) >= 7 || abs(pos.y) >= 7 || abs(pos.z) >= 7;
  switch (index >> 2) {
    case 0: return abs(pos.x) >= 7;
    case 1: return abs(pos.y) >= 7;
    case 2: return abs(pos.z) >= 7;
    default:
      assert(0);
  }
  size_t enc = encodedBitsForPiece(index);
  enc >>= (index & ~0x3);
  enc &= 0xf;
  validate(*this);
  return enc == 1 || enc == 0xf;
}

void State::removePiece(size_t index) {
  storage[1] |= (1ULL << (index + kSlotTwoOffset));
}

void State::movePosition(size_t i, Vec3 diff) {
  char s;
  size_t componentOffset = 0;
  if (diff.x) {
    s = diff.x;
  } else if (diff.y) {
    s = diff.y;
    componentOffset = 4;
  } else {
    assert(diff.z);
    s = diff.z;
    componentOffset = 8;
  }
  size_t change = 1ULL << (bitIndexForPieceVector(i) + componentOffset);
  if (s == 1) {
    switch (componentOffset >> 2) {
      case 0: assert(getPosition(i).x != 7); break;
      case 1: assert(getPosition(i).y != 7); break;
      case 2: assert(getPosition(i).z != 7); break;
      default: break;
    }
    storage[storageIndexForPiece(i)] += change;
  } else {
    assert(s == -1);
    switch (componentOffset >> 2) {
      case 0: assert(getPosition(i).x != -7); break;
      case 1: assert(getPosition(i).y != -7); break;
      case 2: assert(getPosition(i).z != -7); break;
      default: break;
    }
    storage[storageIndexForPiece(i)] -= change;
  }
  validate(*this);
}

size_t State::encodedBitsForPiece(size_t i) const {
  return (storage[storageIndexForPiece(i)] >> bitIndexForPieceVector(i))
    & 0xfff;
}

Vec3 State::getPosition(size_t i) const {
  size_t enc = encodedBitsForPiece(i);
  Vec3 result {0, 0, 0};
  result.x = halfCharExpand(enc         & 0xf);
  result.y = halfCharExpand((enc >> 4)  & 0xf);
  result.z = halfCharExpand((enc >> 8)  & 0xf);
  validate(*this);
  return result;
}

bool State::isRemovedPiece(size_t index) const {
  validate(*this);
  return (storage[1] >> (index + kSlotTwoOffset)) & 1;
}

size_t State::removedCount() const {
  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (isRemovedPiece(i)) { ++tot; }
  }
  validate(*this);
  return tot;
}

bool State::operator==(const State& other) const {
  return storage[0] == other.storage[0] && storage[1] == other.storage[1];
}

State State::solved() {
  State result {};
  result.storage[1] = 0x1ffULL << kSlotTwoOffset;
  validate(result);
  return result;
}

}   // namespace key_lock_solver

namespace std {
size_t hash<key_lock_solver::State>::operator()
(const key_lock_solver::State& s) const {
  using key_lock_solver::impl::hash_combine;
  size_t seed = std::hash<key_lock_solver::Vec3>()(s.storage[0]);
  hash_combine(&seed, s.storage[1]);
  return seed;
}
}  // namespace std
