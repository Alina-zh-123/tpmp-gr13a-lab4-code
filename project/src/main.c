#include "../includes/db.h"

int main(void) {
    sqlite3 *db;
    
    if (sqlite3_open("green.db", &db) != SQLITE_OK) {
        printf("Ошибка открытия базы данных\n");
        return 1;
    }

    Init_DB(db);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM flowers", -1, &stmt, 0);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Записей в таблице цветов: %d\n", sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);

    Reports_Select(db);

    sqlite3_close(db);
    return 0;
}