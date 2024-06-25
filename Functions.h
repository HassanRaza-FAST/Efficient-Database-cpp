#pragma once
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <dirent.h>
#include "DatabasePointer.h"
#include "DataObject.h"
#include "AVLTree.h"
#include "RBTree.h"

using namespace std;

bool initializeDatabase();
bool initializeAVLTree(DatabasePointer& dbptr, string key_head);
bool initializeRBTree(DatabasePointer& dbptr, string key_head);
bool checkFile(string& treeType, string key);
bool search(DatabasePointer& dbptr, string key, string keyValue);
bool searchRange(DatabasePointer& dbptr, string key, string keyValue);
bool searchWhere(DatabasePointer& dbptr, string key, string keyValue, string keyWhere, string keyValueWhere);
bool searchWhereRange(DatabasePointer& dbptr, string key, string keyValue, string keyWhere, string keyValueWhere);
bool update(DatabasePointer& dbptr, string key, string keyValue, string keyUpdate, string keyValueUpdate);
bool deleteEntry(DatabasePointer& dbptr, string key, string keyValue);
void saveAVLTree(fstream& file, AVLNode<DataObject>*& root);
void saveRBTree(fstream& file, RBNode<DataObject>*& root);
bool constructAVLTree(AVLTree<DataObject>& tree, string key);
bool constructRBTree(RBTree<DataObject>& tree, string key);
bool resetTrees(DatabasePointer& dbptr);
int get_coloumn_num(DatabasePointer& dbptr, string key_head);
void extract_row_cells(vector<string>& words, string line);

bool initializeDatabase() {

    string directory;
    cout << "Initializing Database...\n";
    cout << "Enter data directory: ";
    cin >> directory;

    DatabasePointer dbptr(directory);

    if (dbptr.fileNames.empty()) return false;

    string dirStr = ".\\db";
    wstring dirWStr = wstring(dirStr.begin(), dirStr.end());
    const wchar_t* dir = dirWStr.c_str();
    _wmkdir(dir);

    fstream file;
    file.open("db\\data_directory.dat", ios::out);

    if (!file.is_open()) return false;

    file << (dbptr.directory).c_str() << endl;

    file.close();

    cout << "Database initialized.\n";

    return true;

}

bool initializeAVLTree(DatabasePointer& dbptr, string key_head) {

    int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
    vector<string> cells; // to hold the values in the cell row wise

    col_no = get_coloumn_num(dbptr, key_head);

    fstream checkFile;
    checkFile.open("db\\avl_" + key_head + ".dat", ios::out | ios::binary);
    if (!checkFile.is_open())
        cout << "(action in progress: initializing " << key_head << " avl tree)\n";
    else cout << "(action in progress: altering " << key_head << " avl tree)\n";
    checkFile.close();

    AVLTree<DataObject> tree;

    fstream dataFile;
    for (int i = 0; i < dbptr.fileNames.size(); i++) {

        dataFile.open(dbptr.directory + "\\" + dbptr.fileNames[i], ios::in);
        if (!dataFile.is_open()) return false;

        string line;
        int count = 0;

        while (getline(dataFile, line)) {
            count++;
            if (line == "") continue;
            if (line.find(key_head) != -1) continue;

            // we have identified the col number, and we only want that value at that column
            extract_row_cells(cells, line);
            // the cells are storing all the words, but we want the value at col_no's position only
            string val = cells[col_no];
            DataObject object(val.c_str());
            object.setLine(count, i);
            bool insertion = true;
            insertion = tree.insert(object);
            cells.clear();

            if (!insertion) {
                AVLNode<DataObject>* node = tree.search(object);

                for (int i = 0; i < node->data.line.size(); i++) {
                    if (node->data.line[i] == -1) {
                        node->data.line[i] = object.line[0];
                        node->data.file[i] = object.file[0];
                        break;
                    }
                }

            }

        }

        dataFile.close();

    }

    fstream file;
    file.open("db\\avl_" + key_head + ".dat", ios::out | ios::binary);
    if (!file.is_open()) return false;

    saveAVLTree(file, tree.root);

    file.close();

    return true;

}

