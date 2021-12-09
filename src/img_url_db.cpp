// private headers
#include <unsorted_img_storage/img_url_db.h>

ImgDB::ImgDB() {
    const char connInfo[] = "hostaddr=127.0.0.1 dbname=paths user=postgres password=";
    conn = PQconnectdb(connInfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
    } else {
        std::cout << "Connection to database succeed." << std::endl;
    }
}

void ImgDB::insertPath(int id, const std::string& path) {
    const std::string command =
            "INSERT INTO path_to_img VALUES (" + std::to_string(id) + ", '" + path + "');";
    PGresult *res = PQexec(conn, command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert into table failed: " << PQresultErrorMessage(res)
                  << std::endl;
    }
    PQclear(res);
}