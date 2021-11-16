#include "daemon.h"

Daemon::Daemon() : parsedDB(nullptr), recognizedDB(nullptr) {
    initialize("parsed.db", "recognized.db");
}

Daemon::Daemon(const string& parsed, const string& recognized) : parsedDB(nullptr), recognizedDB(nullptr) {
    initialize(parsed, recognized);
}

void Daemon::initialize(const string& parsed, const string& recognized) {
    sqlite3_open(parsed.c_str(), &parsedDB);

    string creating_table = "CREATE TABLE IF NOT EXISTS parsed(URL TEXT);";

    char* error_parsed;
    int rc = sqlite3_exec(parsedDB, creating_table.c_str(), nullptr, nullptr, &error_parsed);

    if (rc != SQLITE_OK) {
        std::cerr << "error: " << sqlite3_errmsg(parsedDB);
    }
    sqlite3_free(error_parsed);


    sqlite3_open(recognized.c_str(), &recognizedDB);

    creating_table = "CREATE TABLE IF NOT EXISTS recognized(URL TEXT, Template INTEGER, Text TEXT);";

    char* error_recognized;
    rc = sqlite3_exec(recognizedDB, creating_table.c_str(), nullptr, nullptr, &error_recognized);

    if (rc != SQLITE_OK) {
        std::cerr << "error: " << sqlite3_errmsg(recognizedDB);
    }
    sqlite3_free(error_recognized);
}

string Daemon::getURL() {
    // извлекаем запись из parsedDB
    return "";
}

void Daemon::recognize() {
    // извлекаем запись из parsedDB
    string url = getURL();
    // определяем шаблон и текст картинки
    int picTemplate = getTemplateOfPicture(url);
    string picText = getTextInPicture(url);
    // записываем результат в recognizedDB
    writeRecognitionResults(url, picTemplate, picText);
}

int Daemon::getTemplateOfPicture(const string& url) {
    // определяем шаблон картинки
    return 1;
}

string Daemon::getTextInPicture(const string& url) {
    // определяем текст картинки
    return "";
}

void Daemon::writeRecognitionResults(const string& url, const int& pitTemplate, const string& text) {
    // записываем результат в recognizedDB
}

void Daemon::work() {
    // пока parsedDB не пуста, будем распознавать каждую ссылку
    recognize();
}

int callbackEmpty(void* sum, int args, char** argv, char** azColName) {
    std::string str = argv[0];
    int *intPtr = static_cast<int*>(sum);
    *intPtr = std::stoi(str);
    return 0;
}