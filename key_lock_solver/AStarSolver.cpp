//
//  AStarSolver.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 13.05.2022.
//

#include <algorithm>
#include "AStarSolver.hpp"
#include "a_star.hpp"
#include <iostream>

size_t AStarSolver::cascadeMove(size_t index, Vec3 move,
                              KeyLockPuzzleState &state, std::array<bool, PIECE_COUNT>& moved) {
  const auto startPos = state.positions[index];
  const auto newPos = startPos + move;
  if (boxCollision(index, newPos)) {
    return 0;
  }
  state.positions[index] = newPos;
  moved[index] = true;

  size_t tot = 0;
  for (size_t i = 0; i < PIECE_COUNT; ++i) {
    if (index != i && !state.isRemovedPiece(i)
        && haveCollision(index, i, newPos, state.positions[i])) {
      if (moved[i]) { return 0; }
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
    return cv == CollisionCache::Collision;
  }
  bool result = movablePieces[index].pointsIntersect(box, offset);
  cv = result ? CollisionCache::Collision : CollisionCache::NoCollision;
  collisionCache.setBoxCacheValue(index, offset, cv);
  return result;
}

bool AStarSolver::haveCollision(size_t firstIndex, size_t secondIndex, Vec3 firstPosition, Vec3 secondPosition) {
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
  CollisionCache::CacheValue cv = collisionCache.cacheValue(firstIndex, secondIndex, offset);
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

static inline float resetHeuristic(KeyLockPuzzleState state, KeyLockPuzzleState target) {
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

struct Step {
  std::array<bool, PIECE_COUNT> piece_flags = {};
  Vec3 diff = {};
  static constexpr std::array<char, PIECE_COUNT + 1> names =
    { "ABCDEFGHK" };
  inline void setFlag(size_t i) {piece_flags[i] = true;}
  inline void removeCommonPieces(Step& other) {
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (piece_flags[i] && other.piece_flags[i]) {
        piece_flags[i] = false;
        other.piece_flags[i] = false;
      }
    }
  }
  inline bool hasPieces() const {
    return std::any_of(piece_flags.begin(),
                       piece_flags.end(),
                       [](bool v){return v;});
  }
  inline bool hasMove() const {
    return !simd_equal(diff, Vec3{});
  }
  inline operator bool() const {
    return hasMove() || hasPieces();
  }
  inline bool operator==(const Step& other) const {
    if (!simd_equal(diff, other.diff)) {return false;}
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (piece_flags[i] != other.piece_flags[i]) {
        return false;
      }
    }
    return true;
  }
  inline Step() {}
  inline Step(const KeyLockPuzzleState& prev,
              const KeyLockPuzzleState& curr) {
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (!simd_equal(prev.positions[i], curr.positions[i])) {
        if (!prev.isRemovedPiece(i) && !curr.isRemovedPiece(i)) {
          diff = curr.positions[i] - prev.positions[i];
        }
        setFlag(i);
      }
    }
  }
  inline void printPieceNameList(std::ostream& out = std::cout) const {
    size_t printed_count = 0;
    for (size_t i = 0; i < PIECE_COUNT; ++i) {
      if (piece_flags[i]) {
        if (printed_count) {
          out << ", ";
        }
        out << names[i];
        ++printed_count;
      }
    }
  }
  inline void printCoordinateDirection(std::ostream& out = std::cout) const {
    char d = 'x';
    char s = '+';
    char v = 0;
    if (diff.x) {
      v = diff.x;
    } else if (diff.y) {
      v = diff.y;
      d = 'y';
    } else if (diff.z) {
      v = diff.z;
      d = 'z';
    }
    if (v < 0) {
      s = '-';
      v = -v;
    }
    out << s << d << " dir";
    if (v > 1) {
      out << ' ' << int(v) << " units";
    }
  }
  inline void printInstruction(std::ostream& out = std::cout) const {
    if (!simd_equal(diff, {})) {
      out << "move ";
      printPieceNameList(out);
      out << " in ";
      printCoordinateDirection(out);
      out << /* " by (" << int(diff.x) << ", " << int(diff.y)
        << ", " << int(diff.z) << ")" <<*/ std::endl;
    } else if (hasPieces()) {
      out << "remove ";
      printPieceNameList(out);
      out << std::endl;
    }
  }
};

static void consolidateStepsInSameDirection(std::vector<Step>& steps) {
  auto consolidatedStep = steps.begin();
  auto readStep = steps.begin();
  auto step = steps.begin();
  const auto end = steps.end();
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
  steps.resize(consolidatedStep - steps.begin());
}

static void removeEmptySteps(std::vector<Step>& steps) {
  auto writeStep = steps.begin();
  auto readStep = steps.begin();
  const auto end = steps.end();
  while (readStep < end) {
    if (*readStep) {
      *writeStep = *readStep;
      ++writeStep;
    }
    ++readStep;
  }
  steps.resize(writeStep - steps.begin());
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
      steps[i].removeCommonPieces(steps[j]);
    }
  }
  removeEmptySteps(steps);
  consolidateStepsInSameDirection(steps);
  for (const auto& step : steps) {
    step.printInstruction(out);
  }
}

std::vector<std::pair<KeyLockPuzzleState, float>> AStarSolver::possibleMoves(KeyLockPuzzleState state) {
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
      size_t res = cascadeMove(i, move, stateCopy, moved);
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
    a_star<KeyLockPuzzleState>(lastSolveState, [&i](KeyLockPuzzleState s){return s.removedCount() > i;}, [&](KeyLockPuzzleState s){return possibleMoves(s);}, searchHeuristic);
    if (solution.empty()) {
      auto rc = lastSolveState.removedCount();
      std::cout << "Failed to find solution on step " << i + 1 << std::endl;
      std::cout << "Removed " << rc << " pieces." << std::endl;
      return;
    }
    totalSolution.pop_back();
    totalSolution.insert(totalSolution.end(), solution.rbegin(), solution.rend()); // append reversed solution to end
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
    a_star<KeyLockPuzzleState>(lastSolveState, [&](KeyLockPuzzleState s){
      if (s.removedCount() < i) { return false;}
      for (size_t j = 0; j < PIECE_COUNT; ++j) {
        if (s.isRemovedPiece(j) && !target.isRemovedPiece(j)) {return false;}
      }
      return s.removedCount() > i || s == target;
    }, [&](KeyLockPuzzleState s){return possibleMoves(s);}, [&](KeyLockPuzzleState s){return resetHeuristic(s, target);});
    if (solution.empty()) {
      auto rc = lastSolveState.removedCount();
      std::cout << "Failed to find solution on step " << i + 1 << std::endl;
      std::cout << "Removed " << rc << " pieces." << std::endl;
      return;
    }
    totalSolution.pop_back();
    totalSolution.insert(totalSolution.end(), solution.rbegin(), solution.rend()); // append reversed solution to end
    lastSolveState = totalSolution.back();
  }
  std::reverse(totalSolution.begin(), totalSolution.end());
  printSolution(totalSolution, target);
}
