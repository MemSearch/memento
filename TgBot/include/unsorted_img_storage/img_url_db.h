#ifndef C_C_IMG_URL_DB_H
#define C_C_IMG_URL_DB_H

// C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <thread>

// PostgreSQL
#include <libpq-fe.h>

class ImgDB {
public:
    ImgDB();
    ~ImgDB() { PQfinish(conn); }
    void insertPath(int id, const std::string& path);

private:
    PGconn* conn;
};

#endif //C_C_IMG_URL_DB_H
