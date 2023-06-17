#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <iostream>
#include <format>
#include <unordered_map>
#include <list>
#include <Windows.h>
#include "Models.h"


using namespace std;

////////////////////////////// ADMIN /////////////////////////////////

Worker sel_worker(string* nameF, string* passw) {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from employee where (login = '" + *nameF + "') and (password = '" + *passw + "')";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        Worker newWorker;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            newWorker.id = sqlite3_column_int(stmt, 0);
            newWorker.fio = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            newWorker.login = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            newWorker.password = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            newWorker.role = sqlite3_column_int(stmt, 4);
        }

        return newWorker;
    }
    else {
        Worker nu;
        nu.id = 0;
        nu.fio = "";
        nu.login = "";
        nu.password = "";
        nu.role = -1;
        return nu;
    }

}

Worker userEmp() {
    string name, passw;

    cout << "Введите имя:" << endl;
    cin.get();
    cin.clear();
    getline(cin, name);

    cout << "Введите пароль: " << endl;
    cin.get();
    cin.clear();
    getline(cin, passw);

    HINSTANCE load;
    load = LoadLibrary(L"HashLib.dll");
    typedef std::string(*hashing) (std::string);
    hashing Hashing;
    Hashing = (hashing)GetProcAddress(load, "hashing");

    string hash = Hashing(passw);
    Worker wr = sel_worker(&name, &hash);
    return wr;
}

void printProds() {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from product";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        Product new_prod;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_prod.id = sqlite3_column_int(stmt, 0);
            new_prod.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            new_prod.price = sqlite3_column_int(stmt, 2);
            cout << "ID: " + to_string(new_prod.id) + "" << " Name: " + new_prod.name << " Price: " + new_prod.price << endl;
        }
    }
}


void printDishes() {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from dish";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        Dishes new_dish;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_dish.id = sqlite3_column_int(stmt, 0);
            new_dish.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            new_dish.productID = sqlite3_column_int(stmt, 2);
            new_dish.grams = sqlite3_column_int(stmt, 3);
            new_dish.price = sqlite3_column_int(stmt, 4);
            new_dish.description = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            new_dish.time = sqlite3_column_int(stmt, 6);
            cout << "ID: " + to_string(new_dish.id) + "" << " Name: " + new_dish.name << endl;
        }
    }
}


void delMenu() {
    printDishes();
    cout << "Введите id блюда для удаления" << endl;
    int ind;
    cin >> ind;
    //Переменные под бд
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и удаления из бдшки
    string query = "delete from dish where (id = " + to_string(ind) + ")";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

    if (rc != SQLITE_OK) {
        cout << err << endl;
    }

}

bool addMenu() {
    string name, descr;
    int product_id, grams, price, timing;

    cin.get();
    cin.clear();

    cout << "Введите название нового блюда" << endl;
    getline(cin, name);

    printProds();
    cout << "Введите id продукта" << endl;
    cin >> product_id;

    cout << "Введите вес блюда" << endl;
    cin >> grams;

    cout << "Введите цену блюда" << endl;
    cin >> price;

    cout << "Введите описание нового блюда" << endl;

    cin.get();
    cin.clear();

    getline(cin, descr);

    cout << "Введите время приготовление" << endl;
    cin >> timing;

    sqlite3* db;
    char* err;

    string query = "insert into dish (dish_name, productID, grams, price, desc, time) values ('" + name + "', " + to_string(product_id) + ", " + to_string(grams) + ", " + to_string(price) + ", '" + descr + "', " + to_string(timing) + ")";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << "Что-то пошло не так!" << endl;
        return false;
    }
    else {
        cout << "Блюдо успешно добавленно!" << endl;
        return true;
    }
}

void redactMenu() {
    cout << "1 - добавить новое блюдо; 2 - удалить блюдо" << endl;
    int choice;

    cin >> choice;

    if (choice == 1) {
        addMenu();
    }
    else if (choice == 2) {
        delMenu();
    }
}

//////////// REDACT USER ////////////

void printUsers() {
    //Переменные под бд
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "select * from user";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //проверка на выполнение запроса
    if (rc == SQLITE_OK) {
        User new_user;
        //перебор всех значений запроса
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            new_user.id = sqlite3_column_int(stmt, 0);
            new_user.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            new_user.password = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            cout << "ID: " + to_string(new_user.id) + "" << " Name: " + new_user.name << endl;
        }
    }
}

void delUser() {
    printUsers();
    cout << "Введите id пользователя для удаления" << endl;
    int ind;
    cin >> ind;
    //Переменные под бд
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "delete from user where (id = " + to_string(ind) + ")";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

    if (rc != SQLITE_OK) {
        cout << err << endl;
    }

}

void addUser() {
    string name, passw;

    cin.get();
    cin.clear();

    cout << "Введите имя нового пользователя" << endl;
    getline(cin, name);

    cout << "Введите пароль нового пользователя" << endl;

    cin.get();
    cin.clear();

    getline(cin, passw);

    HINSTANCE load;
    load = LoadLibrary(L"HashLib.dll");
    typedef std::string(*hashing) (std::string);
    hashing Hashing;
    Hashing = (hashing)GetProcAddress(load, "hashing");

    string hash = Hashing(passw);

    sqlite3* db;
    char* err;

    string query = "insert into user (name, password) values ('"+ name +"', '"+ hash +"')";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << "Что-то пошло не так!" << endl;

    }
    else {
        cout << "Пользователь успешно добавленно!" << endl;
        cout << endl;
    }
}


bool redactUser() {
    cout << "1 - добавить нового пользователя; 2 - удалить пользователя" << endl;
    int choice;

    cin >> choice;

    if (choice == 1) {
        addUser();
        return true;
    }
    else if (choice == 2) {
        delUser();
        return true;
    }
    else
    {
        return false;
    }
}


///////////// REDACT PRODUCTS /////////////

void delProduct() {
    printProds();
    cout << "Введите индекс продукта для удаления" << endl;
    int ind;
    cin >> ind;
    //Переменные под бд
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;
    //Запрос и открытие бдшки
    string query = "delete from product where (id = " + to_string(ind) + ")";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

    if (rc != SQLITE_OK) {
        cout << err << endl;
    }
}

void addProduct() {
    string name;
    int price;

    cin.get();
    cin.clear();

    cout << "Введите имя нового продукта" << endl;
    getline(cin, name);

    cout << "Введите цену нового продукта" << endl;

    cin >> price;

    sqlite3* db;
    char* err;

    string query = "insert into product (prod_name, price) values ('" + name + "', "+ to_string(price) +")";
    cout << query << endl;
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << err << endl;

    }
    else {
        cout << "Продукт успешно добавлен!" << endl;
    }
}

bool redactProduct() {
    cout << "1 - добавить новый продукт; 2 - удалить продукт" << endl;
    int choice;

    cin >> choice;

    if (choice == 1) {
        addProduct();
        return true;
    }
    else if (choice == 2) {
        delProduct();
        return true;
    }
    else
    {
        return false;
    }
}


void intAdmin(Worker* admin) {
    while (true) {
        cout << endl;
        cout << "Ввыедите действие:\n 1 - редактировать меню \n 2 - редактировать пользователя \n 3 - редактировать продукты" << endl;
        int select;
        cin >> select;
        switch (select)
        {
        case (1):

            redactMenu();

        case (2):

            redactUser();

        case (3):
            redactProduct();
            break;
        default:
            return;
            break;
        }
    }
}