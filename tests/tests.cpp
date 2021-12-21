// private headers
#include "unsorted_img_storage/img_url_db.h"
#include "tg_bot/tg_bot_parser.h"

// C++ headers
#include <string>
#include <vector>

// GTest
#include <gtest/gtest.h>

TEST(Test1, InsertCheck) {
    ImgUrlDB db;
    db.InsertImg("home/img/1.png");
    EXPECT_EQ(1, db.GetImgs().size());
    EXPECT_EQ(0, db.GetImgs().size());
    db.InsertImg("home/img/1.png");
    db.InsertImg("home/img/2.png");
    db.InsertImg("home/img/3.png");
    db.InsertImg("home/img/4.png");
    db.InsertImg("home/img/5.png");
    std::vector<std::string> imgs = db.GetImgs();
    EXPECT_EQ(imgs[0], "home/img/1.png");
    EXPECT_EQ(imgs[1], "home/img/2.png");
    EXPECT_EQ(imgs[2], "home/img/3.png");
    EXPECT_EQ(imgs[3], "home/img/4.png");
    EXPECT_EQ(imgs[4], "home/img/5.png");
}

TEST(Test2, AddSite) {
    TgBotParser bot("ApiKey");
    bot.AddSite("@NR");

    auto vec = bot.GetDB();

    EXPECT_NE(0, vec.size());
}