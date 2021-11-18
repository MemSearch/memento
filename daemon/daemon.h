#ifndef PROJECT_DAEMON_H
#define PROJECT_DAEMON_H

#include <iostream>
#include "sqlite3.h"

using std::string;
using std::cout;
using std::cerr;

class Daemon {
public:
    Daemon();
    Daemon(const string& parsed, const string& recognized);
    ~Daemon() {
        sqlite3_close(parsedDB);
        sqlite3_close(recognizedDB);
    }
    void work();
    static string getURL();
    void recognize();
    void writeRecognitionResults(const string& url, const int& picTemplate, const string& text);

private:
    void initialize(const string& parsed, const string& recognized);
    static int getTemplateOfPicture(const string& url);
    static string getTextInPicture(const string& url);

    sqlite3* parsedDB = nullptr;
    sqlite3* recognizedDB = nullptr;
};

int callbackEmpty(void* sum, int args, char** argv, char** azColName);

#endif //PROJECT_DAEMON_H
