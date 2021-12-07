#ifndef MEMENTO_SEARCHER_REQUEST_H
#define MEMENTO_SEARCHER_REQUEST_H

#include <string>
#include <set>
#include <utility>
#include <algorithm>
#include <vector>
#include <array>

#include "trie.h"

class SearcherRequest final {
public:
  using clusterElement = std::pair<std::wstring, std::wstring>;

public:
  SearcherRequest() = default;
  explicit SearcherRequest(const std::wstring& request);

  void setRequest(const std::wstring& request);

  [[nodiscard]] auto getResult() const noexcept -> std::vector<std::wstring>;

private:
  std::wstring request_;
  Trie trie_;
  std::vector<clusterElement> elements_;
  static constexpr std::array<wchar_t, 29> forbiddenSymbols{
      L',', L';', L'?', L'!', L':', L'"', L'|', L'\\', L'.', L'/',
      L'(', L')', L'@', L'#', L'$', L'%', L'^', L'&',  L'*', L'1',
      L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',  L'\n'};

private:
  [[nodiscard]] auto getClusterSentences() const noexcept ->
      std::vector<clusterElement>;

  friend auto fixString(const std::wstring& string) -> std::wstring;
  friend auto parseString(const std::wstring& string) -> std::set<std::wstring>;
  friend auto eraseExtraSpaces(const std::wstring& string) -> std::wstring;
};

#endif // MEMENTO_SEARCHER_REQUEST_H
