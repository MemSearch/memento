// img_storage
#include "unsorted_img_storage/img_url_db.h"

// bot
#include "tg_bot/tg_bot_parser.h"


int main() {
    ImgDB db;

    TgBotParser bot(BOT_TOKEN);
    bot.startWorking();
    return 0;
}


