#ifndef C_C_TG_BOT_PARSER_H
#define C_C_TG_BOT_PARSER_H

// private headers
#include <unsorted_img_storage/img_url_db.h>

// C++ headers
#include <string>
#include <vector>
#include <utility>

// tgbot-cpp
#include <tgbot/tgbot.h>

struct TimeLastMem {
    TimeLastMem(const size_t hour_,
                const size_t seconds_,
                const size_t minutes) :
                hour(hour_),
                minutes(minutes_),
                seconds(seconds_) {}

    size_t hour;
    size_t minutes;
    size_t seconds;
};

class TgBotParser {
public:
    // Initialization, /start in bot, detach thread of work
    explicit TgBotParser(std::string apiKey) {}
    // Add new site if it's valid
    void AddSite(const std::string& chatName) {}

private:
    bool isValid(const std::string& chatName) const { return false; }
    // Take first 100 memes and then latest of every day
    void Parse(const std::string& chatName) {}
    // Write to BD image's URL
    void WriteImgInDB(std::string imgURL) {}

    std::vector<std::pair<std::string, TimeLastMem>> chatsName;
};
#endif //C_C_TG_BOT_PARSER_H
