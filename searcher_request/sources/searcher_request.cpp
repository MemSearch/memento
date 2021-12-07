#include "searcher_request.h"
#include "trie.h"


SearcherRequest::SearcherRequest(const std::wstring &request) {

}


auto eraseExtraSpaces(const std::wstring &string) -> std::wstring {
  auto result = string;
  result.erase(std::unique(string.begin(), string.end(),
[](const wchar_t l, const wchar_t r) { return std::isspace(l) && l == r; }),
               string.end());
  return result;
}

auto fixString(const std::wstring &string) -> std::wstring {
  std::wstring result = string;
  for (auto& symbol : string) {
    for (const auto& banSymbol : SearcherRequest::forbiddenSymbols) {
      if (symbol == banSymbol) {
        result.erase(symbol);
        break;
      }
    }
  }

  return eraseExtraSpaces(result);
}


auto parseString(const std::wstring &string) -> std::set<std::wstring> {
  std::set<std::wstring> result;
  auto resultString = fixString(string);

  auto beginIter = resultString.begin();
  auto spaceIter = resultString.begin();

  while (spaceIter != resultString.end()) {
    spaceIter = std::find(beginIter, resultString.end(), ' ');
    result.insert(std::wstring(beginIter, spaceIter));
    if (spaceIter != string.end()) {
      beginIter = std::next(spaceIter);
    }
  }
}