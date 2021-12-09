#include "gtest/gtest.h"
#include "memInfo.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(MemInfo, output_int_string) {
    MemInfo<> mem;
    mem.id = 12;
    mem.tags.push_back("woman");
    mem.tags.push_back("cat");
    std::ostringstream out;
    out << mem;
    EXPECT_EQ("[id: 12, tags: <woman>, <cat>]", out.str());
}

TEST(MemInfo, output_string_string) {
    MemInfo<std::string> mem;
    mem.id = "doublePicTemplate";
    mem.tags.push_back("woman");
    mem.tags.push_back("cat");
    std::ostringstream out;
    out << mem;
    EXPECT_EQ("[id: doublePicTemplate, tags: <woman>, <cat>]", out.str());
}

/*
TEST(MemInfo, getId) {
    int id = 1;
    std::vector<std::string> tags;
    MemInfo<> mem = MemInfo(1, tags);
    EXPECT_EQ(1, mem.getId());
}

TEST(MemInfo, getTags) {
    int id = 1;
    std::vector<std::string> tags;
    std::string str1 = "woman";
    std::string str2 = "cat";
    tags.push_back(str1);
    tags.push_back(str2);
    MemInfo<> mem = MemInfo(1, tags);
    for (auto tag : mem.getTags()) {
      EXPECT_EQ(str1, mem.getTags());
      EXPECT_EQ(str2, mem.getTags());
    }
}
*/
