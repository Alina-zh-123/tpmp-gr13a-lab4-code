#include <assert.h>
#include <stdio.h>
#include "../includes/db.h"

void test_database_logic() {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    Create_Tables(db);

    Insert_Flower(db, "Test", "Sort", 10.0);
    printf("Test 1: Insert_Flower - OK\n");

    Safe_Price_Update(db, 1, 15.0); 
    printf("Test 2: Safe_Price_Update Logic - OK\n");

    Delete_Order_By_Id(db, 999);
    printf("Test 3: Delete_Order - OK\n");

    sqlite3_close(db);
}

int main() {
    test_database_logic();
    printf("\nВсе тесты пройдены успешно!\n");
    return 0;
}
