//  Copyright (c) 2022 Peter Aisher
//
//  main.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#include <iostream>
#include "search/AStarSolver.hpp"

int main(int argc, const char * argv[]) {
  auto solver = AStarSolver();
  solver.solve();
  return 0;
}
