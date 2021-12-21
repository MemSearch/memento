#include "searcher_request.h"

#include "trie.h"


auto eraseExtraSpaces(const std::string &string) -> std::string {
  auto result = string;
  result.erase(std::unique(result.begin(), result.end(),
                           [](char l, char r) {
                             return std::isspace(l) && l == r;
                           }),
               result.end());
  return result;
}

auto toLowerCase(const std::string &string) noexcept -> std::string {
  std::string result;
  for (const auto& symbol : string) {
    result += std::tolower(symbol);
  }
  return result;
}

auto isLetter(const char symbol) noexcept -> bool {
  return (symbol >= 65 && symbol <= 90) || (symbol >= 97 && symbol <= 122)
         || (symbol >= -64 && symbol <= -1);
}

auto fixString(const std::string &string) -> std::string {
  std::string result;
  std::for_each(string.begin(), string.end(), [&result](char symbol) {
    if (!isLetter(symbol)) {
      result.push_back(' ');
    } else {
      result.push_back(symbol);
    }
  });
  return eraseExtraSpaces(result);
}


auto parseString(const std::string &string) -> std::set<std::string> {
  std::set<std::string> result;
  std::string word;
  auto resultString = fixString(string);

  auto beginIter = resultString.begin();
  auto spaceIter = resultString.begin();

  while (spaceIter != resultString.end()) {
    spaceIter = std::find(beginIter, resultString.end(), ' ');
    word = {beginIter, spaceIter};
    if (!word.empty()) {
      result.insert(std::string(beginIter, spaceIter));
    }
    if (spaceIter != string.end()) {
      beginIter = std::next(spaceIter);
    }
  }

  return result;
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
    std::string sentence;
    std::string path;
    for (size_t i = 0; i < PQntuples(res); ++i) {
      sentence = toLowerCase(std::string(PQgetvalue(res, i, 0)));
      path = std::string(PQgetvalue(res, i, 1));
      result.emplace_back(sentence, path);
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

SearcherRequest::SearcherRequest(const std::string& request,
                                 const size_t pattern)
    : request_(request), pattern_(pattern) {
  fillFields();
}

void SearcherRequest::setRequest(const std::string &request) {
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
