#include "daemon.h"
#include <opencv2/opencv.hpp>

Daemon::Daemon(const std::string& connInfo) : conn(PQconnectdb(connInfo.c_str()), PQfinish) {

    if (PQstatus(conn.get()) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn.get()) << "\n";
        PQfinish(conn.get());
        throw std::invalid_argument("Connection to database failed");
    } else {
        std::cout << "Connection to database succeed." << std::endl;
    }
}

static void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    exit(signum);
}

std::pair<int, std::string> Daemon::getPath() {
    PGresult *res = PQexec(conn.get(), "SELECT id, name FROM new_table ORDER BY id DESC limit 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
    } else {
        std::cout << "Get " << PQntuples(res) << " tuples, each tuple has "
                  << PQnfields(res) << " fields" << std::endl;
        // print column values
        std::cout << PQgetvalue(res, 0, 0) << "   "
                  << PQgetvalue(res, 0, 1) << '\n';
        char buffer[100];
        strncpy(buffer, PQgetvalue(res, 0, 0), 100);
        int id = std::stoi(std::string(buffer));
        std::pair<int, std::string> result = {id, std::string(PQgetvalue(res, 0, 1))};
        PQclear(res);
        return result;
    }
    PQclear(res);
    return {0, ""};
}

void Daemon::removeRecord(const std::pair<int, std::string>& record) {
    const std::string command =
            "DELETE FROM new_table WHERE id = " + std::to_string(record.first) + " AND name = '" + record.second + "'";
    PGresult *res = PQexec(conn.get(), command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Delete from table failed: " << PQresultErrorMessage(res)
                  << std::endl;
    }
    PQclear(res);
}

int Daemon::getBiggestID() {
    int result = 0;

    PGresult *res = PQexec(conn.get(), "SELECT id FROM new_table ORDER BY id DESC limit 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
    } else if (PQntuples(res) != 0) {
        char buffer[100];
        strncpy(buffer, PQgetvalue(res, 0, 0), 100);
        result = std::stoi(std::string(buffer));
    }
    PQclear(res);
    return result;
}

void Daemon::insertRecord(int id, const std::string& path, int pattern , const std::string& text) {
    const std::string command =
            "INSERT INTO parsed VALUES (" + std::to_string(id) + ", '" + path + "', " + std::to_string(pattern) + ", '" + text + "');";
    PGresult *res = PQexec(conn.get(), command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert into table failed: " << PQresultErrorMessage(res)
                  << std::endl;
    }
    PQclear(res);
}

bool Daemon::isEmpty() {
    bool empty = true;

    PGresult *res = PQexec(conn.get(), "SELECT * FROM new_table ORDER BY id DESC limit 1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
    } else if (PQntuples(res) != 0) {
        std::cout << PQgetvalue(res, 0, 0) << "\n";
        empty = false;
    }
    PQclear(res);
    return empty;
}

void Daemon::recognize() {
    auto signal = std::signal(SIGINT, signalHandler);
    while (signal != SIG_IGN) {
        if (isEmpty()) {
            std::cout << "Sleep for 1 sec\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        auto record = getPath();
        std::cout << "We get :\n";
        std::cout << record.first << "   "
                  << record.second << '\n';
        if (!(record.first == 0 && record.second.empty())) {
            removeRecord(record);
        }
        int id = getBiggestID() + 1;
        int picTemplate = getTemplateOfPicture(record.second);
        std::string picText = getTextInPicture(record.second);
        insertRecord(id, record.second, picTemplate, picText);
    }
}

int Daemon::getTemplateOfPicture(const std::string& path) {
    /// TODO: определяем шаблон картинки
    return 1;
}

std::string Daemon::getTextInPicture(const std::string& path) {
    std::string text;
    auto *ocr = new tesseract::TessBaseAPI();

    // Initialize OCR engine to use English (eng) and The LSTM OCR engine.
    ocr->Init(nullptr, "eng+rus", tesseract::OEM_LSTM_ONLY);

    // Set Page segmentation mode to PSM_AUTO (3)
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    // Open input image using OpenCV
    cv::Mat im = cv::imread(path, cv::IMREAD_COLOR);

    // Set image data
    ocr->SetImage(im.data, im.cols, im.rows, 3, static_cast<int>(im.step));

    text = std::string(ocr->GetUTF8Text());

    ocr->End();
    return text;
}
