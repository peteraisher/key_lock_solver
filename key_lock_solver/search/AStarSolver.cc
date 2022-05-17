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
    2,4,1, 2,5,1, 2,5,3, 2,6,1, 2,6,2, 2,6,3,
  },
  {   // K
    3,2,0, 3,2,4, 3,2,5, 3,2,6, 3,2,10, 3,2,11, 3,2,12, 3,3,0, 3,3,2, 3,3,3,
    3,3,4, 3,3,6, 3,3,7, 3,3,8, 3,3,9, 3,3,10, 3,3,11, 3,3,12,
    3,4,0, 3,4,1, 3,4,2, 3,4,4, 3,4,6, 3,4,10, 3,4,11, 3,4,12
  }
};

size_t AStarSolver::cascadeMove(size_t index, Vec3 move,
                                KeyLockPuzzleState* state,
                                std::array<bool, PIECE_COUNT>* moved) {
  const auto startPos = state->positions[index];
  const auto newPos = startPos + move;
  if (boxCollision(index, newPos)) {
    return 0;
  }
  state->positions[index] = newPos;
  (*moved)[index] = true;

  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (index != i && !state->isRemovedPiece(i)
        && haveCollision(index, i, newPos, state->positions[i])) {
      if ((*moved)[i]) { return 0; }
      size_t res = cascadeMove(i, move, state, moved);
      if (res) {
        tot += res;
      } else {
        return 0;
      }
    }
  }
  return tot + 1;
}

bool AStarSolver::boxCollision(size_t index, Vec3 offset) {
  CollisionCache::CacheValue cv = collisionCache.boxCacheValue(index, offset);
  if (cv) {
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
    auto t = firstIndex;
    firstIndex = secondIndex;
    secondIndex = t;
    offset = firstPosition - secondPosition;
  } else {
    offset = secondPosition - firstPosition;
  }

  offset = -offset;

  auto& firstBody = movablePieces[firstIndex];
  auto& secondBody = movablePieces[secondIndex];
  if (!firstBody.boundingBox.intersects(secondBody.boundingBox, offset)) {
    return false;
  }
  CollisionCache::CacheValue cv =
    collisionCache.cacheValue(firstIndex,secondIndex, offset);
  if (cv) {
    return cv == CollisionCache::Collision;
  }
  auto result = firstBody.pointsIntersect(secondBody, offset);
  cv = result ? CollisionCache::Collision : CollisionCache::NoCollision;
  collisionCache.setCacheValue(firstIndex, secondIndex, offset, cv);
  return result;
}

static inline float searchHeuristic(KeyLockPuzzleState state) {
  int result = 0;
  size_t i = 0;
  for (; i < 4; ++i) {
    result += std::max(8 - abs(state.positions[i].x), 0);
  }
  for (; i < 8; ++i) {
    result += std::max(8 - abs(state.positions[i].y), 0);
  }
  result += std::max(8 - abs(state.positions[i].z), 0);
  return static_cast<float>(result);
}

static inline float resetHeuristic(KeyLockPuzzleState state,
                                   KeyLockPuzzleState target) {
  int result = 0;
  size_t i = 0;
  for (; i < 4; ++i) {
    result += abs(state.positions[i].x - target.positions[i].x);
  }
  for (; i < 8; ++i) {
    result += abs(state.positions[i].y - target.positions[i].y);
  }
  result += abs(state.positions[i].z - target.positions[i].z);
  return static_cast<float>(result);
}

static void consolidateStepsInSameDirection(std::vector<Step>* steps) {
  auto consolidatedStep = steps->begin();
  auto readStep = steps->begin();
  auto step = steps->begin();
  const auto end = steps->end();
  while (++step < end) {
    size_t count = 0;
    while (*step == *readStep) {
      ++count;
      ++step;
    }
    if (count) {
      readStep->diff *= (count + 1);
    }
    *consolidatedStep = *readStep;
    ++consolidatedStep;
    readStep = step;
  }
  steps->resize(consolidatedStep - steps->begin());
}

static void removeEmptySteps(std::vector<Step>* steps) {
  auto writeStep = steps->begin();
  auto readStep = steps->begin();
  const auto end = steps->end();
  while (readStep < end) {
    if (*readStep) {
      *writeStep = *readStep;
      ++writeStep;
    }
    ++readStep;
  }
  steps->resize(writeStep - steps->begin());
}

