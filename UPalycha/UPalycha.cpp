#include <iostream>

#include "sqlite/sqlite3.h"
#include <string>
#include <format>
#include <unordered_map>
#include <list>
#include <Windows.h>
#include "structures.h"
#include "Admin.h"
#include "Storekeeper.h"
#include "Provider.h"
#include "Accountant.h"
#include "Cook.h"
#include "Waiter.h"

using namespace std;



int main()
{
    setlocale(LC_ALL, "Russian");
    int aut;

    cout << "Под кем хотите зайти: работник - 1; Посетитель - 2" << endl;
    cin >> aut;
    if (aut == 1) {
        Worker workAut = userEmp();

        if (workAut.id != 0) {
            switch (workAut.role)
            {
            case 1:
                intAdmin(&workAut);
                break;

            case 2:
                intStore(&workAut);
                break;

            case 3:
                intProvider(&workAut);
                break;
            case 4:
                intAcc(&workAut);
                break;

            case 5:
                intCook(&workAut);
                break;

            case 6:
                intWaiter(&workAut);
                break;
            default:
                break;
            }
        }
        else
        {
            cout << "Такого пользователя нету!";
        }
    }
    else if (aut == 2) {
        User autUs = userAut();


        if (autUs.id != 0) {
            intUser(&autUs);
        }
        else
        {
            cout << "Такого пользователя нету!";
        }
    }
    else {
        cout << "Такого выбора нет" << endl;
    }
}
