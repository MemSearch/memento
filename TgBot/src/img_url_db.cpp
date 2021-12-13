// private headers
#include <unsorted_img_storage/img_url_db.h>

ImgDB::ImgDB() {
    const char connInfo[] = "postgresql://ezury@localhost?port=5432&dbname=mydb";
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
            "INSERT INTO new_table VALUES (" + std::to_string(id) + ", '" + path + "');";
    PGresult *res = PQexec(conn, command.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert into table failed: " << PQresultErrorMessage(res)
                  << std::endl;
    }
    std::cout << "insert into new_table path " << path << "\n";
    PQclear(res);
}