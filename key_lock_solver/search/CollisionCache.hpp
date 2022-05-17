//  Copyright (c) 2022 Peter Aisher
//
//  CollisionCache.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 17.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_HPP_
#define KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_HPP_

#include "../../key_lock_solver/spatial/vec.hpp"

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
  
  Index cacheIndex(size_t pairIndex, Vec3 offset) const;
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
  CacheValue boxCacheValue(size_t a, Vec3 offset) const;
  void setBoxCacheValue(size_t a, Vec3 offset, CacheValue value);
  CacheValue cacheValue(size_t a, size_t b, Vec3 offset) const;
  void setCacheValue(size_t a, size_t b, Vec3 offset, CacheValue value);
};

#endif  // KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_HPP_
