//  Copyright (c) 2022 Peter Aisher
//
//  ProgressiveSolveHelper.cpp
//  key_lock_solver
//

#include "ProgressiveSolveHelper.h"
#include "../../key_lock_solver/search/Step.h"
#include "../../key_lock_solver/search/a_star.h"
#include "../../key_lock_solver/search/AStarSolver.h"

namespace key_lock_solver {
namespace impl {

static void printFailureMessage(const State &lastSolveState) {
  auto rc = lastSolveState.removedCount();
  std::cout << "Failed to find solution on step " << rc + 1 << std::endl;
  std::cout << "Removed " << rc << " pieces." << std::endl;
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

static void printSolution(std::vector<State> solution,
                          std::ostream& out = std::cout) {
  State lastState = solution.front();

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

ProgressiveSolveHelper::ProgressiveSolveHelper(AStarSolver* solver)
: solver(solver) {
  successorFunc = [&](State s){return this->solver->getPossibleMoves(s);};
  searchHeuristic = [&](State s){
    return static_cast<float>(s.minTotalDistanceFromOther(target));
  };
}

void ProgressiveSolveHelper::solve(State start) {
  internalSolve(start, State::solved(), false);
  printResults();
}

void ProgressiveSolveHelper::reset(State start) {
  internalSolve({}, start, true);
  printResults();
}

void ProgressiveSolveHelper::internalSolve(State start, State target,
                                           bool reverse) {
  performSetupForStartState(start);
  setTarget(target);
  reverseBeforePrinting = reverse;
  progressiveSolve();
}

void ProgressiveSolveHelper::performSetupForStartState(const State &start) {
  interimSolve = start;
  totalSolution = {start};
  solveFailed = false;
}

void ProgressiveSolveHelper::progressiveSolve() {
  for (size_t removalTarget = 0; removalTarget < PIECE_COUNT;
       ++removalTarget) {
    findPartialSolution(removalTarget);
    if (solveFailed) { return; }
    appendPartialSolutionToTotalSolution();
  }
}

void ProgressiveSolveHelper::findPartialSolution(size_t removalTarget) {
  auto goalFunc = goalFuncForStep(removalTarget);
  partialSolution =
    a_star<State>(interimSolve, goalFunc, successorFunc, searchHeuristic);
  solveFailed = partialSolution.empty();
}

ProgressiveSolveHelper::GoalFunc
ProgressiveSolveHelper::goalFuncForStep(size_t step) {
  return [&, step](State s){
    return (s.removedCount() > step) && s.removedPiecesAreSubsetOf(target);
  };
}

void ProgressiveSolveHelper::appendPartialSolutionToTotalSolution() {
  totalSolution.pop_back();
  totalSolution.insert(totalSolution.end(),
                       partialSolution.rbegin(), partialSolution.rend());
  interimSolve = totalSolution.back();
}

void ProgressiveSolveHelper::printResults() {
  if (solveFailed) {
    printFailureMessage(interimSolve);
  } else {
    outputSolution();
  }
}

void ProgressiveSolveHelper::outputSolution() {
  reverseSolutionIfNeeded();
  printSolution(totalSolution);
}

void ProgressiveSolveHelper::reverseSolutionIfNeeded() {
  if (reverseBeforePrinting) {
    std::reverse(totalSolution.begin(), totalSolution.end());
    reverseBeforePrinting = false;
  }
}

}   // namespace impl
}   // namespace key_lock_solver
