#ifndef MEMENTO_SEARCHER_REQUEST_H
#define MEMENTO_SEARCHER_REQUEST_H

#include <string>
#include <set>
#include <utility>
#include <algorithm>
#include <vector>
#include <array>
#include <stdexcept>

#include <libpq-fe.h>

#include "trie.h"

#define MAX_RESULT_COUNTER 3

const char connInfo[] = "postgresql://hvarz@localhost?port=5432&dbname=mydb";

class SearcherRequest final {
public:
  using wordsClusterElement = std::pair<std::set<std::string>, std::string>;
  using clusterElement  = std::pair<std::string, std::string>;

public:
  SearcherRequest() = delete;
  ~SearcherRequest() = default;
  explicit SearcherRequest(const std::string& request, size_t pattern);

  void setRequest(const std::string& request);

  [[nodiscard]] auto getResult() const noexcept -> std::vector<std::string>;

private:
  std::string request_;
  size_t pattern_ = 0;
  Trie trie_;
  std::vector<wordsClusterElement> elements_;
  std::set<std::string> trieWords_;

private:
  [[nodiscard]] static auto getClusterSentences(PGconn *conn, int pattern)
      -> std::vector<clusterElement>;

  friend auto fixString(const std::string& string) -> std::string;
  friend auto parseString(const std::string& string) -> std::set<std::string>;
  friend auto eraseExtraSpaces(const std::string& string) -> std::string;
  friend auto toLowerCase(const std::string& string) noexcept -> std::string;
  void fillFields();
};

#endif // MEMENTO_SEARCHER_REQUEST_H
