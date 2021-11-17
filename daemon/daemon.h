#ifndef PROJECT_DAEMON_H
#define PROJECT_DAEMON_H

#include <iostream>
#include "sqlite3.h"

using std::string;
using std::cout;
using std::endl;

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
    static int getTemplateOfPicture(const string& url);
    static string getTextInPicture(const string& url);
    void writeRecognitionResults(const string& url, const int& picTemplate, const string& text);

private:
    void initialize(const string& parsed, const string& recognized);

    sqlite3* parsedDB;
    sqlite3* recognizedDB;
};

int callbackEmpty(void* sum, int args, char** argv, char** azColName);

#endif //PROJECT_DAEMON_H
