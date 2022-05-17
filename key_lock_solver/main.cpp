//  Copyright (c) 2022 Peter Aisher
//
//  main.cc
//  key_lock_solver
//

#include "search/AStarSolver.h"

int main(int argc, const char * argv[]) {
  auto solver = key_lock_solver::AStarSolver();
  solver.solve();
  return 0;
}
