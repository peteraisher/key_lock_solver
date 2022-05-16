//
//  AStarSolver.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 13.05.2022.
//

#ifndef AStarSolver_hpp
#define AStarSolver_hpp

#include <vector>
#include <array>
#include "a_star.hpp"
#include "Volume.hpp"
#include "KeyLockPuzzleState.hpp"
#include <iostream>



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

  Volume box = {
    0,0,0, 0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0,
    1,0,0, 1,1,0, 1,2,0, 1,3,0, 1,4,0, 1,5,0, 1,6,0,
    2,0,0, 2,1,0, 2,2,0, 2,3,0, 2,4,0, 2,5,0, 2,6,0,
    3,0,0, 3,1,0,                      3,5,0, 3,6,0,
    4,0,0, 4,1,0, 4,2,0, 4,3,0, 4,4,0, 4,5,0, 4,6,0,
    5,0,0, 5,1,0, 5,2,0, 5,3,0, 5,4,0, 5,5,0, 5,6,0,
    6,0,0, 6,1,0, 6,2,0, 6,3,0, 6,4,0, 6,5,0, 6,6,0,

    0,0,6, 0,1,6, 0,2,6, 0,3,6, 0,4,6, 0,5,6, 0,6,6,
    1,0,6, 1,1,6, 1,2,6, 1,3,6, 1,4,6, 1,5,6, 1,6,6,
    2,0,6, 2,1,6, 2,2,6, 2,3,6, 2,4,6, 2,5,6, 2,6,6,
    3,0,6, 3,1,6,                      3,5,6, 3,6,6,
    4,0,6, 4,1,6, 4,2,6, 4,3,6, 4,4,6, 4,5,6, 4,6,6,
    5,0,6, 5,1,6, 5,2,6, 5,3,6, 5,4,6, 5,5,6, 5,6,6,
    6,0,6, 6,1,6, 6,2,6, 6,3,6, 6,4,6, 6,5,6, 6,6,6,

    0,0,1, 0,6,1, 6,0,1, 6,6,1,
    0,0,2, 0,6,2, 6,0,2, 6,6,2,
    0,0,3, 0,6,3, 6,0,3, 6,6,3,
    0,0,4, 0,6,4, 6,0,4, 6,6,4,
    0,0,5, 0,6,5, 6,0,5, 6,6,5
  };

  std::array<Volume, PIECE_COUNT> movablePieces = {
    Volume{ // A
      0,3,4, 0,3,5, 0,4,4, 0,4,5, 0,5,4, 0,5,5,
      1,3,4, 1,5,4,
      2,5,4, 2,5,5,
      3,5,4,
      4,5,4,
      5,3,4, 5,5,4,
      6,3,4, 6,3,5, 6,4,4, 6,4,5, 6,5,4, 6,5,5
    },
    Volume{ // B
      0,1,3, 0,1,4, 0,1,5, 0,2,3, 0,2,4, 0,2,5,
      1,1,3, 1,1,4, 1,2,3, 1,2,4,
      2,2,3, 2,2,4,
      3,2,3,
      4,2,3,
      5,2,3,
      6,1,3, 6,1,4, 6,1,5, 6,2,3, 6,2,4, 6,2,5
    },
    Volume{ // C
      0,1,1, 0,1,2, 0,2,1, 0,2,2, 0,3,1, 0,3,2,
      1,1,2, 1,2,2,
      2,1,2,
      3,1,2,
      4,1,2,
      5,1,2, 5,2,2,
      6,1,1, 6,1,2, 6,2,1, 6,2,2, 6,3,1, 6,3,2
    },
    { // D
      0,4,1, 0,4,2, 0,4,3, 0,5,1, 0,5,2, 0,5,3,
      1,4,3,
      2,4,3,
      3,4,3,
      4,4,3,
      5,4,3,
      6,4,1, 6,4,2, 6,4,3, 6,5,1, 6,5,2, 6,5,3
    },
    { // E
      1,0,4, 1,0,5, 1,1,5, 1,2,5, 1,3,5, 1,4,5, 1,5,5, 1,6,4, 1,6,5,
      2,0,4, 2,0,5, 2,6,4, 2,6,5,
      3,0,4, 3,0,5, 3,1,4, 3,1,5, 3,6,4, 3,6,5
    },
    { // F
      4,0,3, 4,0,4, 4,0,5, 4,1,5, 4,2,5, 4,3,5, 4,4,5, 4,5,5, 4,6,3, 4,6,4, 4,6,5,
      5,0,3, 5,0,4, 5,0,5, 5,6,3, 5,6,4, 5,6,5
    },
    { // G
      3,0,1, 3,0,2, 3,1,1, 3,5,1, 3,5,2, 3,6,1, 3,6,2,
      4,0,1, 4,0,2, 4,1,1, 4,2,1, 4,6,1, 4,6,2,
      5,0,1, 5,0,2, 5,1,1, 5,2,1, 5,3,1, 5,4,1, 5,5,1, 5,6,1, 5,6,2
    },
    { // H
      1,0,1, 1,0,2, 1,0,3, 1,6,1, 1,6,2, 1,6,3,
      2,0,1, 2,0,2, 2,0,3, 2,1,1, 2,2,1, 2,3,1, 2,4,1, 2,5,1, 2,5,3, 2,6,1, 2,6,2, 2,6,3,
    },
    { // K
      3,2,0, 3,2,4, 3,2,5, 3,2,6, 3,2,10, 3,2,11, 3,2,12,
      3,3,0, 3,3,2, 3,3,3, 3,3,4, 3,3,6, 3,3,7, 3,3,8, 3,3,9, 3,3,10, 3,3,11, 3,3,12,
      3,4,0, 3,4,1, 3,4,2, 3,4,4, 3,4,6, 3,4,10, 3,4,11, 3,4,12
    }
  };

  size_t cascadeMove(size_t index, Vec3 move, KeyLockPuzzleState& state, std::array<bool, PIECE_COUNT>& moved);
  bool boxCollision(size_t index, Vec3 offset);
  bool haveCollision(size_t firstIndex, size_t secondIndex,
                     Vec3 firstPosition, Vec3 secondPosition);

public:
  std::vector<std::pair<KeyLockPuzzleState, float>> possibleMoves(KeyLockPuzzleState state);
  
  void solve(KeyLockPuzzleState state = {});
  void reset(KeyLockPuzzleState state = {});
};

#endif /* AStarSolver_hpp */
