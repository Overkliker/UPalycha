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


using namespace std;

void pirntOrders() {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;

    string query = "select * from orders where (status = 1)";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int vecSize = 0;
        vector<Order> l_orders(vecSize);

        //������� ���� �������� �������
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
    

void changeStateOrder(int* id, int* st) {
    sqlite3* db;
    char* err;

    string query = "update orders set status = " + to_string(*st) + " where id = " + to_string(*id) + "";


    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc == SQLITE_OK)
    {
        cout << "������ ������ ������ �� " + to_string(*st) << endl;
    }
    else {
        cout << err << endl;
    }
}

void makeDish(int* id) {
	string d;
	int ct = 0;
    int st = 2;

    changeStateOrder(id, &st);
	while (ct < 4) {
        cout << "������� ����������" << endl;
		cin.get();
		cin.clear();
		getline(cin, d);
		ct++;
	}

    st = 3;
    changeStateOrder(id, &st);

}

void intCook(Worker* cooker) {
    while (true)
    {
        cout << endl;
        pirntOrders();
        cout << "������� ID ������ ��� ������������� " << endl;
        int choice;
        cin >> choice;
        try {
            makeDish(&choice);
        }
        catch(const std::exception& ex){
            cout << &ex << endl;
        }
    }
}