bool initializeRBTree(DatabasePointer& dbptr, string key_head) {

    int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
    vector<string> cells; // to hold the values in the cell row wise

    col_no = get_coloumn_num(dbptr, key_head);

    fstream checkFile;
    checkFile.open("db\\rb_" + key_head + ".dat", ios::out | ios::binary);
    if (!checkFile.is_open())
        cout << "(action in progress: initializing " << key_head << " rb tree)\n";
    else cout << "(action in progress: altering " << key_head << " rb tree)\n";
    checkFile.close();

    RBTree<DataObject> tree;

    fstream dataFile;
    for (int i = 0; i < dbptr.fileNames.size(); i++) {

        dataFile.open(dbptr.directory + "\\" + dbptr.fileNames[i], ios::in);
        if (!dataFile.is_open()) return false;

        string line;
        int count = 0;

        while (getline(dataFile, line)) {
            count++;
            if (line == "") continue;
            if (line.find(key_head) != -1) continue;

            // we have identified the col number, and we only want that value at that column
            extract_row_cells(cells, line);
            // the cells are storing all the words, but we want the value at col_no's position only
            string val = cells[col_no];
            DataObject object(val.c_str());
            object.setLine(count, i);
            bool insertion = true;
            insertion = tree.insert(object);
            cells.clear();

            if (!insertion) {
                RBNode<DataObject>* node = tree.search(object);

                for (int i = 0; i < node->data.line.size(); i++) {
                    if (node->data.line[i] == -1) {
                        node->data.line[i] = object.line[0];
                        node->data.file[i] = object.file[0];
                        break;
                    }
                }

            }

        }

        dataFile.close();

    }

    fstream file;
    file.open("db\\rb_" + key_head + ".dat", ios::out | ios::binary);
    if (!file.is_open()) return false;

    saveRBTree(file, tree.root);

    file.close();

    return true;

}

bool checkFile(string& treeType, string key) {

    bool fileFound = false;

    fstream file;

    if (!fileFound) {
        file.open("db\\avl_" + key + ".dat");
        if (file.is_open()) {
            fileFound = true;
            treeType = "avl";
        }
    }

    if (!fileFound) {
        file.open("db\\rb_" + key + ".dat");
        if (file.is_open()) {
            fileFound = true;
            treeType = "rb";
        }
    }

    if (!fileFound) {
        file.open("db\\b_" + key + ".dat");
        if (file.is_open()) {
            fileFound = true;
            treeType = "b";
        }
    }

    file.close();

    if (!fileFound) return false;

}

bool search(DatabasePointer& dbptr, string key, string keyValue) {

    string treeType = "";

    if (!checkFile(treeType, key)) return false;

    cout << "(action in progress: searching in " << key << " " << treeType << " tree)\n";

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        AVLNode<DataObject>* node = tree.search(object);

        if (node == nullptr) return false;

        fstream file;

        cout << endl;
        for (int i = 0; i < node->data.file.size(); i++) {

            if (node->data.file[i] == -1) break;
            file.open(dbptr.directory + "\\" + dbptr.fileNames[node->data.file[i]]);
            if (!file.is_open()) return false;

            int line = 0;
            string data;

            while (line != node->data.line[i]) {
                line++;
                getline(file, data);
            }

            cout << data << endl;

            file.close();

        }

    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        RBNode<DataObject>* node = tree.search(object);

        if (node == nullptr) return false;

        fstream file;

        cout << endl;
        for (int i = 0; i < node->data.file.size(); i++) {

            if (node->data.file[i] == -1) break;
            file.open(dbptr.directory + "\\" + dbptr.fileNames[node->data.file[i]]);
            if (!file.is_open()) return false;

            int line = 0;
            string data;

            while (line != node->data.line[i]) {
                line++;
                getline(file, data);
            }

            cout << data << endl;

            file.close();

        }

    }

    return true;

}

