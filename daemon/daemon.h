#ifndef PROJECT_DAEMON_H
#define PROJECT_DAEMON_H

#include <iostream>
#include <iostream>
#include <libpq-fe.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

using std::string;

class Daemon {
public:
    Daemon();
    Daemon(PGconn *connection) = delete;
    ~Daemon() {
        PQfinish(conn);
    }
    std::pair<int, string> getPath();
    void recognize();

private:
    void removeRecord(const std::pair<int, string>& record);
    int getBiggestID();
    void insertRecord(int id, const string& path, int pattern , const string& text);
    bool isEmpty();
    int getTemplateOfPicture(const string& path);
    string getTextInPicture(const string& path);

    PGconn *conn;
};

#endif //PROJECT_DAEMON_H
