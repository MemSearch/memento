#include "daemon.h"

int main() {
    Daemon daemon("postgresql://ezury@localhost?port=5432&dbname=mydb");
    daemon.recognize();
    return 0;
}
