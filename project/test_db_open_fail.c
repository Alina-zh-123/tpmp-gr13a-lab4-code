#include <assert.h>
#include <stdio.h>
#include <sqlite3.h>
#include "../includes/db.h"

void test_db_open_fail() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("definitely_not_exists.db", &db);

    assert(rc != SQLITE_OK);
    printf("Test 0: sqlite3_open error\n");

    sqlite3_close(db);
}

int main() {
    test_db_open_fail();
    printf("\nВсе тесты пройдены успешно!\n");
    return 0;
}
