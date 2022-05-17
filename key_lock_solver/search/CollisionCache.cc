//  Copyright (c) 2022 Peter Aisher
//
//  CollisionCache.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/CollisionCache.h"

namespace key_lock_solver {
namespace impl {

CollisionCache::Index
CollisionCache::cacheIndex(size_t pairIndex, Vec3 offset) const {
  size_t offsetIndex = size_t(offset.x + 7)
                    + (size_t(offset.y + 7) << 4)
                    + (size_t(offset.z + 7) << 8);
  size_t totalIndex = pairIndex * (15 << 8) + offsetIndex;
  return {.index = totalIndex >> 5, .bitOffset = (totalIndex & 0x1f) << 1};
}

CollisionCache::CacheValue
CollisionCache::boxCacheValue(size_t a, Vec3 offset) const {
  size_t p = boxPairIndex(a);
  Index index = cacheIndex(p, offset);
  return getValue(index);
}
void
CollisionCache::setBoxCacheValue(size_t a, Vec3 offset, CacheValue value) {
  size_t p = boxPairIndex(a);
  Index index = cacheIndex(p, offset);
  setValue(index, value);
}
CollisionCache::CacheValue
CollisionCache::cacheValue(size_t a, size_t b, Vec3 offset) const {
  size_t p = pairIndex(a, b);
  Index index = cacheIndex(p, offset);
  return getValue(index);
}
void
CollisionCache::setCacheValue(size_t a, size_t b,
                              Vec3 offset, CacheValue value) {
  size_t p = pairIndex(a, b);
  Index index = cacheIndex(p, offset);
  setValue(index, value);
}

}   // namespace impl
}   // namespace key_lock_solver
