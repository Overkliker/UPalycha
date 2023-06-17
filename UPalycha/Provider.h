#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <iostream>
#include <format>
#include <unordered_map>
#include <list>
#include <Windows.h>
#include "Models.h"
#include "Admin.h"


using namespace std;

int getCount(int* id) {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from apps where id "+ to_string(*id) + "";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        App new_app;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_app.id = sqlite3_column_int(stmt, 0);
            new_app.prodID = sqlite3_column_int(stmt, 1);
            new_app.count = sqlite3_column_int(stmt, 2);
        }
        return new_app.count;
    }
    else
    {
        return 0;
    }
}

void printApps() {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from apps";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        App new_app;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_app.id = sqlite3_column_int(stmt, 0);
            new_app.count = sqlite3_column_int(stmt, 1);
            new_app.prodID = sqlite3_column_int(stmt, 2);
            cout << "ID: " + to_string(new_app.id) << " ProductID: " + to_string(new_app.count) +"" << " Count: " + to_string(new_app.prodID) << endl;
        }
    }
}

void intProvider(Worker* provider) {
	while (true)
	{
		cout << endl;
        printApps();

        cout << "Введите ID заявки, которую хотите обработать" << endl;
        int id, count;
        cin >> id;

        sqlite3* db;
        char* err;

        count = getCount(&id);
        string query = "update warehouse set count = count + " + to_string(count) + " where id = " + to_string(id) + "";


        sqlite3_open("myDb.db", &db);
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

        if (rc == SQLITE_OK) {
            cout << "Заявка успешно обработанна!" << endl;
        }
        else
        {
            query = "insert into warehouse (count, productID) values (" + to_string(count) + ", " + to_string(id) + ")";


            sqlite3_open("myDb.db", &db);
            int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
            if (rc == SQLITE_OK) {
                cout << "Заявка успешно обработанна!" << endl;
            }
        }

        count = getCount(&id);
        query = "insert into journal (wd, who) values ('send_prod"+ to_string(id) +"', '"+ provider->fio +"')";


        sqlite3_open("myDb.db", &db);
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
   
	}
}