// Member 3: Abstract Base Domain Model
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

using namespace std;

class Item {
protected:
    string sku;
    string name;
    int quantity;
    double basePrice;

public:
    Item(string s, string n, int q, double p)
        : sku(s), name(n), quantity(q), basePrice(p) {}
    virtual ~Item() {}

    virtual double calculateTax() const = 0;
    virtual bool needsRestock() const = 0;
    virtual string getCategory() const = 0;

    virtual void displayDetails() const {
        cout << "+-----------------------------------------+" << endl;
        cout << "| SKU       : " << sku << endl;
        cout << "| Name      : " << name << endl;
        cout << "| Quantity  : " << quantity << endl;
        cout << "| Base Price: $" << basePrice << endl;
        cout << "| Tax       : $" << calculateTax() << endl;
        cout << "| Category  : " << getCategory() << endl;
        cout << "| Restock?  : " << (needsRestock() ? "YES" : "No") << endl;
        cout << "+-----------------------------------------+" << endl;
    }

    string getSKU() const { return sku; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getBasePrice() const { return basePrice; }

    void updateQuantity(int amount) {
        if (quantity + amount < 0) {
            cout << "[Warning] Cannot reduce stock below 0. Current: "
                 << quantity << ", Attempted change: " << amount << endl;
            return;
        }
        quantity += amount;
    }

    bool operator<(const Item& other) const {
        return quantity < other.quantity;
    }
};

#endif
