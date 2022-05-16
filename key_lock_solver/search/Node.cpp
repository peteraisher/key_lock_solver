//  Copyright (c) 2022 Peter Aisher
//
//  Node.cpp
//  key_lock_solver
//
//  Created by Peter Aisher on 12.05.2022.
//

#include "../../key_lock_solver/search/Node.hpp"

template <class T>
bool Node<T>::operator==(const Node<T> &other) const {
  return state == other.state
    && parent == other.parent
    && cost == other.cost
    && heuristic == other.heuristic;
}
