#include "client.h"
#include "server.h"
#include "memInfo.h"

int main() {
    MemInfo<> mem;
    boost::asio::io_context io_context;
    
    std::cout << "Hello, world!\n";
    return 0;
}