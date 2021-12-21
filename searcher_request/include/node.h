#ifndef MEMENTO_NODE_H
#define MEMENTO_NODE_H

#include <unordered_map>

struct Node {
  std::unordered_map<char, Node *> children_;
  bool isEnd_ = false;
  Node() : isEnd_(false) {}
  explicit Node(bool isEnd) noexcept : isEnd_(isEnd) {}
};

#endif // MEMENTO_NODE_H
