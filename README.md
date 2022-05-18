# key_lock_solver
A solver for Stephan Baumegger's Key Lock puzzle

The class `AStarSolver` uses A* search to remove pieces one by one and solve the puzzle.

The `solve(_)` method starts from a given state and completes the solution until all pieces are removed

The `reset(_)` method starts from a given state and gives the steps required to return the puzzle to the original 'fresh-out-of-the-box' state.