bool searchRange(DatabasePointer& dbptr, string key, string keyValue) {

    string treeType = "";

    if (!checkFile(treeType, key)) return false;

    cout << "(action in progress: searching in " << key << " " << treeType << " tree)\n";

    string rangeFrom = keyValue.substr(0, keyValue.find(","));
    string rangeTo = keyValue.substr(keyValue.find(",") + 1, keyValue.length() - keyValue.find(","));

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, key)) return false;

        DataObject rangeFromObject(rangeFrom.c_str());
        DataObject rangeToObject(rangeTo.c_str());
        vector<AVLNode<DataObject>*> searchResult;

        if (!tree.search(rangeFromObject, rangeToObject, searchResult)) return false;

        fstream file;

        cout << endl;
        for (int i = 0; i < searchResult.size(); i++) {

            for (int j = 0; j < searchResult[i]->data.file.size(); j++) {

                if (searchResult[i]->data.file[j] == -1) break;
                file.open(dbptr.directory + "\\" + dbptr.fileNames[searchResult[i]->data.file[j]]);
                if (!file.is_open()) return false;

                int line = 0;
                string data;

                while (line != searchResult[i]->data.line[j]) {
                    line++;
                    getline(file, data);
                }

                cout << data << endl;

                file.close();

            }

        }

    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, key)) return false;

        DataObject rangeFromObject(rangeFrom.c_str());
        DataObject rangeToObject(rangeTo.c_str());
        vector<RBNode<DataObject>*> searchResult;

        if (!tree.search(rangeFromObject, rangeToObject, searchResult)) return false;

        fstream file;

        cout << endl;
        for (int i = 0; i < searchResult.size(); i++) {

            for (int j = 0; j < searchResult[i]->data.file.size(); j++) {

                if (searchResult[i]->data.file[j] == -1) break;
                file.open(dbptr.directory + "\\" + dbptr.fileNames[searchResult[i]->data.file[j]]);
                if (!file.is_open()) return false;

                int line = 0;
                string data;

                while (line != searchResult[i]->data.line[j]) {
                    line++;
                    getline(file, data);
                }

                cout << data << endl;

                file.close();

            }

        }

    }

    return true;

}

bool searchWhere(DatabasePointer& dbptr, string key, string keyValue, string keyWhere, string keyValueWhere) {

    string treeType = "";

    if (!checkFile(treeType, key)) return false;

    cout << "(action in progress: searching in " << key << " " << treeType << " tree)\n";

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        AVLNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, keyWhere);
        int current_line = 0;

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[i] == -1) break;
            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]);
            if (!file_in.is_open()) return false;

            current_line = 0;
            while (current_line != result->data.line[i]) {
                current_line++;
                getline(file_in, line);
            }
            cells.clear();
            extract_row_cells(cells, line);

            if (cells[col_no] == keyValueWhere)
                cout << line << endl;

            file_in.close();

        }

    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        RBNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, keyWhere);
        int current_line = 0;

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[i] == -1) break;
            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]);
            if (!file_in.is_open()) return false;

            current_line = 0;
            while (current_line != result->data.line[i]) {
                current_line++;
                getline(file_in, line);
            }
            cells.clear();
            extract_row_cells(cells, line);

            if (cells[col_no] == keyValueWhere)
                cout << line << endl;

            file_in.close();

        }

    }

    return true;

}

