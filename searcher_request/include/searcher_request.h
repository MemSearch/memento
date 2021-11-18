#ifndef MEMENTO_SEARCHER_REQUEST_H
#define MEMENTO_SEARCHER_REQUEST_H

#include <string>
#include <set>
#include <utility>

#include "trie.h"

using wordPopularity = std::pair<std::wstring, size_t>;

struct Compare {
  auto operator()(const wordPopularity &lhs,
                  const wordPopularity &rhs) const noexcept -> bool;
};

class SearcherRequest final {
public:
  SearcherRequest() = default;
  explicit SearcherRequest(const std::wstring& request);
  explicit SearcherRequest(std::wstring&& request);

  void setRequest(const std::wstring& request);
  void setRequest(std::wstring&& request);

private:
  std::wstring request_;
  Trie trie;

private:
  auto getPopularityWords() const noexcept -> std::set<wordPopularity>;
  [[nodiscard]] auto getSimilarWords() const noexcept ->
      std::set<std::wstring, Compare>;
};

#endif // MEMENTO_SEARCHER_REQUEST_H
