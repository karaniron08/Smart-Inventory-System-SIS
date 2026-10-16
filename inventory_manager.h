#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <iostream>
#include <string>
#include "Item.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "OrderQueue.h"
#include "FileManager.h"

using namespace std;


class InventoryManager {
private:
    HashTable inventoryMap;   
    MinHeap restockQueue;       
    OrderQueue orderQueue;      

public:
    void addItem(Item* item) {
        inventoryMap.insert(item->getSKU(), item);
        if (item->needsRestock()) {
            restockQueue.insert(item);
        }
        cout << "[System] Added Item: " << item->getName() << endl;
    }

    void queueCustomerOrder(string sku, int qty) {
        orderQueue.enqueueOrder(sku, qty);
        cout << "[System] Order Queued for SKU: " << sku << endl;
    }

    void processPendingOrders() {
        while (orderQueue.hasOrders()) {
            Order curr = orderQueue.dequeueOrder();
            Item* target = inventoryMap.search(curr.sku);
            
            if (target != nullptr && target->getQuantity() >= curr.requestedQty) {
                target->updateQuantity(-curr.requestedQty); 
                FileManager::saveTransaction(curr.sku, curr.requestedQty);
                
                if (target->needsRestock()) {
                    restockQueue.insert(target);
                }
            } else {
                cout << "[Error] Insufficient stock or invalid SKU: " << curr.sku << endl;
            }
        }
    }
};

#endif
