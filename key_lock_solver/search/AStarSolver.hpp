//  Copyright (c) 2022 Peter Aisher
//
//  AStarSolver.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 13.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_HPP_
#define KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_HPP_

#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include "../../key_lock_solver/search/a_star.hpp"
#include "../../key_lock_solver/spatial/Volume.hpp"
#include "../../key_lock_solver/search/KeyLockPuzzleState.hpp"



class CollisionCache {
 public:
  enum CacheValue : size_t {
    NotStored   = 0,
    Stored      = 1,
    NoCollision = 2,
    Collision   = 3
  };

 private:
  struct Index {
    size_t index;
    size_t bitOffset;
  };
  uint64_t storage[5400] = {};

  inline Index cacheIndex(size_t pairIndex, Vec3 offset) const {
    size_t offsetIndex = size_t(offset.x + 7)
                      + (size_t(offset.y + 7) << 4)
                      + (size_t(offset.z + 7) << 8);
    size_t totalIndex = pairIndex * (15 << 8) + offsetIndex;
    return {.index = totalIndex >> 5, .bitOffset = (totalIndex & 0x1f) << 1};
  }
  inline size_t pairIndex(size_t a, size_t b) const {
    return (((15 - a) * a) >> 1) + (b - 1);
  }

  inline CacheValue getValue(Index index) const {
    return CacheValue((storage[index.index] >> index.bitOffset) & 0x3);
  }

  inline void setValue(Index index, CacheValue value) {
    storage[index.index] |= value << index.bitOffset;
  }

  inline size_t boxPairIndex(size_t piece) const {
    return 36 + piece;
  }

 public:
  inline CacheValue boxCacheValue(size_t a, Vec3 offset) const {
    size_t p = boxPairIndex(a);
    Index index = cacheIndex(p, offset);
    return getValue(index);
  }
  inline void setBoxCacheValue(size_t a, Vec3 offset, CacheValue value) {
    size_t p = boxPairIndex(a);
    Index index = cacheIndex(p, offset);
    setValue(index, value);
  }
  inline CacheValue cacheValue(size_t a, size_t b, Vec3 offset) const {
    size_t p = pairIndex(a, b);
    Index index = cacheIndex(p, offset);
    return getValue(index);
  }
  inline void setCacheValue(size_t a, size_t b, Vec3 offset, CacheValue value) {
    size_t p = pairIndex(a, b);
    Index index = cacheIndex(p, offset);
    setValue(index, value);
  }
};

class AStarSolver {
  CollisionCache collisionCache;

  static const Volume box;

  static const std::array<Volume, PIECE_COUNT> movablePieces;

  size_t cascadeMove(size_t index, Vec3 move, KeyLockPuzzleState* state,
                     std::array<bool, PIECE_COUNT>* moved);
  bool boxCollision(size_t index, Vec3 offset);
  bool haveCollision(size_t firstIndex, size_t secondIndex,
                     Vec3 firstPosition, Vec3 secondPosition);
  std::vector<std::pair<KeyLockPuzzleState, float>>
  possibleMoves(KeyLockPuzzleState state);

 public:
  void solve(KeyLockPuzzleState state = {});
  void reset(KeyLockPuzzleState state = {});
};

#endif  // KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_HPP_
