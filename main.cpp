#include "searcher_request.h"

using namespace std;


int main() {
  SearcherRequest request(L"are you winning son?", 1);
  auto vector = request.getResult();

  for (const auto& path : vector) {
    std::cout << path << std::endl;
  }

}