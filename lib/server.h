#pragma once
#include <vector>
#include <boost/asio.hpp>

class Server {
private:
    bool correctInfo();                    // check received information
    std::vector<std::string> getMemUrls(); // ask anower program to find mem urls 
//  void findPicByUrl();                   // find picture/image by url
    std::vector<std::string> url;
public:
    void getInfo();                        // get information from clien
    void sendInfo();                       // send information to client
};