// private headers
#include "tg_bot/tg_bot_parser.h"

void TgBotParser::startWorking(ImgDB& db) {
    // Start command
    bot.getEvents().onCommand("start", [this](const Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id, "Hi, i'm bot parser photo!");
    });

    downloadFile(db);

    // demon work
    try {
        // Switch back to getUpdates.
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

void TgBotParser::downloadFile(ImgDB& db) {
    // Checking the photo for validity
    bot.getEvents().onAnyMessage([this, &db](const Message::Ptr& message) {
        if (!message->photo.empty()) {
            // take file info about photo with max size!
            auto file_info = bot.getApi().getFile(message->photo[message->photo.size() - 1]->fileId);
            auto download = bot.getApi().downloadFile(file_info->filePath);

            // Additional check for errors
            std::ofstream out("../../Site/static/img/image_" + std::to_string(counter) + ".jpg", std::ios::binary);
            out << download;
            out.close();
            db.insertPath(counter, "../../Site/static/img/image_" + std::to_string(counter) + ".jpg");
            ++counter;
        }
    });
}