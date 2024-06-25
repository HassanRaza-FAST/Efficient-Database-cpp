#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>

using namespace std;

class DatabasePointer {

public:

    string directory;
    vector<string> fileNames;

    DatabasePointer(string directory = "") {

        if (directory == "") return;

        this->directory = directory;
        fileNames = getFileNames(this->directory);

    }

    bool setDirectory(string directory) {

        if (directory == "") return false;

        fileNames = getFileNames(directory);

        if (fileNames.empty()) return false;

        this->directory = directory;

        return true;

    }

    bool readData(string filePath) {

        fstream file;
        file.open(filePath, ios::in);

        if (!file.is_open()) return false;

        string directory;
        file >> directory;

        if (directory == "") return false;

        file.close();

        fileNames = getFileNames(directory);

        if (fileNames.empty()) return false;

        this->directory = directory;

        return true;

    }

    friend ostream& operator<<(ostream& os, const DatabasePointer& object);

private:

    vector<string> getFileNames(string directory) {

        vector<string> files;
        struct dirent* entry;
        DIR* dir = opendir(directory.c_str());

        if (dir == NULL) cout << "Directory could not be accessed.\n";
        else {
            entry = readdir(dir);
            entry = readdir(dir);
            while ((entry = readdir(dir)) != NULL) files.push_back(entry->d_name);
        }

        closedir(dir);

        return files;

    }

};

ostream& operator<<(ostream& os, const DatabasePointer& object) {

    os << "Directory: " << object.directory << endl << "Files:" << endl;

    for (int i = 0; i < object.fileNames.size(); i++) {
        os << object.fileNames[i] << endl;
    }

    return os;

}