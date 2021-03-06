//  Copyright (c) 2022 Peter Aisher
//
//  CollisionCache.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_H_
#define KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_H_

#include "../../key_lock_solver/spatial/vec.h"

namespace key_lock_solver {
namespace impl {

class CollisionCache {
 public:
  enum CacheValue : size_t {
    CollisionFlag = 1 << 0,
    StoredFlag    = 1 << 1,
    NotStored     = 0,
    NoCollision   = StoredFlag,
    Collision     = StoredFlag | CollisionFlag
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

}   // namespace impl
}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_COLLISIONCACHE_H_