static void printSolution(std::vector<KeyLockPuzzleState> solution,
                   KeyLockPuzzleState start, std::ostream& out = std::cout) {
  KeyLockPuzzleState lastState = start;


  std::vector<Step> steps {};

  for (const auto& state : solution) {
    steps.emplace_back(lastState, state);
    lastState = state;
  }
  for (size_t i = 0, j = 1; j < steps.size(); ++i, ++j) {
    if (simd_equal(steps[i].diff, -steps[j].diff)) {
      steps[i].removeCommonPieces(&steps[j]);
    }
  }
  removeEmptySteps(&steps);
  consolidateStepsInSameDirection(&steps);
  for (const auto& step : steps) {
    step.printInstruction(out);
  }
}

std::vector<std::pair<KeyLockPuzzleState, float>>
AStarSolver::possibleMoves(KeyLockPuzzleState state) {
  std::vector<std::pair<KeyLockPuzzleState, float>> result {};
  result.reserve(48);
  std::array<Vec3, 6> moves = {
    Vec3{1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1}
  };
  auto newPositions = state.positions;
  for (size_t i = 0; i < state.positions.size(); ++i) {
    if (state.isRemovedPiece(i)) {
      continue;
    }

    if (state.canRemovePiece(i)) {
      newPositions[i] = std::numeric_limits<Vec3>::max();
      result.emplace_back(newPositions, 0.f);
      newPositions[i] = state.positions[i];
      continue;
    }
    for (const auto& move : moves) {
      auto stateCopy = state;
      std::array<bool, PIECE_COUNT> moved {};
      size_t res = cascadeMove(i, move, &stateCopy, &moved);
      if (res) {
        float cost = static_cast<float>(res);
//        cost = 1.f + ((cost - 1.f) * 0.875f);
        cost = sqrtf(cost);
        result.push_back({stateCopy, cost});
      }
    }
  }
  return result;
}

void AStarSolver::solve(KeyLockPuzzleState state) {
  KeyLockPuzzleState lastSolveState = state;
  std::vector<KeyLockPuzzleState> totalSolution = {state};

  for (size_t i = 0; i < 9; ++i) {
    std::vector<KeyLockPuzzleState> solution =
    a_star<KeyLockPuzzleState>(
      lastSolveState,
      [&i](KeyLockPuzzleState s){return s.removedCount() > i;},
      [&](KeyLockPuzzleState s){return possibleMoves(s);}, searchHeuristic);
    if (solution.empty()) {
      auto rc = lastSolveState.removedCount();
      std::cout << "Failed to find solution on step " << i + 1 << std::endl;
      std::cout << "Removed " << rc << " pieces." << std::endl;
      return;
    }
    totalSolution.pop_back();
    totalSolution.insert(totalSolution.end(),
                         solution.rbegin(), solution.rend());
    lastSolveState = totalSolution.back();
  }
  printSolution(totalSolution, state);
}

void AStarSolver::reset(KeyLockPuzzleState target) {
  KeyLockPuzzleState state {};
  KeyLockPuzzleState lastSolveState = state;
  std::vector<KeyLockPuzzleState> totalSolution = {state};
  for (size_t i = 0; i <= target.removedCount(); ++i) {
    std::vector<KeyLockPuzzleState> solution =
    a_star<KeyLockPuzzleState>(
      lastSolveState,
      [&](KeyLockPuzzleState s){
        if (s.removedCount() < i) { return false;}
        for (size_t j = 0; j < PIECE_COUNT; ++j) {
          if (s.isRemovedPiece(j) && !target.isRemovedPiece(j)) {return false;}
        }
        return s.removedCount() > i || s == target;
      },
      [&](KeyLockPuzzleState s){return possibleMoves(s);},
      [&](KeyLockPuzzleState s){return resetHeuristic(s, target);});
    if (solution.empty()) {
      auto rc = lastSolveState.removedCount();
      std::cout << "Failed to find solution on step " << i + 1 << std::endl;
      std::cout << "Removed " << rc << " pieces." << std::endl;
      return;
    }
    totalSolution.pop_back();
    totalSolution.insert(totalSolution.end(),
                         solution.rbegin(), solution.rend());
    lastSolveState = totalSolution.back();
  }
  std::reverse(totalSolution.begin(), totalSolution.end());
  printSolution(totalSolution, target);
}

}   // namespace key_lock_solver