bool searchWhereRange(DatabasePointer& dbptr, string key, string keyValue, string keyWhere, string keyValueWhere) {

    string treeType = "";

    if (!checkFile(treeType, key)) return false;

    cout << "(action in progress: searching in " << key << " " << treeType << " tree)\n";

    string rangeFrom = keyValue.substr(0, keyValue.find(","));
    string rangeTo = keyValue.substr(keyValue.find(",") + 1, keyValue.length() - keyValue.find(","));

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, key)) return false;

        DataObject rangeFromObject(rangeFrom.c_str());
        DataObject rangeToObject(rangeTo.c_str());
        vector<AVLNode<DataObject>*> searchResult;

        if (!tree.search(rangeFromObject, rangeToObject, searchResult)) return false;
        fstream file;

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, keyWhere);
        cout << endl;

        cout << endl;
        for (int i = 0; i < searchResult.size(); i++) {

            for (int j = 0; j < searchResult[i]->data.file.size(); j++) {

                if (searchResult[i]->data.file[j] == -1) break;
                file.open(dbptr.directory + "\\" + dbptr.fileNames[searchResult[i]->data.file[j]]);
                if (!file.is_open()) return false;

                int line = 0;
                string data;

                while (line != searchResult[i]->data.line[j]) {
                    line++;
                    getline(file, data);
                }

                cells.clear();
                extract_row_cells(cells, data);

                if (cells[col_no] == keyValueWhere)
                    cout << data << endl;

                file.close();

            }

        }
    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, key)) return false;

        DataObject rangeFromObject(rangeFrom.c_str());
        DataObject rangeToObject(rangeTo.c_str());
        vector<RBNode<DataObject>*> searchResult;

        if (!tree.search(rangeFromObject, rangeToObject, searchResult)) return false;
        fstream file;

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, keyWhere);
        cout << endl;

        cout << endl;
        for (int i = 0; i < searchResult.size(); i++) {

            for (int j = 0; j < searchResult[i]->data.file.size(); j++) {

                if (searchResult[i]->data.file[j] == -1) break;
                file.open(dbptr.directory + "\\" + dbptr.fileNames[searchResult[i]->data.file[j]]);
                if (!file.is_open()) return false;

                int line = 0;
                string data;

                while (line != searchResult[i]->data.line[j]) {
                    line++;
                    getline(file, data);
                }

                cells.clear();
                extract_row_cells(cells, data);

                if (cells[col_no] == keyValueWhere)
                    cout << data << endl;

                file.close();

            }

        }
    }

    return true;

}

bool update(DatabasePointer& dbptr, string key, string keyValue, string keyUpdate, string keyValueUpdate) {

    string treeType = "";

    if (!checkFile(treeType, keyUpdate)) return false;

    cout << "(action in progress: updating " << key << ")\n";

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, keyUpdate)) return false;

        DataObject object(keyValueUpdate.c_str());

        AVLNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;
        fstream file_out; // used to write the new updated file

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, key);

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[i] == -1) break;

            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]);
            if (!file_in.is_open()) return false;
            file_out.open("db\\new.csv", ios::binary | ios::trunc | ios::out);
            if (!file_out.is_open()) return false;

            int current_lineno = 1;
            while (getline(file_in, line)) {
                if (current_lineno == result->data.line[i]) {
                    // we are on the line where updation needs to take place
                    // the entries might contain commas so we have to deal with it
                    vector<string> cells;
                    extract_row_cells(cells, line);

                    // first write all the values cell by cell till the updated coloumn
                    // then write the updated coloumn 
                    // then write remanining cells normally

                    for (int i = 0; i < cells.size(); i++) {
                        if (col_no == i) {
                            // this is the updated word
                            file_out << keyValue << ',';
                        }
                        else {
                            file_out << cells[i] << ',';
                        }
                        if (i == cells.size() - 1) file_out << '\n';
                    }
                }
                else {
                    // this is the normal entry and the whole row can be directly writen
                    file_out << line;
                    file_out << '\n';
                }

                current_lineno++;

            }

            file_in.close();
            file_out.close();

            remove((dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]).c_str());
            rename("db\\new.csv", (dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]).c_str());

        }

    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, keyUpdate)) return false;

        DataObject object(keyValueUpdate.c_str());

        RBNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;
        fstream file_out; // used to write the new updated file

        int col_no = 0; // will store the number in which the indexing coloum is (based on 0th indexing)
        vector<string> cells; // to hold the values in the cell row wise
        string line;
        col_no = get_coloumn_num(dbptr, key);

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[i] == -1) break;

            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]);
            if (!file_in.is_open()) return false;
            file_out.open("db\\new.csv", ios::binary | ios::trunc | ios::out);
            if (!file_out.is_open()) return false;

            int current_lineno = 1;
            while (getline(file_in, line)) {
                if (current_lineno == result->data.line[i]) {
                    // we are on the line where updation needs to take place
                    // the entries might contain commas so we have to deal with it
                    vector<string> cells;
                    extract_row_cells(cells, line);

                    // first write all the values cell by cell till the updated coloumn
                    // then write the updated coloumn 
                    // then write remanining cells normally

                    for (int i = 0; i < cells.size(); i++) {
                        if (col_no == i) {
                            // this is the updated word
                            file_out << keyValue << ',';
                        }
                        else {
                            file_out << cells[i] << ',';
                        }
                        if (i == cells.size() - 1) file_out << '\n';
                    }
                }
                else {
                    // this is the normal entry and the whole row can be directly writen
                    file_out << line;
                    file_out << '\n';
                }

                current_lineno++;

            }

            file_in.close();
            file_out.close();

            remove((dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]).c_str());
            rename("db\\new.csv", (dbptr.directory + "\\" + dbptr.fileNames[result->data.file[i]]).c_str());

        }

    }

    if (!resetTrees(dbptr)) return false;

    return true;

}

