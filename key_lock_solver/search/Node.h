//  Copyright (c) 2022 Peter Aisher
//
//  Node.h
//  key_lock_solver
//

#ifndef KEY_LOCK_SOLVER_SEARCH_NODE_H_
#define KEY_LOCK_SOLVER_SEARCH_NODE_H_

#include <functional>
#include <memory>
#include "../../key_lock_solver/util/hash_combine.h"

namespace key_lock_solver {

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


}   // namespace key_lock_solver

#endif  // KEY_LOCK_SOLVER_SEARCH_NODE_H_
