#include "../includes/db.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s %s\n", azColName[i], argv[i] ? argv[i] : "пусто");
    }
    printf("\n");
    return 0;
}

void Show_Table(sqlite3 *db, const char* table_name) {
    char sql[128];
    sprintf(sql, "SELECT * FROM %s;", table_name);
    printf("таблица %s\n", table_name);
    sqlite3_exec(db, sql, callback, 0, 0);
}

void Create_Tables(sqlite3 *db) {
    char *err_msg = 0;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS flowers("
        "flower_id INTEGER PRIMARY KEY, name TEXT NOT NULL, sort TEXT NOT NULL, price NUMERIC NOT NULL);"
        "CREATE TABLE IF NOT EXISTS compositions("
        "composition_id INTEGER PRIMARY KEY, name TEXT NOT NULL UNIQUE);"
        "CREATE TABLE IF NOT EXISTS composition_items("
        "item_id INTEGER PRIMARY KEY, composition_id INTEGER NOT NULL, flower_id INTEGER NOT NULL, quantity INTEGER NOT NULL, "
        "FOREIGN KEY(composition_id) REFERENCES compositions(composition_id), "
        "FOREIGN KEY(flower_id) REFERENCES flowers(flower_id));"
        "CREATE TABLE IF NOT EXISTS users("
        "user_id INTEGER PRIMARY KEY, username TEXT NOT NULL UNIQUE, password TEXT NOT NULL, role TEXT);"
        "CREATE TABLE IF NOT EXISTS orders("
        "order_id INTEGER PRIMARY KEY, order_date DATE NOT NULL, delivery_date DATE NOT NULL, "
        "composition_id INTEGER NOT NULL, count INTEGER NOT NULL, customer_id INTEGER NOT NULL, total_price NUMERIC, "
        "FOREIGN KEY(composition_id) REFERENCES compositions(composition_id), "
        "FOREIGN KEY(customer_id) REFERENCES users(user_id));";
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) == SQLITE_OK) {
        printf("база данных готова связи активны\n");
    }
}

void Reports_Select(sqlite3 *db) {
    printf("сумма за март\n");
    sqlite3_exec(db, "SELECT SUM(total_price) AS выручка FROM orders WHERE order_date BETWEEN '2026-03-01' AND '2026-03-31'", callback, 0, 0);
    printf("популярная композиция\n");
    sqlite3_exec(db, "SELECT name AS название FROM compositions WHERE composition_id = (SELECT composition_id FROM orders GROUP BY composition_id ORDER BY SUM(count) DESC LIMIT 1)", callback, 0, 0);
    printf("срочность заказов\n");
    sqlite3_exec(db, "SELECT (julianday(delivery_date) - julianday(order_date)) AS дни, COUNT(*) AS количество FROM orders GROUP BY дни", callback, 0, 0);
    printf("расход цветов по сортам\n");
    sqlite3_exec(db, "SELECT f.name AS цветок, f.sort AS сорт, SUM(ci.quantity * o.count) AS штук FROM orders o "
                     "JOIN composition_items ci ON o.composition_id = ci.composition_id "
                     "JOIN flowers f ON ci.flower_id = f.flower_id GROUP BY f.name, f.sort", callback, 0, 0);
}

void Insert_Flower(sqlite3 *db, const char* name, const char* sort, double price) {
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, "INSERT INTO flowers (name, sort, price) VALUES (?, ?, ?);", -1, &res, 0);
    sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, sort, -1, SQLITE_STATIC);
    sqlite3_bind_double(res, 3, price);
    sqlite3_step(res);
    sqlite3_finalize(res);
    printf("цветок добавлен\n");
    Show_Table(db, "flowers");
}

void Update_Flower_Price(sqlite3 *db, int id, double new_price) {
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, "UPDATE flowers SET price = ? WHERE flower_id = ?;", -1, &res, 0);
    sqlite3_bind_double(res, 1, new_price);
    sqlite3_bind_int(res, 2, id);
    sqlite3_step(res);
    sqlite3_finalize(res);
    printf("цена обновлена\n");
    Show_Table(db, "flowers");
}

void Safe_Price_Update(sqlite3 *db, int id, double new_p) {
    sqlite3_stmt *res;
    double old_p = 0;
    sqlite3_prepare_v2(db, "SELECT price FROM flowers WHERE flower_id = ?", -1, &res, 0);
    sqlite3_bind_int(res, 1, id);
    if (sqlite3_step(res) == SQLITE_ROW) old_p = sqlite3_column_double(res, 0);
    sqlite3_finalize(res);
    if (old_p > 0 && new_p > old_p * 1.10) {
        printf("отказ рост цены более десяти процентов запрещен\n");
    } else {
        Update_Flower_Price(db, id, new_p);
    }
}

void Add_Order_Calc(sqlite3 *db, const char* d1, const char* d2, int c_id, int cnt, int cust) {
    sqlite3_stmt *res;
    const char *sql = "INSERT INTO orders (order_date, delivery_date, composition_id, count, customer_id, total_price) "
                      "SELECT ?, ?, ?, ?, ?, (SUM(f.price * ci.quantity) * ?) * "
                      "CASE WHEN (julianday(?) - julianday(?)) <= 1 THEN 1.25 "
                      "WHEN (julianday(?) - julianday(?)) <= 2 THEN 1.15 ELSE 1.0 END "
                      "FROM composition_items ci JOIN flowers f ON ci.flower_id = f.flower_id "
                      "WHERE ci.composition_id = ?";
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_text(res, 1, d1, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, d2, -1, SQLITE_STATIC);
    sqlite3_bind_int(res, 3, c_id);
    sqlite3_bind_int(res, 4, cnt);
    sqlite3_bind_int(res, 5, cust);
    sqlite3_bind_int(res, 6, cnt);
    sqlite3_bind_text(res, 7, d2, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 8, d1, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 9, d2, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 10, d1, -1, SQLITE_STATIC);
    sqlite3_bind_int(res, 11, c_id);
    sqlite3_step(res);
    sqlite3_finalize(res);
    printf("заказ оформлен\n");
    Show_Table(db, "orders");
}

void Search_By_Date(sqlite3 *db, const char* date) {
    sqlite3_stmt *res;
    const char *sql = "SELECT o.order_id, c.name, o.total_price FROM orders o "
                      "JOIN compositions c ON o.composition_id = c.composition_id "
                      "WHERE o.order_date = ?";
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_text(res, 1, date, -1, SQLITE_STATIC); 
    printf("заказы на дату %s\n", date);
    while (sqlite3_step(res) == SQLITE_ROW) { 
        printf("номер %s букет %s цена %s\n", 
               sqlite3_column_text(res, 0), sqlite3_column_text(res, 1), sqlite3_column_text(res, 2)); 
    } 
    sqlite3_finalize(res); 
}

void Delete_Order_By_Id(sqlite3 *db, int id) {
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, "DELETE FROM orders WHERE order_id = ?;", -1, &res, 0);
    sqlite3_bind_int(res, 1, id);
    sqlite3_step(res);
    sqlite3_finalize(res);
    printf("заказ удален\n");
    Show_Table(db, "orders");
}