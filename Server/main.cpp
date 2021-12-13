#include "searcher_request.h"

using namespace std;


int main() {
  SearcherRequest request("cat", 1);
  auto vector = request.getResult();

  for (const auto& path : vector) {
    std::cout << path << std::endl;
  }

}