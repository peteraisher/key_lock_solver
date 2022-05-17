//  Copyright (c) 2022 Peter Aisher
//
//  a_star.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_A_STAR_H_
#define KEY_LOCK_SOLVER_SEARCH_A_STAR_H_

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>
#include "../../key_lock_solver/search/Node.h"

namespace key_lock_solver {

template <class T>
std::vector<T> a_star(
        T initialState,
        std::function<bool(T)> goalTestFunc,
        std::function<std::vector<std::pair<T, float>>(T)> successorFunc,
        std::function<float(T)> heuristicFunc);

}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_A_STAR_H_
