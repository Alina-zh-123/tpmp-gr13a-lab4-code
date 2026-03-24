#include "../includes/db.h"

int main(void) {
    sqlite3 *db;
    if (sqlite3_open("green.db", &db) != SQLITE_OK) {
        return 1;
    }

    Create_Tables(db);
    Seed_Database(db);

    Insert_Flower(db, "Лилия", "Белая", 7.0);
    Update_Flower_Price(db, 2, 4.5);

    Add_Order_Calc(db, "2026-03-24", "2026-03-25", 1, 1, 1);
    Add_Order_Calc(db, "2026-03-24", "2026-03-27", 2, 2, 2);

    Reports_Select(db);
    Search_By_Date(db, "2026-03-24");

    Safe_Price_Update(db, 1, 5.8);
    Safe_Price_Update(db, 1, 10.0);

    Delete_Order_By_Id(db, 2);

    sqlite3_close(db);
    return 0;
}
