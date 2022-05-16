//
//  main.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#include <iostream>
#include "AStarSolver.hpp"

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";

  auto solver = AStarSolver();
  KeyLockPuzzleState start {{
    Vec3{0, 0, 0}, // A
    {0, 0, 0}, // B
    {0, 0, 0}, // C
    {0, 0, 0}, // D
    {0, 0, 0}, // E
    {0, 0, 0}, // F
    {0, 0, 0}, // G
    {0, 0, 0}, // H
    {0, 0, 0}
  }};

  solver.reset(start);
//  solver.solve(start);
//  solver.solve();
  return 0;
}
