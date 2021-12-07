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
  using wordsClusterElement = std::pair<std::set<std::wstring>, std::string>;
  using clusterElement  = std::pair<std::wstring, std::string>;

public:
  SearcherRequest() = default;
  ~SearcherRequest() = default;
  explicit SearcherRequest(std::wstring  request, size_t pattern);

  void setRequest(const std::wstring& request);

  [[nodiscard]] auto getResult() const noexcept -> std::vector<std::string>;

private:
  std::wstring request_;
  size_t pattern_ = 0;
  Trie trie_;
  std::vector<wordsClusterElement> elements_;
  std::set<std::wstring> trieWords_;

  static constexpr std::array<wchar_t, 29> forbiddenSymbols{
      L',', L';', L'?', L'!', L':', L'"', L'|', L'\\', L'.', L'/',
      L'(', L')', L'@', L'#', L'$', L'%', L'^', L'&',  L'*', L'1',
      L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',  L'\n'};

private:
  [[nodiscard]] static auto getClusterSentences(PGconn *conn, int pattern)
      -> std::vector<clusterElement>;

  friend auto fixString(const std::wstring& string) -> std::wstring;
  friend auto parseString(const std::wstring& string) -> std::set<std::wstring>;
  friend auto eraseExtraSpaces(const std::wstring& string) -> std::wstring;
  friend auto charToWString(const char* text) -> std::wstring;
  void fillFields();
};

#endif // MEMENTO_SEARCHER_REQUEST_H
