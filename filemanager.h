#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileManager {
public:
    static void saveTransaction(string sku, int qty) {
        ofstream file("sales_log.txt", ios::app);
        if (file.is_open()) {
            file << "SKU: " << sku << " | Qty Sold: " << qty << "\n";
            file.close();
        } else {
            cout << "[Error] Could not open transaction log.\n";
        }
    }
};

#endif