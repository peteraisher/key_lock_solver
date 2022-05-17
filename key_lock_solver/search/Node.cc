//  Copyright (c) 2022 Peter Aisher
//
//  Node.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/Node.h"

namespace key_lock_solver {

template <class T>
bool Node<T>::operator==(const Node<T> &other) const {
  return state == other.state
    && parent == other.parent
    && cost == other.cost
    && heuristic == other.heuristic;
}

}   // namespace key_lock_solver
