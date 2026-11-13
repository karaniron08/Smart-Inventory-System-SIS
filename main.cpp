// Member 4: Interactive CLI & Application Entry Point
#include <iostream>
#include <string>
#include <limits>
#include "perishable_item.h"
#include "inventory_manager.h"

using namespace std;

// Helper: safely read an integer from cin (crash-proof)
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error] Invalid input. Please enter a number." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Helper: safely read a double from cin
double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error] Invalid input. Please enter a number." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Helper: read a string (allows spaces)
string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void printMenu() {
    cout << "\n============================================" << endl;
    cout << "   SMART INVENTORY SYSTEM (SIS) - Main Menu" << endl;
    cout << "============================================" << endl;
    cout << "  1. Add New Item (Perishable / Electronic)" << endl;
    cout << "  2. Search Item by SKU (O(1) Lookup)" << endl;
    cout << "  3. View Full Inventory" << endl;
    cout << "  4. Queue Customer Order (FIFO)" << endl;
    cout << "  5. Process Pending Orders" << endl;
    cout << "  6. View Low-Stock Restock Alerts (Min-Heap)" << endl;
    cout << "  7. Restock an Item" << endl;
    cout << "  8. Undo Last Stock Operation (LIFO Stack)" << endl;
    cout << "  9. Warehouse Picker Shortest Path (BFS)" << endl;
    cout << " 10. View Sales Log & Save Inventory to Disk" << endl;
    cout << "  0. Exit" << endl;
    cout << "============================================" << endl;
}

int main() {
    InventoryManager manager;

    // Try loading saved inventory from CSV
    if (FileManager::inventoryFileExists()) {
        cout << "[Info] Found inventory_data.csv. Loading saved state..." << endl;
        manager.loadSystemState();
    } else {
        // Pre-load sample items for demonstration
        cout << "[Info] No saved data. Loading sample items..." << endl;

        Item* apple = new PerishableItem("SKU100", "Fuji Apples", 50, 1.99, "2026-12-30");
        Item* milk = new PerishableItem("SKU101", "Organic Milk", 15, 3.49, "2026-11-20");
        Item* bread = new PerishableItem("SKU102", "Whole Wheat Bread", 30, 2.79, "2026-11-15");

        Item* laptop = new ElectronicItem("SKU200", "Ultrabook Laptop", 12, 899.99, 24);
        Item* charger = new ElectronicItem("SKU201", "USB-C Charger", 8, 29.99, 12);
        Item* headphones = new ElectronicItem("SKU202", "Wireless Headphones", 25, 79.99, 18);

        manager.addItem(apple);
        manager.addItem(milk);
        manager.addItem(bread);
        manager.addItem(laptop);
        manager.addItem(charger);
        manager.addItem(headphones);
    }

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Enter choice: ");

        switch (choice) {

        case 1: { // Add New Item
            cout << "\n--- Add New Item ---" << endl;
            int type = readInt("Item type (1 = Perishable, 2 = Electronic): ");
            if (type != 1 && type != 2) {
                cout << "[Error] Invalid type. Choose 1 or 2." << endl;
                break;
            }
            string sku = readLine("Enter SKU: ");
            string name = readLine("Enter Name: ");
            int qty = readInt("Enter Quantity: ");
            double price = readDouble("Enter Base Price: ");

            if (type == 1) {
                string expiry = readLine("Enter Expiry Date (YYYY-MM-DD): ");
                Item* item = new PerishableItem(sku, name, qty, price, expiry);
                manager.addItem(item);
            } else {
                int warranty = readInt("Enter Warranty (months): ");
                Item* item = new ElectronicItem(sku, name, qty, price, warranty);
                manager.addItem(item);
            }
            break;
        }

        case 2: { // Search by SKU
            string sku = readLine("Enter SKU to search: ");
            manager.searchAndDisplayItem(sku);
            break;
        }

        case 3: { // Full Inventory
            manager.displayFullInventory();
            break;
        }

        case 4: { // Queue Order
            string sku = readLine("Enter SKU for order: ");
            int qty = readInt("Enter quantity: ");
            manager.queueCustomerOrder(sku, qty);
            break;
        }

        case 5: { // Process Orders
            manager.processPendingOrders();
            break;
        }

        case 6: { // Low-Stock Alerts
            manager.displayLowStockAlerts();
            break;
        }

        case 7: { // Restock
            string sku = readLine("Enter SKU to restock: ");
            int qty = readInt("Enter quantity to add: ");
            manager.restockItem(sku, qty);
            break;
        }

        case 8: { // Undo
            manager.undoLastOperation();
            break;
        }

        case 9: { // BFS Path
            cout << "\n--- Warehouse Picker Route (BFS) ---" << endl;
            cout << "Warehouse grid is 8x8. Dispatch dock at (0,0)." << endl;
            int row = readInt("Enter target shelf row (0-7): ");
            int col = readInt("Enter target shelf col (0-7): ");
            manager.findOptimalPickingRoute(row, col);
            break;
        }

        case 10: { // Sales Log + Save
            FileManager::viewSalesLog();
            manager.saveSystemState();
            break;
        }

        case 0: { // Exit
            cout << "\n[Info] Saving inventory before exit..." << endl;
            manager.saveSystemState();
            cout << "[OK] Goodbye! Smart Inventory System shutting down." << endl;
            running = false;
            break;
        }

        default:
            cout << "[Error] Invalid choice. Please select 0-10." << endl;
            break;
        }
    }

    return 0;
}
