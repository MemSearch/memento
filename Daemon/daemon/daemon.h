#ifndef PROJECT_DAEMON_H
#define PROJECT_DAEMON_H

#include <iostream>
#include <libpq-fe.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

class Daemon {
public:
    explicit Daemon(const std::string& connInfo);
    Daemon() = delete;
    ~Daemon() {}
    std::pair<int, std::string> getPath();
    void removeRecord(const std::pair<int, std::string>& record);
    int getBiggestID();
    void insertRecord(int id, const std::string& path, int pattern , const std::string& text);
    bool isEmpty();
    void recognize();

private:
    static std::string getTextInPicture(const std::string& path);

    std::unique_ptr<PGconn, void(*)(PGconn *)> conn;
};

#endif //PROJECT_DAEMON_H
