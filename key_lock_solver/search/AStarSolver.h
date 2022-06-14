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
#include "../../key_lock_solver/search/State.h"

namespace key_lock_solver {

class AStarSolver {
  using CollisionCache = impl::CollisionCache;
  using Volume = impl::Volume;
  CollisionCache collisionCache;

  static const Volume box;

  static const std::array<Volume, PIECE_COUNT> movablePieces;

  friend class ProgressiveSolveHelper;

  /// Cascade a piece move to all pieces which would collide
  /// @param index the index of the piece to move.
  /// @param move the move to make.
  /// @param state a pointer to the current state. Modified by this method.
  /// @param moved whether each piece has been moved. Modified by this method
  /// @return the number of pieces moved in total, or zero on collision.
  size_t cascadeMove(size_t index, Vec3 move, State* state,
                     std::array<bool, PIECE_COUNT>* moved);
  bool boxCollision(size_t index, Vec3 offset);
  bool haveCollision(size_t firstIndex, size_t secondIndex,
                     Vec3 firstPosition, Vec3 secondPosition);

  std::vector<std::pair<State, float>> possibleMoves {};

  void clearPossibleMoves();
  void addMoveRemovingPiece(size_t i, const key_lock_solver::State &startState);
  bool skipOrRemovePiece(size_t i, const State& startState);
  void addCascadedMoveIfPossible(size_t i, const Vec3& move,
                                 const State &start);
  void addPossibleMovesForPiece(size_t i, const State& start);
  void populatePossibleMoves(State state);

  std::vector<std::pair<State, float>> getPossibleMoves(State state);

 public:
  /// Solve the puzzle to remove all pieces
  /// @param state the starting state to solve from (optional).
  void solve(State state = {});
  /// Reset the puzzle to reinsert all pieces
  /// @param state the starting state to solve from (optional).
  void reset(State state = State::solved());
};

}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_ASTARSOLVER_H_
