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


/////////////////////// Aplication on product ///////////
Product retProd(int* id) {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;
    //������ � �������� �����
    string query = "select * from product where (id = "+ to_string(*id) + ")";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //�������� �� ���������� �������
    if (rc == SQLITE_OK) {
        Product new_prod;
        //������� ���� �������� �������
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_prod.id = sqlite3_column_int(stmt, 0);
            new_prod.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            new_prod.price = sqlite3_column_int(stmt, 2);
        }
        return new_prod;
    }
    else {
        Product empty;
        empty.id = 0;
        empty.name = "";
        empty.price = -1;
    }
}

void makeApp(int* id, int* count) {
    sqlite3* db;
    char* err;

    string query = "insert into apps (productID, count) values ("+ to_string(*id) + ", " + to_string(*count) + ")";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << "���-�� ����� �� ���!" << endl;
    }
    else {
        cout << "����� ������� ���������!" << endl;
    }
}

void makeOrderProd() {
	printProds();
    cout << "������� ID ��������, ������� ������ ��������" << endl;
    int id, count;
    cin >> id;
    cout << "������� ���������� ������" << endl;
    cin >> count;

    Product local_prod = retProd(&id);
    int all_price = local_prod.price * count;
    if (all_price <= balance) {
        makeApp(&id, &count);
        balance -= all_price;
        zakupki += all_price;
        sqlite3* db;
        char* err;

        string query = "insert into buhg (balance, zakupki, prodano, now_prod) values (" + to_string(balance) + ", " + to_string(zakupki) + ", "+ to_string(prodashi) +", 0)";


        sqlite3_open("myDb.db", &db);
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    }
    else {
        cout << "������������ ������� ��� �������" << endl;
    }
}


////////////////////// Check warehouse ///////////////////

void printWare() {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;
    //������ � �������� �����
    string query = "select * from warehouse";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //�������� �� ���������� �������
    if (rc == SQLITE_OK) {
        Ware new_ware;
        //������� ���� �������� �������
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_ware.id = sqlite3_column_int(stmt, 0);
            new_ware.productId = sqlite3_column_int(stmt, 1);
            new_ware.count = sqlite3_column_int(stmt, 2);
            cout << "ID: " + to_string(new_ware.id) + "" << " Product: " + to_string(new_ware.productId) << " Count: " + to_string(new_ware.count) << endl;
        }
    }
}

void intStore(Worker* storekeeper) {
    
	while (true) {
        cout << endl;
		cout << "1 - ������� ����� ���������; 2 - ���������� ���������� ������" << endl;
		int choice;
		cin >> choice;
		if (choice == 1) {
            makeOrderProd();
		}
		else if (choice == 2) {
            printWare();
		}
	}
}