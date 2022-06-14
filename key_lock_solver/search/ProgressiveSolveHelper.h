//  Copyright (c) 2022 Peter Aisher
//
//  ProgressiveSolveHelper.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_PROGRESSIVESOLVEHELPER_H_
#define KEY_LOCK_SOLVER_SEARCH_PROGRESSIVESOLVEHELPER_H_

#include <vector>
#include <utility>
#include <functional>
#include "../../key_lock_solver/search/State.h"

namespace key_lock_solver {

class AStarSolver;

namespace impl {

class ProgressiveSolveHelper {

 public:
  explicit ProgressiveSolveHelper(AStarSolver* solver);
  void solve(State start);
  void reset(State start);

private:
  template <class T>
  using SuccessorFunc = std::function<std::vector<std::pair<T, float>>(T)>;
  using GoalFunc = std::function<bool(State)>;

  AStarSolver* solver = nullptr;
  State interimSolve;
  State target;
  std::vector<State> partialSolution;
  std::vector<State> totalSolution;
  SuccessorFunc<State> successorFunc;
  std::function<float(State)> searchHeuristic;
  bool solveFailed = false;
  bool reverseBeforePrinting = false;

  void internalSolve(State start, State target, bool reverse);
  void performSetupForStartState(const State &start);
  inline void setTarget(State target) {this->target = target;}
  void progressiveSolve();
  void findPartialSolution(size_t removalTarget);
  GoalFunc goalFuncForStep(size_t step);
  void appendPartialSolutionToTotalSolution();
  void printResults();
  void outputSolution();
  void reverseSolutionIfNeeded();
};

}   // namespace impl
}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_PROGRESSIVESOLVEHELPER_H_
