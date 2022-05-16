//  Copyright (c) 2022 Peter Aisher
//
//  Node.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef KEY_LOCK_SOLVER_SEARCH_NODE_HPP_
#define KEY_LOCK_SOLVER_SEARCH_NODE_HPP_

#include <functional>
#include <memory>
#include "../../key_lock_solver/util/hash_combine.h"

template <class T>
struct Node {
  T state;
  std::shared_ptr<Node> parent;
  float cost = 0;
  float heuristic = 0;
  bool operator==(const Node<T> &other) const;
  bool operator<(const Node<T>& other) const {
    return cost + heuristic < other.cost + other.heuristic;
  }
  bool operator>(const Node<T>& other) const {
    return cost + heuristic > other.cost + other.heuristic;
  }
  Node(T state, std::shared_ptr<Node> parent, float cost, float heuristic)
  : state(state), parent(parent), cost(cost), heuristic(heuristic) {}
};



#endif  // KEY_LOCK_SOLVER_SEARCH_NODE_HPP_
