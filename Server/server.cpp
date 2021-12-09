#include <netdb.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include <boost/format.hpp>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "searcher_request.h"

using json = nlohmann::json;

std::vector<std::string> someFunc() {
    std::vector<std::string> urls;
    urls.emplace_back("../static/img/Superman.jpg");
    urls.emplace_back("../static/img/Superman.jpg");
    urls.emplace_back("../static/img/Superman.jpg");
    urls.emplace_back("../static/img/thanus.jpg");
    urls.emplace_back("../static/img/Superman.jpg");
    return urls;
}

json createJson(const std::vector<std::string> &urls) {
    json j;
    for (size_t i = 0; i < urls.size(); ++i) {
        j["url" + std::to_string(i + 1)] = urls[i];
    }
    return j;
}

void simpleParser(std::map<std::string, std::string>& map, const std::string& str) {
    size_t pos = -1;
    std::string key;
    bool haveMapKey = false;
    while((pos = str.find('\"', pos + 1)) != std::string::npos) {
        // cout << "pos: " << pos << endl;
        size_t nextPos = str.find('\"', pos + 1);
        if (!haveMapKey) {
            key = str.substr(pos + 1, nextPos - pos - 1);
            haveMapKey = true;
        }
        else {
            map[key] =  str.substr(pos + 1, nextPos - pos - 1);
            haveMapKey = false;
        }
        pos = nextPos;
        // cout << "next pos: " << pos << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " port" << std::endl;
        return 0;
    }
    int port = std::stoi(std::string(argv[1]));
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        ::close(sock);
        exit(EXIT_FAILURE);
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
        perror("Some errror occured in bind func");
        exit(EXIT_FAILURE);
    }

    listen(sock, 3);
    char buf[1024];

    while (true) {
        std::cout << "\nstart new accept \n";
        int client = accept(sock, NULL, NULL);
        std::cout << "accepted " << sock << " \n";

        memset(buf, 0, 1024);
        int bytesRead = recv(client, buf, 1024, 0);
        printf("data received, size == %d \n", bytesRead);

        std::string strRecv(buf);
        std::string body = strRecv.substr(strRecv.find("{"));
        // std::cout << body << std::endl;
        // json jsonBody = json::parse(body);
        // std::string strTemplateNum = jsonBody["template"];
        // std::stringstream ss(strTemplateNum);
        // size_t templateNum;
        // ss >> templateNum;


        std::map<std::string, std::string> jsonBody;
        simpleParser(jsonBody, body);
        std::stringstream ss(jsonBody["template"]);
        size_t templateNum;
        ss >> templateNum;
        auto req = jsonBody["text"];
        std::wstring wreq;
        for (const auto& symbol : req) {
            wchar_t symb = symbol;
            wreq.push_back(symb);
        }
        std::cout << buf << std::endl
                  << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

        if (bytesRead <= 0) {
            std::cout << ("Client disconnected \n");
            break;
        }

        char newBuf[1024];
        std::vector<std::string> urls;
        try {
             SearcherRequest request(wreq, templateNum);
             urls = request.getResult();
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << "\n";
        }

//        urls = someFunc();

        json jsonAnswer = createJson(urls);
        size_t contentLen = jsonAnswer.dump().length();

        std::string strAnswer = "HTTP/1.1 200 OK\r\nContent-Type: "
                                "application/json\r\nContent-Length: " + std::to_string(contentLen) +
                                "\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + jsonAnswer.dump();

        for (auto c : strAnswer) {
            std::cout << c;
        }

        strcpy(newBuf, strAnswer.c_str());
        send(client, newBuf, bytesRead, 0);
        close(client);
    }

    return 0;
}
