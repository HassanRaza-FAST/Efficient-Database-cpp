#include <iostream>
#include <fstream>
#include "Menu.h"
#include "Functions.h"
#include "DatabasePointer.h"

using namespace std;

int main() {

    fstream file;
    file.open("db\\data_directory.dat");
    if (!file.is_open()) {
        if (!initializeDatabase()) {
            cout << "Unable to initialize database.\n" << endl;
            return 0;
        }
    }
    else file.close();

    DatabasePointer dbptr;
    if (!dbptr.readData("db\\data_directory.dat")) {
        cout << "Unable to access database.\n";
        return 0;
    }

    Menu menu(dbptr);

    menu.show();

    return 0;

}