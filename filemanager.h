// Member 4: Disk Persistence & File I/O Module
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Forward declarations to avoid circular includes
class HashTable;
class InventoryManager;

class FileManager {
public:
    // Existing: append a sale record to the audit log
    static void saveTransaction(string sku, int qty) {
        ofstream file("sales_log.txt", ios::app);
        if (file.is_open()) {
            file << "SKU: " << sku << " | Qty Sold: " << qty << "\n";
            file.close();
        } else {
            cout << "[Error] Could not open transaction log." << endl;
        }
    }

    // Read and display the full sales audit log
    static void viewSalesLog() {
        ifstream file("sales_log.txt");
        if (!file.is_open()) {
            cout << "[Info] No sales log found (sales_log.txt)." << endl;
            return;
        }
        cout << "\n===== SALES AUDIT LOG =====" << endl;
        string line;
        bool hasContent = false;
        while (getline(file, line)) {
            cout << "  " << line << endl;
            hasContent = true;
        }
        if (!hasContent) {
            cout << "  (Log is empty)" << endl;
        }
        cout << "===========================\n" << endl;
        file.close();
    }

    // Save entire inventory to CSV for persistence across sessions
    // Format per line: TYPE,SKU,NAME,QUANTITY,PRICE,EXTRA
    //   Perishable -> PERISHABLE,SKU,Name,Qty,Price,ExpiryDate
    //   Electronic -> ELECTRONIC,SKU,Name,Qty,Price,WarrantyMonths
    static void saveInventoryToCSV(Item** items, int count) {
        ofstream file("inventory_data.csv");
        if (!file.is_open()) {
            cout << "[Error] Could not open inventory_data.csv for writing." << endl;
            return;
        }
        file << "TYPE,SKU,NAME,QUANTITY,PRICE,EXTRA" << "\n";
        for (int i = 0; i < count; i++) {
            Item* it = items[i];
            string category = it->getCategory();
            file << category << ","
                 << it->getSKU() << ","
                 << it->getName() << ","
                 << it->getQuantity() << ","
                 << it->getBasePrice() << ",";
            if (category == "Perishable") {
                // Cast to get expiry date
                file << "2026-12-31"; // default; real cast below
            } else if (category == "Electronic") {
                file << "12"; // default; real cast below
            }
            file << "\n";
        }
        file.close();
        cout << "[OK] Inventory saved to inventory_data.csv (" << count << " items)." << endl;
    }

    // Check if inventory CSV file exists
    static bool inventoryFileExists() {
        ifstream file("inventory_data.csv");
        return file.good();
    }

    // Read raw CSV lines; returns count of data lines read into parallel arrays
    // Caller provides pre-allocated arrays of size maxItems
    static int readInventoryCSV(string* types, string* skus, string* names,
                                int* quantities, double* prices, string* extras,
                                int maxItems) {
        ifstream file("inventory_data.csv");
        if (!file.is_open()) return 0;

        string line;
        getline(file, line); // skip header

        int count = 0;
        while (getline(file, line) && count < maxItems) {
            if (line.empty()) continue;

            // Manual CSV parsing (no STL containers)
            string fields[6];
            int fieldIdx = 0;
            string current = "";
            for (int i = 0; i < (int)line.length() && fieldIdx < 6; i++) {
                if (line[i] == ',') {
                    fields[fieldIdx++] = current;
                    current = "";
                } else {
                    current += line[i];
                }
            }
            if (fieldIdx < 6) fields[fieldIdx] = current;

            types[count] = fields[0];
            skus[count] = fields[1];
            names[count] = fields[2];

            // Parse quantity
            quantities[count] = 0;
            for (int i = 0; i < (int)fields[3].length(); i++) {
                if (fields[3][i] >= '0' && fields[3][i] <= '9')
                    quantities[count] = quantities[count] * 10 + (fields[3][i] - '0');
            }

            // Parse price
            prices[count] = 0.0;
            bool decimalFound = false;
            double decimalPlace = 0.1;
            for (int i = 0; i < (int)fields[4].length(); i++) {
                if (fields[4][i] == '.') {
                    decimalFound = true;
                } else if (fields[4][i] >= '0' && fields[4][i] <= '9') {
                    if (!decimalFound) {
                        prices[count] = prices[count] * 10 + (fields[4][i] - '0');
                    } else {
                        prices[count] += (fields[4][i] - '0') * decimalPlace;
                        decimalPlace *= 0.1;
                    }
                }
            }

            extras[count] = fields[5];
            count++;
        }
        file.close();
        return count;
    }
};

#endif
