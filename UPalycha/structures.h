#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <format>
#include <unordered_map>
#include <list>
#include <Windows.h>
#include "Models.h"


using namespace std;


////////////////////////////// USER /////////////////////////////////

User sel(string* nameF, string* passw) {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;
    //������ � �������� �����
    string query = "select id, name, password from user where (name = '" + *nameF + "') and (password = '" + *passw + "')";
    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    //�������� �� ���������� �������
    if (rc == SQLITE_OK) {
        User newUser;
        //������� ���� �������� �������
        while (sqlite3_step(stmt) != SQLITE_DONE) {

            newUser.id = sqlite3_column_int(stmt, 0);
            newUser.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            newUser.password = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        }

        return newUser;
    }
    else {
        User nu;
        nu.id = 0;
        nu.name = "";
        nu.password = "";
        return nu;
    }

}


User userAut() {
    string name, passw;

    cout << "������� ���:" << endl;
    cin.get();
    cin.clear();
    getline(cin, name);

    cout << "������� ������: " << endl;
    cin.get();
    cin.clear();
    getline(cin, passw);

    //����������� ������
    HINSTANCE load;
    load = LoadLibrary(L"HashLib.dll");
    typedef std::string(*hashing) (std::string);
    hashing Hashing;
    Hashing = (hashing)GetProcAddress(load, "hashing");

    string hash = Hashing(passw);
    User us = sel(&name, &hash);
    return us;
}


vector<Dishes> makeOrder() {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;

    string query = "select * from  dish where ((select count from warehouse where warehouse.productID = dish.productID) > 0)";

    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        int vecSize = 0;
        vector<Dishes> l_dishes(vecSize);

        //������� ���� �������� �������
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            vecSize++;
            Dishes newDish;

            newDish.id = sqlite3_column_int(stmt, 0);
            newDish.name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            newDish.productID = sqlite3_column_int(stmt, 2);
            newDish.grams = sqlite3_column_int(stmt, 3);
            newDish.price = sqlite3_column_int(stmt, 4);
            newDish.description = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            newDish.time = sqlite3_column_int(stmt, 6);

            l_dishes.push_back(newDish);
        }

        return l_dishes;
    }
}

void writeOrder(Dishes* dish, User* myUser) {
    sqlite3* db;
    char* err;

    string query = "insert into orders (dishID, status, userID) values (" + to_string(dish->id) + ", 1, " + to_string(myUser->id) + ")";


    sqlite3_open("myDb.db", &db);
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << "���-�� ����� �� ���!" << endl;
    }
    else {
        balance += dish->price;
        prodashi += dish->price;

        sqlite3* db;
        char* err;

        string query = "insert into buhg (balance, zakupki, prodano, now_prod) values ("+ to_string(balance) +", "+ to_string(zakupki) +", "+ to_string(prodashi) +", " + to_string(dish->price) + ")";


        sqlite3_open("myDb.db", &db);
        int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    }
}

vector<Order> checkOrder(User* myUser) {
    //���������� ��� ��
    sqlite3* db;
    sqlite3_stmt* stmt;

    string query = "select * from orders where (userID = " + to_string(myUser->id) + ")";

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

            l_orders.push_back(newOrder);
        }

        return l_orders;
    }
}


void intUser(User* myUser) {

    while (true) {
        cout << "������� ����� - 1; ���������� ������ ������� - 2" << endl;
        int choice;

        cin >> choice;
        cout << endl;
        if (choice == 1) {
            vector<Dishes> list_dishes = makeOrder();
            int j = 0;
            for (Dishes i : list_dishes) {
                cout << j << ". " << "��������: " + i.name + " " << "���: " + to_string(i.grams) + " "
                    << "����: " + to_string(i.price) + " " << "�����: " + to_string(i.time) + " "
                    << "��������: " + i.description + "" << endl;
                j++;
            }

            cout << "�������� ����� �����, ������� ������ ��������" << endl;
            int number;
            cin >> number;
            if (number >= 0 && number <= j) {
                writeOrder(&list_dishes[number], myUser);
            }
            else {
                cout << "������ ������ ���� ����" << endl << endl;
            }


        }
        else if (choice == 2) {
            vector<Order> user_orders = checkOrder(myUser);

            for (Order i : user_orders) {
                cout << "�����: " + to_string(i.dishId) + " " << "������: " + to_string(i.status) + "" << endl;
            }
        }
        else {
            cout << "������ �������� ����" << endl;
        }

    }
}
