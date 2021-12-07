#include "searcher_request.h"
#include "trie.h"


auto eraseExtraSpaces(const std::wstring &string) -> std::wstring {
  auto result = string;
  result.erase(std::unique(result.begin(), result.end(),
                           [](wchar_t l, wchar_t r) {
                             return std::isspace(l) && l == r;
                           }),
               result.end());
  return result;
}

auto fixString(const std::wstring &string) -> std::wstring {
  std::wstring result;
  bool isBan = false;
  for (auto& symbol : string) {
    isBan = false;
    for (const auto& banSymbol : SearcherRequest::forbiddenSymbols) {
      if (symbol == banSymbol) {
        isBan = true;
        break;
      }
    }
    if (!isBan) {
      result.push_back(symbol);
    }
  }

  return eraseExtraSpaces(result);
}


auto parseString(const std::wstring &string) -> std::set<std::wstring> {
  std::set<std::wstring> result;
  std::wstring word;
  auto resultString = fixString(string);

  auto beginIter = resultString.begin();
  auto spaceIter = resultString.begin();

  while (spaceIter != resultString.end()) {
    spaceIter = std::find(beginIter, resultString.end(), ' ');
    word = {beginIter, spaceIter};
    if (!word.empty()) {
      result.insert(std::wstring(beginIter, spaceIter));
    }
    if (spaceIter != string.end()) {
      beginIter = std::next(spaceIter);
    }
  }

  return result;
}

SearcherRequest::SearcherRequest(const std::wstring &request) {
  auto treeElements = parseString(request);

  for (const auto& element : treeElements) {
    trie_.add(element);
  }
}

auto charToWString(const char *text) -> std::wstring {
  const size_t size = std::strlen(text);
  std::wstring wstr;
  if (size > 0) {
    wstr.resize(size);
    std::mbstowcs(&wstr[0], text, size);
  }
  return wstr;
}

auto SearcherRequest::getClusterSentences(PGconn *conn,
                                          int pattern) const noexcept
    -> std::vector<clusterElement> {
  std::vector<clusterElement> result;
  std::string command = "SELECT text, path FROM parsed WHERE template = " +
                    std::to_string(pattern) + ";";

  PGresult *res = PQexec(conn, command.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
  } else if (PQntuples(res) != 0) {
    char buffer[100];
    strncpy(buffer, PQgetvalue(res, 0, 0), 100);
    std::wstring text = charToWString(buffer);
    strncpy(buffer, PQgetvalue(res, 0, 1), 100);
    std::string path = std::string(buffer);
    result.emplace_back(text, path);
  }
  PQclear(res);
  return result;
}
