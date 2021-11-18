#pragma once
#include <vector>
#include <boost/asio.hpp>

class Client {
private:
    /* some args */
public:
    void sendInfo();  // send information to server
    void getInfo();   // get information from server
};