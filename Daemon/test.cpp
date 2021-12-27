#include <gtest/gtest.h>
#include <vector>

#include "daemon.h"

using std::string;
using std::ifstream;
using std::ofstream;

const char connInfo[] = "postgresql://ezury@localhost?port=5432&dbname=mydb";

TEST(Test1, getPath_isEmpty_removeRecord) {
    PGconn *conn = PQconnectdb(connInfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
    }

    string creating_table = "INSERT INTO new_table VALUES(1, 'path/pic1');"
                            "INSERT INTO new_table VALUES(2, 'path/pic2');"
                            "INSERT INTO new_table VALUES(3, 'path/pic3');";

    PGresult *res = PQexec(conn, creating_table.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Create table or insert into table failed: " << PQresultErrorMessage(res) << "\n";
    }
    PQclear(res);
    PQfinish(conn);

    TextRecognizer daemon(connInfo);

    EXPECT_FALSE(daemon.isEmpty());

    auto lastURL = daemon.getPath();
    daemon.removeRecord(lastURL);
    EXPECT_EQ(lastURL.first, 3);
    EXPECT_EQ(lastURL.second, "path/pic3");
    lastURL = daemon.getPath();
    daemon.removeRecord(lastURL);
    EXPECT_EQ(lastURL.first, 2);
    EXPECT_EQ(lastURL.second, "path/pic2");
    lastURL = daemon.getPath();
    daemon.removeRecord(lastURL);
    EXPECT_EQ(lastURL.first, 1);
    EXPECT_EQ(lastURL.second, "path/pic1");

    EXPECT_TRUE(daemon.isEmpty());
}

TEST(Test2, insertRecord) {
    std::vector<string> paths = {"path/pic1", "path/pic2", "path/pic3"};
    std::vector<int> templates = {1, 2, 3};
    std::vector<string> texts = {"Text1", "Text2", "Text3"};

    TextRecognizer daemon(connInfo);
    for (size_t i = 0; i < 3; ++i) {
        int id = daemon.getBiggestID() + 1;
        daemon.insertRecord(id, paths[i], templates[i], texts[i]);
    }

    PGconn *conn = PQconnectdb(connInfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
    }

    string getLastRecord = "SELECT * FROM parsed ORDER BY id DESC limit 1;";

    for (int i = 0; i < 3; ++i) {
        PGresult *res = PQexec(conn, getLastRecord.c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
        } else if (PQntuples(res) != 0) {
            char buffer[100];
            strncpy(buffer, PQgetvalue(res, 0, 1), 100);
            string path = string(buffer);
            EXPECT_EQ(path, paths[i]);
            strncpy(buffer, PQgetvalue(res, 0, 2), 100);
            EXPECT_EQ(std::stoi(string(buffer)), templates[i]);
            strncpy(buffer, PQgetvalue(res, 0, 3), 100);
            string text = string(buffer);
            EXPECT_EQ(text, texts[i]);
            PQclear(res);
            string deleteCommand = "delete FROM parsed WHERE path = '" + path + "';";
            res = PQexec(conn, deleteCommand.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cout << "Delete from table failed: " << PQresultErrorMessage(res) << "\n";
            }
            PQclear(res);
        }
    }
    PQfinish(conn);
}

//TEST(Test3, recognize) {
//    string pFilename = "test_parsed3.db";
//    string rFilename = "test_recognized3.db";
//
//    string url1 = "https://yt3.ggpht.com/a/AATXAJxwcptIJcRI5EsGZV14XqPRqTMvm9chvuKTHeD8=s900-c-k-c0xffffffff-no-rj-mo";
//    string url2 = "https://esourceengine.com/downloads/images/NPC-face-1.png";
//    string url3 = "https://www.meme-arsenal.com/memes/e430b559c6bcf654d1bd9bf15df40212.jpg";
//    string url4 = "https://images11.esquire.ru/upload/custom/80c/80c6011ff40cb5a80cb8f6b938b22d41.jpeg"; //yes.
//
//    sqlite3* pdb = nullptr;
//    sqlite3_open(pFilename.c_str(), &pdb);
//
//    string pCommand = "CREATE TABLE IF NOT EXISTS parsed (URL TEXT);"
//                            "INSERT INTO parsed VALUES('" + url1 + "');"
//                            "INSERT INTO parsed VALUES('" + url2 + "');"
//                            "INSERT INTO parsed VALUES('" + url3 + "');"
//                            "INSERT INTO parsed VALUES('" + url4 + "');";
//
//    char* error_parsed;
//    int rc = sqlite3_exec(pdb, pCommand.c_str(), nullptr, nullptr, &error_parsed);
//
//    if (rc != SQLITE_OK) {
//        cerr << "error: " << sqlite3_errmsg(pdb);
//    }
//    sqlite3_free(error_parsed);
//    sqlite3_close(pdb);
//
//    TextRecognizer daemon(pFilename, rFilename);
//    daemon.recognize();
//
//    std::vector<string> urls = {url1, url2, url3, url4};
//    std::vector<int> templates = {1, 2, 3, 3};
//    std::vector<string> texts = {"", "", "", "Yes."};
//
//    sqlite3* rdb = nullptr;
//    sqlite3_open(rFilename.c_str(), &rdb);
//
//    sqlite3_stmt *stmt;
//
//    const char* rCommand = "SELECT * FROM recognized;";
//    rc = sqlite3_prepare_v2(rdb, rCommand, -1, &stmt, nullptr);
//    if (rc != SQLITE_OK) {
//        cerr << "error: " << sqlite3_errmsg(rdb);
//    }
//    size_t i = 0;
//    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
//        if (i >= urls.size()) {
//            cerr << "sizes are not equal\n";
//            break;
//        }
//        const char *url = (char*)sqlite3_column_text(stmt, 0);
//        int picTemplate = sqlite3_column_int(stmt, 1);
//        const char *picText = (char*)sqlite3_column_text(stmt, 2);
//        EXPECT_EQ(url, urls[i]);
//        EXPECT_EQ(picTemplate, templates[i]);
//        EXPECT_EQ(picText, texts[i]);
//        ++i;
//    }
//
//    if (rc != SQLITE_DONE) {
//        cerr << "error: " << sqlite3_errmsg(rdb);
//    }
//    sqlite3_finalize(stmt);
//    sqlite3_close(rdb);
//}
