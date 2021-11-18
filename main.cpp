#include "daemon.h"

int main() {
    Daemon daemon("parsed.db", "recognized.db");
    daemon.work();
    return 0;
}