bool deleteEntry(DatabasePointer& dbptr, string key, string keyValue) {

    string treeType = "";

    if (!checkFile(treeType, key)) return false;

    cout << "(action in progress: deleting entry/enteries where " << key << " = " << keyValue << ")\n";

    if (treeType == "avl") {

        AVLTree<DataObject> tree;

        if (!constructAVLTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        AVLNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;
        fstream file_out; // used to write the new updated file

        vector<string> cells; // to hold the values in the cell row wise
        string line;

        int fileIndex = 0, lineIndex = 0;

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[fileIndex] == -1) break;
            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex]]);
            if (!file_in.is_open()) return false;

            file_out.open("db\\new.csv", ios::binary | ios::trunc | ios::out);
            if (!file_out.is_open()) return false;

            int current_lineno = 1;

            while (getline(file_in, line)) {
                if (current_lineno != result->data.line[lineIndex]) {
                    //this entry does not need to be deleted, write it simply to new file
                    file_out << line << '\n';
                }
                else {
                    fileIndex++;
                    lineIndex++;
                }
                current_lineno++;
            }

            file_in.close();
            file_out.close();

            remove((dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex - 1]]).c_str());
            rename("db\\new.csv", (dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex - 1]]).c_str());

        }
    }

    else if (treeType == "rb") {

        RBTree<DataObject> tree;

        if (!constructRBTree(tree, key)) return false;

        DataObject object(keyValue.c_str());

        RBNode<DataObject>* result = tree.search(object);

        if (result == nullptr) return false;

        fstream file_in;
        fstream file_out; // used to write the new updated file

        vector<string> cells; // to hold the values in the cell row wise
        string line;

        int fileIndex = 0, lineIndex = 0;

        cout << endl;
        for (int i = 0; i < result->data.file.size(); i++) {

            if (result->data.file[fileIndex] == -1) break;
            file_in.open(dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex]]);
            if (!file_in.is_open()) return false;

            file_out.open("db\\new.csv", ios::binary | ios::trunc | ios::out);
            if (!file_out.is_open()) return false;

            int current_lineno = 1;

            while (getline(file_in, line)) {
                if (current_lineno != result->data.line[lineIndex]) {
                    //this entry does not need to be deleted, write it simply to new file
                    file_out << line << '\n';
                }
                else {
                    fileIndex++;
                    lineIndex++;
                }
                current_lineno++;
            }

            file_in.close();
            file_out.close();

            remove((dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex - 1]]).c_str());
            rename("db\\new.csv", (dbptr.directory + "\\" + dbptr.fileNames[result->data.file[fileIndex - 1]]).c_str());

        }
    }

    if (!resetTrees(dbptr)) return false;

    return true;
}

