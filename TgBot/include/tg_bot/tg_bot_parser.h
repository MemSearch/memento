#ifndef C_C_TG_BOT_PARSER_H
#define C_C_TG_BOT_PARSER_H

// private headers
#include <unsorted_img_storage/img_url_db.h>

// C++ headers
#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <exception>

// tgbot-cpp
#include <tgbot/tgbot.h>
#include <tgbot/Api.h>

const std::string BOT_TOKEN = "5047232560:AAE6OITFF42DWkgh6Z3sGwM_DY_MTR4iV3M";

using namespace TgBot;

class TgBotParser {
public:
    // Initialization, /start in bot, detach thread of work
    explicit TgBotParser(std::string bot_token) : bot(bot_token),
                                                  counter(0) {}
    ~TgBotParser() = default;

    void startWorking(ImgDB& db);

private:
    void downloadFile();

    TgBot::Bot bot;
    uint64_t counter;
};
#endif //C_C_TG_BOT_PARSER_H
