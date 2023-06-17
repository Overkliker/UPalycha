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

void printBuhg() {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from buhg";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        Buhg new_journal;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_journal.id = sqlite3_column_int(stmt, 0);
            new_journal.balance = sqlite3_column_int(stmt, 1);
            new_journal.zakupki = sqlite3_column_int(stmt, 2);
            new_journal.prodano= sqlite3_column_int(stmt, 3);
            new_journal.now_prod = sqlite3_column_int(stmt, 4);
            cout << "ID: " + to_string(new_journal.id) << " Balance: " + to_string(new_journal.balance) + "" << " Закупки: " + to_string(new_journal.id) << "Продано: " + to_string(new_journal.prodano) << "Продажи в моменте: " + to_string(new_journal.now_prod) << endl;
        }
    }
}

void printJournal(){
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from journal";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        Journal new_journal;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_journal.id = sqlite3_column_int(stmt, 0);
            new_journal.wd = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            new_journal.who = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            cout << "ID: " + to_string(new_journal.id) << " What did: " + new_journal.wd + "" << " Who: " + new_journal.who << endl;
        }
    }
}

void intAcc(Worker* acc) {
	while (true) {
		int choice;
		cout << "1 - заявки; 2 - принятые поставки; 3 - бухгалтерия" << endl;
		cin >> choice;
		if (choice == 1) {
			printApps();
		}
		else if (choice == 2) {
            printJournal();
		}
		else if (choice == 3) {
            printBuhg();
		}
	}
}