void saveAVLTree(fstream& file, AVLNode<DataObject>*& root) {

    if (!root) return;

    saveAVLTree(file, root->left);
    file.write((char*)&root->data, sizeof(root->data));
    saveAVLTree(file, root->right);

}

void saveRBTree(fstream& file, RBNode<DataObject>*& root) {

    if (!root) return;

    saveRBTree(file, root->left);
    file.write((char*)&root->data, sizeof(root->data));
    saveRBTree(file, root->right);

}

bool constructAVLTree(AVLTree<DataObject>& tree, string key) {

    fstream file;
    file.open("db\\avl_" + key + ".dat", ios::in | ios::binary);
    if (!file.is_open()) return false;

    DataObject obj;

    while (file.read((char*)&obj, sizeof(obj))) tree.insert(obj);

    file.close();

    return true;

}

bool constructRBTree(RBTree<DataObject>& tree, string key) {

    fstream file;
    file.open("db\\rb_" + key + ".dat", ios::in | ios::binary);
    if (!file.is_open()) return false;

    DataObject obj;

    while (file.read((char*)&obj, sizeof(obj))) tree.insert(obj);

    file.close();

    return true;

}

bool resetTrees(DatabasePointer& dbptr) {

    vector<string> files;
    struct dirent* entry;
    DIR* dir = opendir("db");

    if (dir == NULL) {
        cout << "Directory could not be accessed.\n";
        return false;
    }
    else {
        entry = readdir(dir);
        entry = readdir(dir);
        while ((entry = readdir(dir)) != NULL) files.push_back(entry->d_name);
    }

    closedir(dir);

    for (int i = 0; i < files.size(); i++) {

        if (files[i] == "data_directory.dat") continue;

        string tree = files[i].substr(0, files[i].find("_"));
        string key = files[i].substr(files[i].find("_") + 1, files[i].find(".dat") - files[i].find("_") - 1);

        if (tree == "avl")
            if (!initializeAVLTree(dbptr, key)) return false;

        if (tree == "rb")
            if (!initializeRBTree(dbptr, key)) return false;

    }

    return true;

}

int get_coloumn_num(DatabasePointer& dbptr, string key_head) {

    int col_no = 0;
    vector<string> cells;

    fstream dataFile;
    // we can simply the first file, 0th file
    dataFile.open(dbptr.directory + "\\" + dbptr.fileNames[0], ios::in);

    if (!dataFile.is_open()) {
        cout << "File could not be opened. " << endl;
        return 0;
    }

    string line;

    //the first file line contains the headings and these headings are same in all the files
     //heading has not been extracted yet
    getline(dataFile, line);
    // the first line is stored in line variable
    // now process to get headings the column val (in which the heading(indexing) is)
    extract_row_cells(cells, line);

    for (int i = 0; i < cells.size(); i++) {
        if (cells[i] == key_head)
            break;
        col_no++;
    }

    cells.clear();
    dataFile.close();

    return col_no;

}

void extract_row_cells(vector<string>& words, string line) {

    string characters;
    bool backslash = false;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',' || line[i] == '\r') {
            if (characters.find('"') != string::npos && backslash == false) {
                // this means that ',' is within the cell because of presence of '\'
                characters += line[i];
                backslash = true; // we have found that string inside has commas so need to only store, when there is backslash before comma

            }
            else if (backslash == true) {
                if (line[i - 1] != '"') {
                    characters += line[i];
                }
                else {
                    words.push_back(characters);
                    characters.erase();
                    backslash = false;
                }
            }
            else {
                // no ',' in cell, store the word
                words.push_back(characters);
                characters.erase();
                backslash = false;
            }
        }
        else {
            characters += line[i];
        }
    }

    words.push_back(characters);

}