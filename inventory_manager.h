// Member 3: Master Controller Facade
#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <iostream>
#include <string>
#include "item.h"
#include "perishable_item.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "OrderQueue.h"
#include "FileManager.h"


using namespace std;

class InventoryManager {
private:
    HashTable inventoryMap;
    MinHeap restockHeap;
    OrderQueue orderQueue;
    UndoStack undoStack;

    
    static const int GRID_ROWS = 8;
    static const int GRID_COLS = 8;

public:
    InventoryManager() {}

    void addItem(Item* item) {
        inventoryMap.insert(item->getSKU(), item);
        restockHeap.insert(item);
        cout << "[OK] Item added: " << item->getSKU()
             << " (" << item->getName() << ")" << endl;
    }

    void queueCustomerOrder(string sku, int qty) {
        orderQueue.enqueueOrder(sku, qty);
        cout << "[OK] Order queued -> SKU: " << sku << ", Qty: " << qty << endl;
    }

    void processPendingOrders() {
        if (!orderQueue.hasOrders()) {
            cout << "[Info] No pending orders to process." << endl;
            return;
        }
        cout << "\n--- Processing Pending Orders ---" << endl;
        while (orderQueue.hasOrders()) {
            Order order = orderQueue.dequeueOrder();
            Item* item = inventoryMap.search(order.sku);
            if (item == nullptr) {
                cout << "[Error] SKU " << order.sku << " not found in inventory." << endl;
                continue;
            }
            if (item->getQuantity() < order.requestedQty) {
                cout << "[Error] Insufficient stock for " << order.sku
                     << " (Have: " << item->getQuantity()
                     << ", Need: " << order.requestedQty << ")" << endl;
                continue;
            }
            item->updateQuantity(-order.requestedQty);
            undoStack.push(order.sku, -order.requestedQty, "ORDER_SOLD");
            FileManager::saveTransaction(order.sku, order.requestedQty);
            restockHeap.updateItem(item);
            cout << "[OK] Sold " << order.requestedQty << " x " << item->getName()
                 << " | Remaining: " << item->getQuantity() << endl;
            if (item->needsRestock()) {
                cout << "  [!] RESTOCK ALERT: " << item->getName()
                     << " is below threshold!" << endl;
            }
        }
        cout << "---------------------------------\n" << endl;
    }

    

    void searchAndDisplayItem(string sku) {
        Item* item = inventoryMap.search(sku);
        if (item == nullptr) {
            cout << "[Error] Item with SKU '" << sku << "' not found." << endl;
            return;
        }
        cout << "\n--- Search Result (O(1) Hash Lookup) ---" << endl;
        item->displayDetails();
    }

    void restockItem(string sku, int addedQty) {
        Item* item = inventoryMap.search(sku);
        if (item == nullptr) {
            cout << "[Error] SKU '" << sku << "' not found." << endl;
            return;
        }
        item->updateQuantity(addedQty);
        undoStack.push(sku, addedQty, "RESTOCK");
        restockHeap.updateItem(item);
        cout << "[OK] Restocked " << item->getName()
             << " by +" << addedQty
             << " | New Stock: " << item->getQuantity() << endl;
    }

    void undoLastOperation() {
        if (undoStack.isEmpty()) {
            cout << "[Info] Nothing to undo." << endl;
            return;
        }
        UndoAction action = undoStack.pop();
        Item* item = inventoryMap.search(action.sku);
        if (item == nullptr) {
            cout << "[Error] Cannot undo: item " << action.sku
                 << " no longer exists." << endl;
            return;
        }
        
        int reverseDelta = -action.quantityDelta;
        
        item->updateQuantity(reverseDelta);
        restockHeap.updateItem(item);
        cout << "[OK] UNDO successful -> Action: " << action.actionType
             << " | SKU: " << action.sku
             << " | Reversed by: " << reverseDelta
             << " | New Stock: " << item->getQuantity() << endl;
    }

    void displayFullInventory() {
        cout << "\n===== FULL INVENTORY LISTING =====" << endl;
        inventoryMap.displayAll();
        cout << "==================================\n" << endl;
    }

