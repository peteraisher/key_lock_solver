//
//  Volume.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef Volume_hpp
#define Volume_hpp

#include <unordered_set>
#include "vec.hpp"
#include "BoundingBox.hpp"



typedef std::unordered_set<Vec3> PointSet;

struct Volume {
  BoundingBox boundingBox {};
  PointSet points {};
  inline bool pointsIntersect(const Volume& other, Vec3 offset) const {
    for (const auto& p : points) {
      if (other.points.find(p + offset) != other.points.end()) {
        return true;
      }
    }
    return false;
  }
  inline bool boundingBoxIntersects(const Volume& other, Vec3 offset) const {
    return boundingBox.intersects(other.boundingBox, offset);
  }
  Volume(std::initializer_list<Vec3> il) : points(il) {
    constexpr char char_max = std::numeric_limits<char>::max();
    constexpr char char_min = std::numeric_limits<char>::min();
    Vec3 bMin {char_max, char_max, char_max};
    Vec3 bMax {char_min, char_min, char_min};
    for (const auto& p : points) {
      bMin = simd_min(bMin, p);
      bMax = simd_max(bMax, p);
    }
    boundingBox = {bMin, bMax};
  }
  Volume(std::initializer_list<char> il)
  : boundingBox {std::numeric_limits<Vec3>::max(),
    std::numeric_limits<Vec3>::min()}{
    points.reserve(il.size() / 3);
      if (il.size() % 3) {
        throw std::runtime_error(
          "Size of initializer list must be a multiple of three.");
      }
    auto p = il.begin();
    const auto end = il.end();
    while (p < end) {
      char x = *(p++);
      char y = *(p++);
      char z = *(p++);
      Vec3 point = {x, y, z};
      points.insert(point);
      boundingBox.min = simd_min(boundingBox.min, point);
      boundingBox.max = simd_max(boundingBox.max, point);
    }
  }
};

#endif /* Volume_hpp */
