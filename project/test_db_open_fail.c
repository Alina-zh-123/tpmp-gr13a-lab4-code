#include <assert.h>
#include <stdio.h>
#include <sqlite3.h>
#include "../includes/db.h"

void test_db_open_fail() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("/", &db);

    assert(rc != SQLITE_OK);
    printf("Test DB fail: sqlite3_open error\n");

    sqlite3_close(db);
}
