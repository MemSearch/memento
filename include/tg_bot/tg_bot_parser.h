#ifndef C_C_TG_BOT_PARSER_H
#define C_C_TG_BOT_PARSER_H

// Add telegram bot header

// C++ headers
#include <string>
#include <vector>
#include <utility>

struct TimeLastMem {
    TimeLastMem(const size_t hour_,
                const size_t seconds_,
                const size_t minutes) :
                hour(hour_),
                seconds(seconds_),
                minutes(minutes_) {}

    size_t hour;
    size_t minutes;
    size_t seconds;
};

class TgBotParser {
public:
    // Initialization, /start in bot, detach thread of work
    explicit TgBotParser(std::string apiKey) {}
    // Add new site if it's valid
    void AddSite() {}

private:
    bool isValid(const std::string& chatName) const { return false; }
    // Take first 100 memes and then latest of every day
    void Parse(const std::string& chatName) {}
    // Write to BD image's URL
    void WriteImgInDB(std::string imgURL) {}

    std::vector<std::pair<std::string, TimeLastMem>> chatsName;
};
#endif //C_C_TG_BOT_PARSER_H
