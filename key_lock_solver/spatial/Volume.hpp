//  Copyright (c) 2022 Peter Aisher
//
//  Volume.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_VOLUME_HPP_
#define KEY_LOCK_SOLVER_SPATIAL_VOLUME_HPP_

#include <unordered_set>
#include <limits>
#include "../../key_lock_solver/spatial/vec.hpp"
#include "../../key_lock_solver/spatial/BoundingBox.hpp"



typedef std::unordered_set<Vec3> PointSet;

struct Volume {
  BoundingBox boundingBox {};
  PointSet points {};
  bool pointsIntersect(const Volume& other, Vec3 offset) const;
  inline bool boundingBoxIntersects(const Volume& other, Vec3 offset) const {
    return boundingBox.intersects(other.boundingBox, offset);
  }
  Volume(std::initializer_list<char> il);
};

#endif  // KEY_LOCK_SOLVER_SPATIAL_VOLUME_HPP_
