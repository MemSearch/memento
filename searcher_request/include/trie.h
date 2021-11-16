#ifndef MEMENTO_TRIE_H
#define MEMENTO_TRIE_H

#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <optional>

#include "node.h"

auto ToLowerCase(const std::wstring &string) noexcept -> std::wstring;

class Trie {
public:
  Trie();
  ~Trie();

  void Add(const std::wstring &string) noexcept;

  [[nodiscard]] auto Search(const std::wstring &string) const noexcept -> bool;

  [[maybe_unused]] auto GetCorrections(std::wstring &string) const noexcept
      -> std::optional<std::set<std::wstring>>;

private:
  Node *root_;


private:
  void RecursiveReplace(const std::wstring &string,
                        std::set<std::wstring> &corrections, Node *currentNode,
                        const std::wstring &correction,
                        const size_t counter = 0,
                        size_t errorCount = 0) const noexcept;

  void Clear(Node *node_p) noexcept;
};

#endif // MEMENTO_TRIE_H
