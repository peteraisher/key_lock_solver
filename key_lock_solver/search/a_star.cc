//  Copyright (c) 2022 Peter Aisher
//
//  a_star.cc
//  key_lock_solver
//

#include "../../key_lock_solver/search/a_star.h"
#include "../../key_lock_solver/search/State.h"

namespace key_lock_solver {
namespace impl {

template <class T>
std::vector<T> backtrack(std::shared_ptr<Node<T>> goalNode) {
  std::vector<T> result {};
  std::shared_ptr<Node<T>> node = goalNode;
  while (node->parent) {
    result.push_back(node->state);
    node = node->parent;
  }
  result.push_back(node->state);
  return result;
}

struct PointeeCompareGreater {
  template <class T>
  bool operator()(const std::shared_ptr<T>& a,
                  const std::shared_ptr<T>& b) const {
    return *a > *b;
  }
};

struct PointeeCompareEqual {
  template <class T>
  bool operator()(const std::shared_ptr<T>& a,
                  const std::shared_ptr<T>& b) const {
    return *a == *b;
  }
};

struct PointeeHash {
  template <class T>
  size_t operator()(const std::shared_ptr<T>& x) {
    return std::hash<T>()(*x);
  }
};

template <class T>
using SuccessorFunc = std::function<std::vector<std::pair<T, float>>(T)>;

template <class T>
std::vector<T> a_star(
        T initialState,
        std::function<bool(T)> goalTestFunc,
        SuccessorFunc<T> successorFunc,
        std::function<float(T)> heuristicFunc) {
  std::priority_queue<std::shared_ptr<Node<T>>,
                      std::vector<std::shared_ptr<Node<T>>>,
                      PointeeCompareGreater> frontier;
  std::unordered_map<T, float> explored {};
  std::shared_ptr<Node<T>> start =
    std::make_shared<Node<T>>(initialState, nullptr, 0.f,
                              heuristicFunc(initialState));
  frontier.push(start);
  explored[initialState] = 0.f;

  while (!frontier.empty()) {
    std::shared_ptr<Node<T>> currentNode = frontier.top();
    frontier.pop();
    T currentState = currentNode->state;

    if (goalTestFunc(currentState)) {
      return backtrack(currentNode);
    }

    for (const auto& pair : successorFunc(currentState)) {
      const T& child = pair.first;
      float newCost = currentNode->cost + pair.second;
      const auto idx = explored.find(child);
      if (idx == explored.end() || idx->second > newCost) {
        explored[child] = newCost;
        std::shared_ptr<Node<T>> childNode =
        std::make_shared<Node<T>>(child, currentNode, newCost,
                                  heuristicFunc(child));
        frontier.push(childNode);
      }
    }
  }
  return {};
}

template
std::vector<State> a_star(
        State initialState,
        std::function<bool(State)> goalTestFunc,
        SuccessorFunc<State> successorFunc,
        std::function<float(State)> heuristicFunc);

}   // namespace impl
}   // namespace key_lock_solver
