#include "trie.h"

auto ToLowerCase(const std::wstring &string) noexcept -> std::wstring {
  thread_local std::locale loc{""};
  std::wstring result;
  for (const auto& symbol : string) {
    result += std::tolower(symbol, loc);
  }
  return result;
}

Trie::Trie() : root_(new Node) {}

Trie::~Trie() {
  clear(root_);
}

void Trie::add(const std::wstring &string) noexcept {
  if (string.empty()) {
    return;
  }
  auto tmp = root_;
  std::wstring str = ToLowerCase(string);
  size_t i = 0;
  for (const auto& symbol : str) {
    if (tmp->children_.count(symbol) == 0) {
      if (i == str.size() - 1) {
        tmp->children_[symbol] = new Node(true);
      } else {
        tmp->children_[symbol] = new Node(false);
      }
    }
    tmp = tmp->children_[symbol];
    if (i == str.size() - 1) {
      tmp->isEnd_ = true;
    }
    ++i;
  }
}

[[nodiscard]] auto Trie::search(const std::wstring &string) const noexcept ->
    bool {
  auto *node = root_;
  for (auto key_ : string) {
    if (node->children_.count(key_) != 0) {
      node = node->children_.at(key_);
    }
    else {
      return false;
    }
  }
  return node->isEnd_;
}

[[maybe_unused]] auto Trie::getCorrections(std::wstring &string) const noexcept
    -> std::optional<std::set<std::wstring>> {
  std::set<std::wstring> result;
  auto str = ToLowerCase(string);
  std::set<std::wstring> corrections;
  std::wstring correction;
  recursiveReplace(str, corrections, root_, correction);
  return std::make_optional(corrections);
}

void Trie::recursiveReplace(const std::wstring &string,
                      std::set<std::wstring> &corrections, Node *currentNode,
                      const std::wstring &correction,
                      const size_t counter, size_t errorCount) const noexcept {

  if (errorCount == 2) {
    return;
  } else if (errorCount == 1 && counter == string.size() &&
             currentNode->isEnd_) {
    corrections.insert(correction);
    return;
  } else if (errorCount == 0 && counter == string.size() - 1 &&
             currentNode->isEnd_) {
    corrections.insert(correction);
  } else if (errorCount == 0 && counter == string.size() && !currentNode->children_.empty()) {
    for (const auto& node : currentNode->children_) {
      if (node.second->isEnd_)
        corrections.insert(correction + node.first);
    }
    return;
  } else if (search(string)) {
    corrections.insert(string);
  }

  for (const auto& node : currentNode->children_) {
    if (node.first == string[counter]) {
      recursiveReplace(string, corrections, node.second,
                       correction + string[counter], counter + 1, errorCount);
    } else {
      if (string.size() >= counter + 1 && node.first == string[counter + 1]) {
        recursiveReplace(string, corrections, currentNode, correction,
                         counter + 1,errorCount + 1);
      }
      if (node.second->children_.count(string[counter]) != 0 && node.first == string[counter + 1]) {
        recursiveReplace(string, corrections, node.second->children_.at
                                              (string[counter]),
                         correction + node.first + string[counter],
                         counter + 2, errorCount + 1);
      }
      if (node.second->children_.count(string[counter]) != 0)
        recursiveReplace(string, corrections, node.second,
                         correction + node.first, counter, errorCount + 1);

      if (node.first != string[counter]) {
        recursiveReplace(string, corrections, node.second,
                         correction + node.first, counter + 1,
                         errorCount + 1);
      }
    }
  }
}

void Trie::clear(Node *node_p) noexcept {
  if (node_p == nullptr) {
    return;
  }
  for (auto node : node_p->children_) {
    clear(node.second);
  }
  delete node_p;
}

void Trie::clearFull() noexcept {
  clear(root_);
}

