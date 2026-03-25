#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int callback(void *NotUsed, int argc, char **argv, char **azColName);
void Create_Tables(sqlite3 *db);
void Show_Table(sqlite3 *db, const char* table_name);
void Reports_Select(sqlite3 *db);
void Search_By_Date(sqlite3 *db, const char* date);
void Insert_Flower(sqlite3 *db, const char* name, const char* sort, double price);
void Update_Flower_Price(sqlite3 *db, int id, double new_price);
void Delete_Order_By_Id(sqlite3 *db, int id);
void Safe_Price_Update(sqlite3 *db, int id, double new_p);
void Add_Order_Calc(sqlite3 *db, const char* d1, const char* d2, int c_id, int cnt, int cust);
int Get_Flower_Price(sqlite3 *db, int id, double *price);

#endif
