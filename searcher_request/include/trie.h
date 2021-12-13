#ifndef MEMENTO_TRIE_H
#define MEMENTO_TRIE_H

#pragma once

#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <optional>

#include "node.h"

auto ToLowerCase(const std::string &string) noexcept -> std::string;

class Trie {
public:
  Trie();
  ~Trie();

  void add(const std::string &string) noexcept;

  [[nodiscard]] auto search(const std::string &string) const noexcept -> bool;

  [[maybe_unused]] auto getCorrections(std::string &string) const noexcept
      -> std::optional<std::set<std::string>>;

  void clearFull() noexcept;

private:
  Node *root_;


private:
  void recursiveReplace(const std::string &string,
                        std::set<std::string> &corrections, Node *currentNode,
                        const std::string &correction,
                        const size_t counter = 0,
                        size_t errorCount = 0) const noexcept;

  void clear(Node *node_p) noexcept;
};

#endif // MEMENTO_TRIE_H
