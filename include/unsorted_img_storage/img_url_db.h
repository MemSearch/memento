#ifndef C_C_IMG_URL_DB_H
#define C_C_IMG_URL_DB_H

// C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <shared_mutex>

// SQLite3
#include <sqlite3.h>

enum Status {
    OK,
    ERROR
};

class ImgUrlDB {
public:
    // Initialize repo
    ImgUrlDB() : db(nullptr), err(nullptr) {
        sqlite3_open("DataBase.db", &db);
        if (CreateDataBase() == Status::ERROR) {
            std::cerr << "Table wasn't created" << std::endl;
        }
    }

    virtual ~ImgUrlDB() {
        sqlite3_close(db);
    }

    Status InsertImg(const std::string imgUrl) { return Status::ERROR; }
    std::vector<std::string> GetImgs() { return {};}

private:
    Status CreateDataBase() {}

    sqlite3* db;
    char* err;
    std::shared_mutex mutex_;
};

#endif //C_C_IMG_URL_DB_H
