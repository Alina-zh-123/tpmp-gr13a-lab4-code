#include <assert.h>
#include <stdio.h>
#include "../includes/db.h"

void test_database_logic() {
    sqlite3 *db;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    assert(rc == SQLITE_OK);
    assert(db != NULL);
    printf("Test 0: Database open - OK\n");
    
    rc = Create_Tables(db);
    assert(rc == SQLITE_OK);
    printf("Test 1: Create_Tables - OK\n");
    
    rc = Insert_Flower(db, "Test", "Sort", 10.0);
    assert(rc == SQLITE_OK);
    
    double price;
    rc = Get_Flower_Price(db, 1, &price);
    assert(rc == SQLITE_OK);
    assert(price == 10.0);
    printf("Test 2: Insert_Flower - OK\n");
    
    rc = Safe_Price_Update(db, 1, 15.0);
    assert(rc == SQLITE_OK);
    
    rc = Get_Flower_Price(db, 1, &price);
    assert(rc == SQLITE_OK);
    assert(price == 15.0);
    printf("Test 3: Safe_Price_Update - OK\n");
    
    rc = Delete_Order_By_Id(db, 999);
    assert(rc == SQLITE_OK);
    printf("Test 4: Delete_Order - OK\n");
    
    sqlite3_close(db);
}

int main() {
    test_database_logic();
    printf("\nВсе тесты пройдены успешно!\n");
    return 0;
}
