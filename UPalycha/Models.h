#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <format>
#include <unordered_map>
#include <list>
#include <Windows.h>


using namespace std;

class User
{
public:
    int id;
    std::string name;
    std::string password;
};

class Worker {
public:
    int id;
    std::string fio;
    std::string login;
    std::string password;
    int role;
};

class Ware {
public:
    int id;
    int count;
    int productId;
};

struct Product {
    int id;
    int price;
    std::string name;
};

struct Order {
    int id;
    int dishId;
    int status;
    int userID;
};

struct  Dishes
{
    int id;
    std::string name;
    int productID;
    int grams;
    int price;
    std::string description;
    int time;
};

struct App {
    int id;
    int prodID;
    int count;
};

struct Journal
{
    int id;
    string wd;
    string who;
};

struct Buhg {
    int id;
    int balance;
    int zakupki;
    int prodano;
    int now_prod;
};

int balance = 10000;
int zakupki = 0;
int prodashi = 0;