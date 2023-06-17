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
#include "Provider.h"
#include "Cook.h"


using namespace std;

void pirntOrders3() {
    //ѕеременные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;

    string query = "select * from orders where status = 3";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int vecSize = 0;
        vector<Order> l_orders(vecSize);

        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            vecSize++;
            Order newOrder;

            newOrder.id = sqlite3_column_int(stmt, 0);
            newOrder.dishId = sqlite3_column_int(stmt, 1);
            newOrder.status = sqlite3_column_int(stmt, 2);
            newOrder.userID = sqlite3_column_int(stmt, 3);

            cout << "ID: " + to_string(newOrder.id) << " Status: " + to_string(newOrder.status) << " Dishes ID: " + to_string(newOrder.dishId) << endl;
        }
    }
}

void intWaiter(Worker* waiter) {
    while (true)
    {
        cout << endl;
        pirntOrders3();
        cout << "¬ведите ID заказа дл€ отправуки " << endl;
        int choice;
        cin >> choice;
        try {
            int st = 4;
            changeStateOrder(&choice, &st);
        }
        catch (const std::exception& ex) {
            cout << &ex << endl;
        }
    }
}