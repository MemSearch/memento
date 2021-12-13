#include "searcher_request.h"

#include <utility>
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

auto toLowerCase(const std::wstring &string) noexcept -> std::wstring {
  thread_local std::locale loc{""};
  std::wstring result;
  for (const auto& symbol : string) {
    result += std::tolower(symbol, loc);
  }
  return result;
}

auto fixString(const std::wstring &string) -> std::wstring {
  std::wstring result;
  bool isBan;
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
                                          int pattern)
    -> std::vector<clusterElement> {
  std::vector<clusterElement> result;
  std::string command = "SELECT text, path FROM memes where cluster=" +
                    std::to_string(pattern) + ";";


  PGresult *res = PQexec(conn, command.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    throw std::runtime_error("Select failed: ");
  } else if (PQntuples(res) != 0) {
    char buffer[100];
    for (size_t i = 0; i < PQntuples(res); ++i) {
      strncpy(buffer, PQgetvalue(res, i, 0), 100);
      std::wstring text = charToWString(buffer);
      text = toLowerCase(text);
      strncpy(buffer, PQgetvalue(res, i, 1), 100);
      std::string path = std::string(buffer);
      result.emplace_back(text, path);
    }
  }
  PQclear(res);
  return result;
}

void SearcherRequest::fillFields() {
  auto *conn = PQconnectdb(connInfo);

  if (PQstatus(conn) != CONNECTION_OK) {
    PQfinish(conn);
    throw std::runtime_error("Connection to database failed");
  }

  auto sentences = getClusterSentences(conn, pattern_);
  elements_.reserve(sentences.size());
  for (const auto& sentence : sentences) {
    auto clusterWords =
        wordsClusterElement(parseString(sentence.first), sentence.second);
    for (const auto& word : clusterWords.first) {
      trie_.add(word);
    }
    elements_.emplace_back(parseString(sentence.first), sentence.second);
  }

  auto wordsRequest = parseString(request_);

  for (auto word : wordsRequest) {
    auto corrections = trie_.getCorrections(word);
    auto value = corrections.value();
    if (value.empty()) {
      continue;
    } else {
      for (const auto& correction : value) {
        trieWords_.insert(correction);
      }
    }
  }

  if (trieWords_.empty()) {
    throw std::runtime_error("not found");
  }
}

SearcherRequest::SearcherRequest(std::wstring request,
                                 const size_t pattern)
    : request_(std::move(request)), pattern_(pattern) {
  fillFields();
}

void SearcherRequest::setRequest(const std::wstring &request) {
  request_ = request;
  trie_.clearFull();
  elements_.clear();
  trieWords_.clear();
  fillFields();
}

auto SearcherRequest::getResult() const noexcept -> std::vector<std::string> {
  std::vector<std::string> result;
  std::vector<std::pair<size_t, size_t>> priority(elements_.size(), {0, 0});
  for (size_t i = 0; i < elements_.size(); ++i) {
    priority[i].first = i;
    for (const auto& word : trieWords_) {
      if (elements_[i].first.count(word) != 0) {
        priority[i].second++;
      }
    }
  }


  std::sort(priority.begin(), priority.end(),
        [](const std::pair<size_t, size_t> &lhs,
                 const std::pair<size_t, size_t> &rhs) {
          return lhs.second > rhs.second;
        });

  size_t resultCounter = 0;
  for (const auto& [index, prior] : priority) {
    if (prior != 0) {
      result.push_back(elements_[index].second);
      ++resultCounter;
      if (resultCounter == MAX_RESULT_COUNTER) {
        break;
      }
    }
  }
  return result;
}
