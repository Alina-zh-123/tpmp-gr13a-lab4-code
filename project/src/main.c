#include "../includes/db.h"

int main(void) {
    sqlite3 *db;
    if (sqlite3_open("green.db", &db) != SQLITE_OK) return 1;

    Create_Tables(db);

    int count = 0;
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM flowers", -1, &stmt, 0);
    if (sqlite3_step(stmt) == SQLITE_ROW) count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    printf("Записей в БД: %d\n", count);

    Insert_Flower(db, "Орхидея", "Тропическая", 15.0);
    Add_Order_Calc(db, "2026-03-24", "2026-03-25", 1, 1, 1);
    
    Reports_Select(db);
    Search_By_Date(db, "2026-03-24");
    Safe_Price_Update(db, 1, 20.0);

    sqlite3_close(db);
    return 0;
}