#include "../includes/db.h"

int main(void) {
    sqlite3 *db;
    
    if (sqlite3_open("green.db", &db) != SQLITE_OK) {
        return 1;
    }

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    Create_Tables(db);

    Insert_Flower(db, "гвоздика", "розовая", 80.0);
    Update_Flower_Price(db, 2, 210.0);
    Show_Table(db, "flowers");
    
    Safe_Price_Update(db, 1, 150.0);
    Safe_Price_Update(db, 3, 85.0);

    sqlite3_exec(db, "INSERT OR IGNORE INTO compositions (composition_id, name) VALUES (1, 'весенний микс');", 0, 0, 0);
    sqlite3_exec(db, "INSERT OR IGNORE INTO composition_items (composition_id, flower_id, quantity) VALUES (1, 1, 5);", 0, 0, 0);
    sqlite3_exec(db, "INSERT OR IGNORE INTO composition_items (composition_id, flower_id, quantity) VALUES (1, 3, 10);", 0, 0, 0);
    sqlite3_exec(db, "INSERT OR IGNORE INTO users (user_id, username, password, role) VALUES (1, 'лиза', '123', 'клиент');", 0, 0, 0);

    Add_Order_Calc(db, "2026-03-24", "2026-03-25", 1, 2, 1);

    Search_By_Date(db, "2026-03-24");
    Reports_Select(db);

    Delete_Order_By_Id(db, 1);
    sqlite3_exec(db, "DELETE FROM flowers WHERE flower_id = 3;", 0, 0, 0);
    printf("цветок удален\n");

    sqlite3_close(db);
    return 0;
}