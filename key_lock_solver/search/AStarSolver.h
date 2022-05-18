//  Copyright (c) 2022 Peter Aisher
//
//  AStarSolver.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_H_
#define KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_H_

#include <utility>
#include <vector>
#include <array>
#include "../../key_lock_solver/search/a_star.h"
#include "../../key_lock_solver/search/CollisionCache.h"
#include "../../key_lock_solver/spatial/Volume.h"
#include "../../key_lock_solver/search/KeyLockPuzzleState.h"

namespace key_lock_solver {

class AStarSolver {
  using CollisionCache = impl::CollisionCache;
  using Volume = impl::Volume;
  CollisionCache collisionCache;

  static const Volume box;

  static const std::array<Volume, PIECE_COUNT> movablePieces;

  size_t cascadeMove(size_t index, Vec3 move, KeyLockPuzzleState* state,
                     std::array<bool, PIECE_COUNT>* moved);
  bool boxCollision(size_t index, Vec3 offset);
  bool haveCollision(size_t firstIndex, size_t secondIndex,
                     Vec3 firstPosition, Vec3 secondPosition);
  std::vector<std::pair<KeyLockPuzzleState, float>>
  possibleMoves(KeyLockPuzzleState state);

 public:
  /// Solve the puzzle to remove all pieces
  /// @param state the starting state to solve from (optional).
  void solve(KeyLockPuzzleState state = {});
  /// Reset the puzzle to reinsert all pieces
  /// @param state the starting state to solve from (optional).
  void reset(KeyLockPuzzleState state = KeyLockPuzzleState::solved());
};

}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_H_
