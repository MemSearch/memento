#include "server.h"

// Will be in the future
#define page_with_memes
#define empty_page
// Will be in the future

void Server::getInfo() {
    try {
        urls = getMemUrls();
    }
    catch(const std::exception& error) {
        throw(error);
    }
}

void Server::sendInfo(/* where, from, what */) {
    if (urls.size() != 0) {
        try {
            //sendPageWithMemes();
        }
        catch(const std::exception& error) {
            throw(error);
        }
    }
    else {
        try {
            //sendEmptyPage();
        }
        catch(const std::exception& error) {
            throw(error);
        }
    }
}