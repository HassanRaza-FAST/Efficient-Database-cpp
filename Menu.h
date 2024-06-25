#pragma once
#include <iostream>
#include <Windows.h>
#include "DatabasePointer.h"
#include "Functions.h"

using namespace std;

class Menu {

    DatabasePointer dbptr;

public:

    Menu(DatabasePointer& dbptr) { this->dbptr = dbptr; }

    void show() {

        Sleep(500);
        system("color 0a");

        string input;

        while (true) {

            outputHeading();

            cout << "--help\n";
            cout << "--exit\n";
            cout << "\n> ";

            getline(cin, input);

            processInput(input);

        }

    }

private:

    void outputHeading() {

        system("cls");
        cout << "    _____     ______     _____     ______    \n";
        cout << "   /\\  __-.  /\\  ___\\   /\\  __-.  /\\  == \\   \n";
        cout << "   \\ \\ \\/\\ \\ \\ \\___  \\  \\ \\ \\/\\ \\ \\ \\  __<   \n";
        cout << "    \\ \\____-  \\/\\_____\\  \\ \\____-  \\ \\_____\\ \n";
        cout << "     \\/____/   \\/_____/   \\/____/   \\/_____/ \n";
        cout << endl << endl;

    }

    void processInput(string& input) {

        if (input == "--help") outputHelp();

        else if (input == "--exit") quitApplication();

        else if (input.find("create") != -1) {
            if (input.find("avl") != -1) {
                string key = input.substr(input.find("create avl ") + 11);
                if (!initializeAVLTree(dbptr, key)) cout << "\n> action failed.";
                else cout << "\n> action successful.";
                waitForEnter();
            }

            if (input.find("rb") != -1) {
                string key = input.substr(input.find("create rb ") + 10);
                if (!initializeRBTree(dbptr, key)) cout << "\n> action failed.";
                else cout << "\n> action successful.";
                waitForEnter();
            }
        }

        else if (input.find("search") != -1) {

            string key = input.substr(input.find("search ") + 7,
                input.find("[", input.find("search ") + 7) - (input.find("search ") + 7) - 1);
            string keyValue = input.substr(input.find("[") + 1, input.find("]") - (input.find("[")) - 1);

            if (input.find("where") != -1) {
                string keyWhere = input.substr(input.find("where ") + 6,
                    input.find("[", input.find("where ") + 6) - (input.find("where ") + 6) - 1);
                string keyValueWhere = input.substr(
                    input.find("[", input.find("where ") + 6) + 1,
                    input.find("]", (input.find("where ") + 6)) - input.find("[", input.find("where ") + 6) - 1);

                if (keyValue.find(",") == -1) {
                    if (!(searchWhere(dbptr, key, keyValue, keyWhere, keyValueWhere)))
                        cout << "\n> action failed.";
                    else cout << "\n> action successful.";
                }
                else {
                    if (!(searchWhereRange(dbptr, key, keyValue, keyWhere, keyValueWhere)))
                        cout << "\n> action failed.";
                    else cout << "\n> action successful.";
                }

            }
            else {
                if (keyValue.find(",") == -1) {
                    if (!search(dbptr, key, keyValue)) cout << "\n> action failed.";
                    else cout << "\n> action successful.";
                }
                else {
                    if (!searchRange(dbptr, key, keyValue)) cout << "\n> action failed.";
                    else cout << "\n> action successful.";
                }
            }

            waitForEnter();
        }

        else if (input.find("update") != -1) {
            string key = input.substr(input.find("update ") + 7,
                input.find("[", input.find("update ") + 7) - (input.find("update ") + 7) - 1);
            string keyValue = input.substr(input.find("[") + 1, input.find("]") - (input.find("[")) - 1);
            string keyUpdate = input.substr(
                input.find("where ") + 6, input.find("[", input.find("where ") + 6) - (input.find("where ") + 6) - 1);
            string keyValueUpdate = input.substr(
                input.find("[", input.find("where ") + 6) + 1,
                input.find("]", (input.find("where ") + 6)) - input.find("[", input.find("where ") + 6) - 1);
            if (!update(dbptr, key, keyValue, keyUpdate, keyValueUpdate)) cout << "\n> action failed.";
            else cout << "\n> action successful.";
            waitForEnter();
        }

        else if (input.find("delete") != -1) {
            string key = input.substr(
                input.find("where ") + 6, input.find("[", input.find("where ") + 6) - (input.find("where ") + 6) - 1);
            string keyValue = input.substr(
                input.find("[", input.find("where ") + 6) + 1,
                input.find("]", (input.find("where ") + 6)) - input.find("[", input.find("where ") + 6) - 1);
            if (!deleteEntry(dbptr, key, keyValue)) cout << "\n> action failed.";
            else cout << "\n> action successful.";
            waitForEnter();
        }

    }

    void quitApplication() { exit(0); }

    void outputHelp() {

        outputHeading();

        cout << "commands:\n\n";
        cout << "create <tree_type> <key>\n";
        cout << "search <key> [key_value]\n";
        cout << "search <key> [key_value, key_value]\n";
        cout << "search <key> [key_value] where <key> [key_value]\n";
        cout << "search <key> [key_value, key_value] where <key> [key_value]\n";
        cout << "update <key> [key_value] where <key> [key_value]\n";
        cout << "delete where <key> [key_value]\n";

        cout << "\ntree types: ";
        cout << "avl, ";
        cout << "rb";
        cout << endl;

        waitForEnter();

    }

    void waitForEnter() {

        cout << "\n> (hit enter to return)";

        cin.get();

    }

};