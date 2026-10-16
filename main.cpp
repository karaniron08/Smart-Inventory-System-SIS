#include <iostream>
#include "PerishableItem.h"
#include "InventoryManager.h"

using namespace std;

int main() {
    InventoryManager systemManager;

    Item* apple = new PerishableItem("SKU100", "Fuji Apples", 50, 1.99, "2026-10-30");
    systemManager.addItem(apple);

    cout << "\n--- Smart Inventory CLI ---" << endl;
    cout << "Staff Member entering order..." << endl;
    
    systemManager.queueCustomerOrder("SKU100", 40);
    systemManager.processPendingOrders();

    cout << "\n[Audit] Final Stock for SKU100: " 
         << apple->getQuantity() << " (Should trigger restock)\n";

    delete apple;
    return 0;
}