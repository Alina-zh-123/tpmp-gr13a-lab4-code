#include <assert.h>
#include <stdio.h>
#include "../includes/db.h"

int Get_Flower_Price(sqlite3 *db, int id, double *price);

void test_database_logic() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    assert(rc == SQLITE_OK);
    printf("Test 0: DB open OK\n");

    Create_Tables(db);
    printf("Test 1: Create_Tables OK\n");

    Insert_Flower(db, "Test", "Sort", 10.0);

    double price = 0;
    rc = Get_Flower_Price(db, 1, &price);
    assert(rc == SQLITE_OK);
    assert(price == 10.0);
    printf("Test 2: Insert_Flower OK\n");

    Safe_Price_Update(db, 1, 15.0);

    rc = Get_Flower_Price(db, 1, &price);
    assert(rc == SQLITE_OK);
    assert(price == 10.0);
    printf("Test 3: Safe_Price_Update OK\n");

    Delete_Order_By_Id(db, 999);
    printf("Test 4: Delete_Order OK\n");

    sqlite3_close(db);
}

void test_db_open_fail();

int main() {
    test_database_logic();
    test_db_open_fail();
    printf("\nВсе тесты пройдены успешно!\n");
    return 0;
}
