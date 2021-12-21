#ifndef PROJECT_DAEMON_H
#define PROJECT_DAEMON_H

#include <iostream>
#include <libpq-fe.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

using std::string;
using namespace cv;

class Daemon {
public:
    explicit Daemon(const string& connInfo);
    Daemon() = delete;
    ~Daemon() {
        PQfinish(conn);
    }
    std::pair<int, string> getPath();
    void removeRecord(const std::pair<int, string>& record);
    int getBiggestID();
    void insertRecord(int id, const string& path, int pattern , const string& text);
    bool isEmpty();
    void recognize();

private:
    int getTemplateOfPicture(const string& path);
    string getTextInPicture(const string& path);

    PGconn *conn;
};

#endif //PROJECT_DAEMON_H
