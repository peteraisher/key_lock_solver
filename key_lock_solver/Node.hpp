//
//  Node.hpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#ifndef Node_hpp
#define Node_hpp

#include <functional>
#include <memory>
#include "hash_combine.h"

template <class T>
struct Node {
  T state;
  std::shared_ptr<Node> parent;
  float cost = 0;
  float heuristic = 0;
  bool operator==(const Node<T> &other) const {
    return state == other.state
      && parent == other.parent
      && cost == other.cost
      && heuristic == other.heuristic;
  }
  bool operator<(const Node<T>& other) const {
    return cost + heuristic < other.cost + other.heuristic;
  }
  bool operator>(const Node<T>& other) const {
    return cost + heuristic > other.cost + other.heuristic;
  }
  Node(T state, std::shared_ptr<Node> parent, float cost, float heuristic)
  : state(state), parent(parent), cost(cost), heuristic(heuristic) {}
};

namespace std {
template <class T> struct hash<Node<T>> {
  size_t operator()(const Node<T> & x) const {
    size_t seed = hash<T>()(x.state);
    hash_combine(seed, x.parent);
    hash_combine(seed, x.cost);
    hash_combine(seed, x.heuristic);
    return seed;
  }
};
}

#endif /* Node_hpp */
