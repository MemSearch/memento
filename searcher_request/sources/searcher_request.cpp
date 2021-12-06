#include "searcher_request.h"
#include "trie.h"

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

  return result;
}

SearcherRequest::SearcherRequest(const std::wstring &request) {
  request_ = fixString(request);
  auto clusterWords = get
}
