//  Copyright (c) 2022 Peter Aisher
//
//  AStarSolver.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/AStarSolver.h"
#include <algorithm>
#include <iostream>
#include "../search/a_star.h"
#include "../search/Step.h"

namespace key_lock_solver {

using impl::Step;
using impl::a_star;
using impl::Volume;

const Volume AStarSolver::box = {
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

const std::array<Volume, PIECE_COUNT> AStarSolver::movablePieces = {
  Volume{   // A
    0,3,4, 0,3,5, 0,4,4, 0,4,5, 0,5,4, 0,5,5,
    1,3,4, 1,5,4,
    2,5,4, 2,5,5,
    3,5,4,
    4,5,4,
    5,3,4, 5,5,4,
    6,3,4, 6,3,5, 6,4,4, 6,4,5, 6,5,4, 6,5,5
  },
  {   // B
    0,1,3, 0,1,4, 0,1,5, 0,2,3, 0,2,4, 0,2,5,
    1,1,3, 1,1,4, 1,2,3, 1,2,4,
    2,2,3, 2,2,4,
    3,2,3,
    4,2,3,
    5,2,3,
    6,1,3, 6,1,4, 6,1,5, 6,2,3, 6,2,4, 6,2,5
  },
  {   // C
    0,1,1, 0,1,2, 0,2,1, 0,2,2, 0,3,1, 0,3,2,
    1,1,2, 1,2,2,
    2,1,2,
    3,1,2,
    4,1,2,
    5,1,2, 5,2,2,
    6,1,1, 6,1,2, 6,2,1, 6,2,2, 6,3,1, 6,3,2
  },
  {   // D
    0,4,1, 0,4,2, 0,4,3, 0,5,1, 0,5,2, 0,5,3,
    1,4,3,
    2,4,3,
    3,4,3,
    4,4,3,
    5,4,3,
    6,4,1, 6,4,2, 6,4,3, 6,5,1, 6,5,2, 6,5,3
  },
  {   // E
    1,0,4, 1,0,5, 1,1,5, 1,2,5, 1,3,5, 1,4,5, 1,5,5, 1,6,4, 1,6,5,
    2,0,4, 2,0,5, 2,6,4, 2,6,5,
    3,0,4, 3,0,5, 3,1,4, 3,1,5, 3,6,4, 3,6,5
  },
  {   // F
    4,0,3, 4,0,4, 4,0,5, 4,1,5, 4,2,5, 4,3,5, 4,4,5, 4,5,5, 4,6,3, 4,6,4, 4,6,5,
    5,0,3, 5,0,4, 5,0,5, 5,6,3, 5,6,4, 5,6,5
  },
  {   // G
    3,0,1, 3,0,2, 3,1,1, 3,5,1, 3,5,2, 3,6,1, 3,6,2,
    4,0,1, 4,0,2, 4,1,1, 4,2,1, 4,6,1, 4,6,2,
    5,0,1, 5,0,2, 5,1,1, 5,2,1, 5,3,1, 5,4,1, 5,5,1, 5,6,1, 5,6,2
  },
  {   // H
    1,0,1, 1,0,2, 1,0,3, 1,6,1, 1,6,2, 1,6,3,
    2,0,1, 2,0,2, 2,0,3, 2,1,1, 2,2,1, 2,3,1,
    2,4,1, 2,5,1, 2,5,3, 2,6,1, 2,6,2, 2,6,3
  },
  {   // K
    3,2,0, 3,2,4, 3,2,5, 3,2,6, 3,2,10, 3,2,11, 3,2,12, 3,3,0, 3,3,2, 3,3,3,
    3,3,4, 3,3,6, 3,3,7, 3,3,8, 3,3,9, 3,3,10, 3,3,11, 3,3,12,
    3,4,0, 3,4,1, 3,4,2, 3,4,4, 3,4,6, 3,4,10, 3,4,11, 3,4,12
  }
};

size_t AStarSolver::cascadeMove(size_t index, Vec3 move,
                                State* state,
                                std::array<bool, PIECE_COUNT>* moved) {
  const auto startPos = state->getPosition(index);
  const auto newPos = startPos + move;
  if (boxCollision(index, newPos)) {
    return 0;
  }
  state->movePosition(index, move);
  (*moved)[index] = true;

  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (index == i
        || state->isRemovedPiece(i)
        || !haveCollision(index, i, newPos, state->getPosition(i))) {
      continue;
    }
    size_t res = cascadeMove(i, move, state, moved);
    if (res) {
      tot += res;
    } else {
      return 0;
    }
  }
  return tot + 1;
}

bool AStarSolver::boxCollision(size_t index, Vec3 offset) {
  CollisionCache::CacheValue cv = collisionCache.boxCacheValue(index, offset);
  if (cv != impl::CollisionCache::NotStored) {
    return cv == impl::CollisionCache::Collision;
  }
  bool result = movablePieces[index].pointsIntersect(box, offset);
  cv = result ? CollisionCache::Collision : CollisionCache::NoCollision;
  collisionCache.setBoxCacheValue(index, offset, cv);
  return result;
}

bool AStarSolver::haveCollision(size_t firstIndex, size_t secondIndex,
                                Vec3 firstPosition, Vec3 secondPosition) {
  Vec3 offset;
  if (secondIndex < firstIndex) {
    std::swap(firstIndex, secondIndex);
    offset = secondPosition - firstPosition;
  } else {
    offset = firstPosition - secondPosition;
  }

  // no collision if x, y, or z component of offset >= 7
  if (simd_any(6 - simd_abs(offset))) { return false; }

  // check cache
  CollisionCache::CacheValue cv =
    collisionCache.cacheValue(firstIndex,secondIndex, offset);
  if (cv != impl::CollisionCache::NotStored) {
    return cv == CollisionCache::Collision;
  }

  // check bounding box
  auto& firstBody = movablePieces[firstIndex];
  auto& secondBody = movablePieces[secondIndex];
  if (!firstBody.boundingBox.intersects(secondBody.boundingBox, offset)) {
    collisionCache.setCacheValue(firstIndex, secondIndex, offset,
                                 impl::CollisionCache::NoCollision);
    return false;
  }

  // check points
  auto result = firstBody.pointsIntersect(secondBody, offset);
  cv = result ? CollisionCache::Collision : CollisionCache::NoCollision;
  collisionCache.setCacheValue(firstIndex, secondIndex, offset, cv);
  return result;
}


std::vector<std::pair<State, float>>
AStarSolver::getPossibleMoves(State state) {
  populatePossibleMoves(state);
  return possibleMoves;
}

void AStarSolver::clearPossibleMoves() {
  possibleMoves.reserve(48);
  possibleMoves.resize(0);
}

void AStarSolver::addMoveRemovingPiece(size_t i, const State &start) {
  auto newState = start;
  newState.removePiece(i);
  possibleMoves.emplace_back(newState, 0.f);
}

bool AStarSolver::skipOrRemovePiece(size_t i, const State &start) {
  if (start.isRemovedPiece(i)) {
    return true;
  }
  if (start.canRemovePiece(i)) {
    addMoveRemovingPiece(i, start);
    return true;
  }
  return false;
}

static float costFromMovedPieceCount(size_t res) {
  return sqrtf(static_cast<float>(res));
}

void AStarSolver::addCascadedMoveIfPossible(size_t i, const Vec3 &move,
                                            const State &start) {
  auto stateCopy = start;
  std::array<bool, PIECE_COUNT> moved {};
  size_t res = cascadeMove(i, move, &stateCopy, &moved);
  if (res) {
    float cost = costFromMovedPieceCount(res);
    possibleMoves.emplace_back(stateCopy, cost);
  }
}

void AStarSolver::addPossibleMovesForPiece(size_t i, const State &start) {
  if (skipOrRemovePiece(i, start)) {
    return;
  }

  std::array<Vec3, 6> moves = {
    Vec3{1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1}
  };

  for (const auto& move : moves) {
    addCascadedMoveIfPossible(i, move, start);
  }
}

void
AStarSolver::populatePossibleMoves(State state) {
  clearPossibleMoves();

  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    addPossibleMovesForPiece(i, state);
  }
}

enum SolveType {
  Solve = 0,
  Reset = 1
};

void AStarSolver::solve(State start) {
  auto helper = impl::ProgressiveSolveHelper(this);
  helper.solve(start);
}

void AStarSolver::reset(State start) {
  auto helper = impl::ProgressiveSolveHelper(this);
  helper.reset(start);
}

}   // namespace key_lock_solver