    void displayLowStockAlerts() {
        restockHeap.displayRestockAlerts();
    }
    void findOptimalPickingRoute(int targetRow, int targetCol) {
        if (targetRow < 0 || targetRow >= GRID_ROWS ||
            targetCol < 0 || targetCol >= GRID_COLS) {
            cout << "[Error] Target (" << targetRow << "," << targetCol
                 << ") is outside the " << GRID_ROWS << "x" << GRID_COLS
                 << " warehouse grid." << endl;
            return;
        }

        cout << "\n===== WAREHOUSE BFS SHORTEST PATH =====" << endl;
        cout << "Grid: " << GRID_ROWS << " x " << GRID_COLS
             << " | From: (0,0) -> To: (" << targetRow << "," << targetCol << ")" << endl;

        bool visited[GRID_ROWS][GRID_COLS];
        int parentRow[GRID_ROWS][GRID_COLS];
        int parentCol[GRID_ROWS][GRID_COLS];
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                visited[i][j] = false;
                parentRow[i][j] = -1;
                parentCol[i][j] = -1;
            }
        }
        int maxQueueSize = GRID_ROWS * GRID_COLS;
        int* queueR = new int[maxQueueSize];
        int* queueC = new int[maxQueueSize];
        int front = 0, rear = 0;

        
        visited[0][0] = true;
        queueR[rear] = 0;
        queueC[rear] = 0;
        rear++;

        int dr[] = {-1, 1, 0, 0}; 
        int dc[] = {0, 0, -1, 1};
        bool found = false;

        while (front < rear) {
            int r = queueR[front];
            int c = queueC[front];
            front++;

            if (r == targetRow && c == targetCol) {
                found = true;
                break;
            }

            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS
                    && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    parentRow[nr][nc] = r;
                    parentCol[nr][nc] = c;
                    queueR[rear] = nr;
                    queueC[rear] = nc;
                    rear++;
                }
            }
        }

        if (found) {
            int pathR[GRID_ROWS * GRID_COLS];
            int pathC[GRID_ROWS * GRID_COLS];
            int pathLen = 0;
            int cr = targetRow, cc = targetCol;
            while (cr != 0 || cc != 0) {
                pathR[pathLen] = cr;
                pathC[pathLen] = cc;
                pathLen++;
                int pr = parentRow[cr][cc];
                int pc = parentCol[cr][cc];
                cr = pr;
                cc = pc;
            }
            pathR[pathLen] = 0;
            pathC[pathLen] = 0;
            pathLen++;

            cout << "Optimal Route (" << (pathLen - 1) << " steps):" << endl;
            for (int i = pathLen - 1; i >= 0; i--) {
                cout << "  -> (" << pathR[i] << ", " << pathC[i] << ")";
                if (i == pathLen - 1) cout << "  [Dispatch Dock]";
                if (i == 0) cout << "  [Item Shelf]";
                cout << endl;
            }
        } else {
            cout << "[Error] No path found to target." << endl;
        }

        delete[] queueR;
        delete[] queueC;
        cout << "========================================\n" << endl;
    }

    
    void saveSystemState() {
        Item* items[1000];
        int count = inventoryMap.getAllItems(items, 1000);

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
                PerishableItem* pi = dynamic_cast<PerishableItem*>(it);
                if (pi) file << pi->getExpiryDate();
                else file << "N/A";
            } else if (category == "Electronic") {
                ElectronicItem* ei = dynamic_cast<ElectronicItem*>(it);
                if (ei) file << ei->getWarrantyMonths();
                else file << "0";
            } else {
                file << "N/A";
            }
            file << "\n";
        }
        file.close();
        cout << "[OK] Inventory saved to inventory_data.csv (" << count << " items)." << endl;
    }

    
    void loadSystemState() {
        const int MAX_ITEMS = 500;
        string types[MAX_ITEMS], skus[MAX_ITEMS], names[MAX_ITEMS], extras[MAX_ITEMS];
        int quantities[MAX_ITEMS];
        double prices[MAX_ITEMS];

        int count = FileManager::readInventoryCSV(types, skus, names,
                                                   quantities, prices, extras, MAX_ITEMS);
        if (count == 0) {
            cout << "[Info] No saved inventory data found." << endl;
            return;
        }

        for (int i = 0; i < count; i++) {
            Item* item = nullptr;
            if (types[i] == "Perishable") {
                item = new PerishableItem(skus[i], names[i], quantities[i],
                                          prices[i], extras[i]);
            } else if (types[i] == "Electronic") {
                
                int warranty = 0;
                for (int j = 0; j < (int)extras[i].length(); j++) {
                    if (extras[i][j] >= '0' && extras[i][j] <= '9')
                        warranty = warranty * 10 + (extras[i][j] - '0');
                }
                item = new ElectronicItem(skus[i], names[i], quantities[i],
                                          prices[i], warranty);
            }
            if (item != nullptr) {
                addItem(item);
            }
        }
        cout << "[OK] Loaded " << count << " items from inventory_data.csv." << endl;
    }

    
    HashTable& getInventoryMap() { return inventoryMap; }
};

#endif
