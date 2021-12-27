#include "daemon.h"

int main() {
    TextRecognizer daemon("postgresql://ezury@localhost?port=5432&dbname=mydb");
    daemon.recognize();
    return 0;
}
