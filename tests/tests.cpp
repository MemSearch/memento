#include <gtest/gtest.h>
#include <string>

#include "searcher_request.h"

TEST(Trie, ErrorFix) {
  Trie trie;
  std::wstring randoms = L"randoms";
  std::wstring fur = L"fur";
  trie.add(L"some");
  trie.add(L"random");
  trie.add(L"words");
  trie.add(L"for");
  trie.add(L"testing");
  trie.add(L"your");
  trie.add(L"solutions");
  trie.add(L"far");

  auto testFar = trie.getCorrections(fur);
  ASSERT_EQ(testFar.value().size(), 2);
  ASSERT_EQ(*testFar.value().begin(), L"far");
  ASSERT_EQ(*++testFar.value().begin(), L"for");

  auto testSet = trie.getCorrections(randoms);
  ASSERT_EQ(*testSet.value().begin(), randoms);
  ASSERT_EQ(testSet.value().size(), 1);
}

TEST(trie, SearchInTrie) {
  Trie trie;
  std::wstring randoms = L"randoms";
  trie.add(L"some");
  trie.add(L"random");
  trie.add(L"words");
  trie.add(L"for");
  trie.add(L"testing");
  trie.add(L"your");
  trie.add(L"solutions");
  trie.add(L"far");

  ASSERT_EQ(trie.search(randoms), false);
  ASSERT_EQ(trie.search(L"some"), true);
}

TEST(trie, ToLowerCase) {
  std::wstring capsWord = L"HOPE";
  std::wstring word = L"hope";
  ASSERT_EQ(ToLowerCase(capsWord), word);
}