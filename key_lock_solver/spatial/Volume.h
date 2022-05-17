//  Copyright (c) 2022 Peter Aisher
//
//  Volume.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SPATIAL_VOLUME_H_
#define KEY_LOCK_SOLVER_SPATIAL_VOLUME_H_

#include <unordered_set>
#include <limits>
#include "../../key_lock_solver/spatial/vec.h"
#include "../../key_lock_solver/spatial/BoundingBox.h"



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

#endif  // KEY_LOCK_SOLVER_SPATIAL_VOLUME_H_
