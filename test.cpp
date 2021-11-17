#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "daemon.h"

using std::string;
using std::ifstream;
using std::ofstream;

TEST(Test1, getURL) {
    sqlite3* pdb = nullptr;
    string pFilename = "test_parsed1.db";
    string rFilename = "test_recognized1.db";
    sqlite3_open(pFilename.c_str(), &pdb);

    string creating_table = "CREATE TABLE IF NOT EXISTS parsed (URL TEXT);"
                            "INSERT INTO parsed VALUES('url/pic1');"
                            "INSERT INTO parsed VALUES('url/pic2');"
                            "INSERT INTO parsed VALUES('url/pic3');";

    char* error_parsed;
    int rc = sqlite3_exec(pdb, creating_table.c_str(), nullptr, nullptr, &error_parsed);

    if (rc != SQLITE_OK) {
        cerr << "error: " << sqlite3_errmsg(pdb);
    }
    sqlite3_free(error_parsed);

    Daemon daemon(pFilename, rFilename);

    string lastURL = daemon.getURL();
    EXPECT_EQ(lastURL, "url/pic3");
    lastURL = daemon.getURL();
    EXPECT_EQ(lastURL, "url/pic2");
    lastURL = daemon.getURL();
    EXPECT_EQ(lastURL, "url/pic1");

    // проверяем что БД теперь пуста
    int count = 0;
    char* err = nullptr;
    std::string command = "SELECT COUNT(*) FROM parsed;";
    sqlite3_exec(pdb, command.c_str(), callbackEmpty, &count, &err);
    sqlite3_close(pdb);
    EXPECT_EQ(count, 0);
}

TEST(Test2, writeRecognitionResults) {
    string pFilename = "test_parsed2.db";
    string rFilename = "test_recognized2.db";

    std::vector<string> urls = {"url/pic1", "url/pic2", "url/pic3"};
    std::vector<int> templates = {1, 2, 3};
    std::vector<string> texts = {"Text1", "Text2", "Text3"};

    Daemon daemon(pFilename, rFilename);
    daemon.writeRecognitionResults(urls[0], templates[0], texts[0]);
    daemon.writeRecognitionResults(urls[1], templates[1], texts[1]);
    daemon.writeRecognitionResults(urls[2], templates[2], texts[2]);

    sqlite3* rdb = nullptr;
    sqlite3_open(rFilename.c_str(), &rdb);

    sqlite3_stmt *stmt;
    const char *command = "SELECT * FROM recognized;";
    int rc = sqlite3_prepare_v2(rdb, command, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "error: " << sqlite3_errmsg(rdb);
    }
    size_t i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (i >= urls.size()) {
            cerr << "sizes are not equal\n";
            break;
        }
        const char *url = (char*)sqlite3_column_text(stmt, 0);
        int picTemplate = sqlite3_column_int(stmt, 1);
        const char *picText = (char*)sqlite3_column_text(stmt, 2);
        EXPECT_EQ(url, urls[i]);
        EXPECT_EQ(picTemplate, templates[i]);
        EXPECT_EQ(picText, texts[i]);
        ++i;
    }

    if (rc != SQLITE_DONE) {
        cerr << "error: " << sqlite3_errmsg(rdb);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(rdb);
}

TEST(Test3, recognize) {
    string pFilename = "test_parsed3.db";
    string rFilename = "test_recognized3.db";

    string url1 = "https://yt3.ggpht.com/a/AATXAJxwcptIJcRI5EsGZV14XqPRqTMvm9chvuKTHeD8=s900-c-k-c0xffffffff-no-rj-mo";
    string url2 = "https://esourceengine.com/downloads/images/NPC-face-1.png";
    string url3 = "https://www.meme-arsenal.com/memes/e430b559c6bcf654d1bd9bf15df40212.jpg";
    string url4 = "https://images11.esquire.ru/upload/custom/80c/80c6011ff40cb5a80cb8f6b938b22d41.jpeg"; //yes.

    sqlite3* pdb = nullptr;
    sqlite3_open(pFilename.c_str(), &pdb);

    string pCommand = "CREATE TABLE IF NOT EXISTS parsed (URL TEXT);"
                            "INSERT INTO parsed VALUES('" + url1 + "');"
                            "INSERT INTO parsed VALUES('" + url2 + "');"
                            "INSERT INTO parsed VALUES('" + url3 + "');"
                            "INSERT INTO parsed VALUES('" + url4 + "');";

    char* error_parsed;
    int rc = sqlite3_exec(pdb, pCommand.c_str(), nullptr, nullptr, &error_parsed);

    if (rc != SQLITE_OK) {
        cerr << "error: " << sqlite3_errmsg(pdb);
    }
    sqlite3_free(error_parsed);
    sqlite3_close(pdb);

    Daemon daemon(pFilename, rFilename);
    daemon.recognize();

    std::vector<string> urls = {url1, url2, url3, url4};
    std::vector<int> templates = {1, 2, 3, 3};
    std::vector<string> texts = {"", "", "", "Yes."};

    sqlite3* rdb = nullptr;
    sqlite3_open(rFilename.c_str(), &rdb);

    sqlite3_stmt *stmt;

    const char* rCommand = "SELECT * FROM recognized;";
    rc = sqlite3_prepare_v2(rdb, rCommand, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "error: " << sqlite3_errmsg(rdb);
    }
    size_t i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (i >= urls.size()) {
            cerr << "sizes are not equal\n";
            break;
        }
        const char *url = (char*)sqlite3_column_text(stmt, 0);
        int picTemplate = sqlite3_column_int(stmt, 1);
        const char *picText = (char*)sqlite3_column_text(stmt, 2);
        EXPECT_EQ(url, urls[i]);
        EXPECT_EQ(picTemplate, templates[i]);
        EXPECT_EQ(picText, texts[i]);
        ++i;
    }

    if (rc != SQLITE_DONE) {
        cerr << "error: " << sqlite3_errmsg(rdb);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(rdb);
}
