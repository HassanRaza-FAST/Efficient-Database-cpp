#pragma once
#include <iostream>
#include <string>
#include <array>

using namespace std;

class DataObject {

public:

    char key[100]{};
    array<int, 1000> line;
    array<int, 1000> file;

    DataObject(const char* key = "\0") {

        strcpy_s(this->key, 100, key);
        line.fill(-1);
        file.fill(-1);

    }

    void setLine(int lineNum, int filename) {

        for (int i = 0; i < 100; i++) {
            if (line[i] == -1) {
                line[i] = lineNum;
                file[i] = filename;
                break;
            }
        }

    }

    bool operator==(DataObject& obj) {

        try {
            if (stof(this->key) == stof(obj.key))return true;
            else return false;
        }
        catch (exception& error) {
            if (string(this->key).compare(string(obj.key)) == 0) return true;
            else return false;

        }
    }

    bool operator<(DataObject& obj) {

        try {
            if (stof(this->key) < stof(obj.key)) return true;
            else return false;
        }
        catch (exception& error) {
            if (string(this->key).compare(string(obj.key)) < 0) return true;
            else return false;

        }
    }

    bool operator>(DataObject& obj) {

        try {
            if (stof(this->key) > stof(obj.key)) return true;
            else return false;
        }
        catch (exception& error) {
            if (string(this->key).compare(string(obj.key)) > 0) return true;
            else return false;

        }
    }

    bool operator<=(DataObject& obj) {

        try {
            if (stof(this->key) <= stof(obj.key)) return true;
            else return false;
        }
        catch (exception& error) {
            if (string(this->key).compare(string(obj.key)) <= 0) return true;
            else return false;

        }
    }

    bool operator>=(DataObject& obj) {

        try {
            if (stof(this->key) >= stof(obj.key)) return true;
            else return false;
        }
        catch (exception& error) {
            if (string(this->key).compare(string(obj.key)) >= 0) return true;
            else return false;

        }
    }

    friend ostream& operator<<(ostream& os, const DataObject& object);

};

ostream& operator<<(ostream& os, const DataObject& object) {

    os << " Key: " << object.key << " Files: ";
    for (int i = 0; i < object.file.size() && object.file[i] != -1; i++) os << object.file[i] << " ";
    os << " Lines: ";
    for (int i = 0; i < object.line.size() && object.file[i] != -1; i++) os << object.line[i] << " ";
    os << endl;

    return os;

}