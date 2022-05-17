//  Copyright (c) 2022 Peter Aisher
//
//  a_star.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_A_STAR_HPP_
#define KEY_LOCK_SOLVER_SEARCH_A_STAR_HPP_

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>
#include "../../key_lock_solver/search/Node.hpp"

template <class T>
std::vector<T> a_star(
        T initialState,
        std::function<bool(T)> goalTestFunc,
        std::function<std::vector<std::pair<T, float>>(T)> successorFunc,
        std::function<float(T)> heuristicFunc);

#endif  // KEY_LOCK_SOLVER_SEARCH_A_STAR_HPP_
