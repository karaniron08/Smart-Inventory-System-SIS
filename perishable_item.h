// Member 3: Derived Item Subclasses & Business Logic
#ifndef PERISHABLE_ITEM_H
#define PERISHABLE_ITEM_H

#include "item.h"
#include <string>
#include <iostream>

using namespace std;


class PerishableItem : public Item {
private:
    string expiryDate;

public:
    PerishableItem(string s, string n, int q, double p, string exp)
        : Item(s, n, q, p), expiryDate(exp) {}

    double calculateTax() const override {
        return basePrice * 0.05; 
    }

    bool needsRestock() const override {
        return quantity < 20; 
    }

    string getCategory() const override {
        return "Perishable";
    }

    string getExpiryDate() const { return expiryDate; }

    void displayDetails() const override {
        cout << "+-----------------------------------------+" << endl;
        cout << "| SKU       : " << sku << endl;
        cout << "| Name      : " << name << endl;
        cout << "| Category  : Perishable" << endl;
        cout << "| Quantity  : " << quantity << endl;
        cout << "| Base Price: $" << basePrice << endl;
        cout << "| Tax (5%)  : $" << calculateTax() << endl;
        cout << "| Expiry    : " << expiryDate << endl;
        cout << "| Restock?  : " << (needsRestock() ? "YES (< 20)" : "No") << endl;
        cout << "+-----------------------------------------+" << endl;
    }
};


class ElectronicItem : public Item {
private:
    int warrantyMonths;

public:
    ElectronicItem(string s, string n, int q, double p, int warranty)
        : Item(s, n, q, p), warrantyMonths(warranty) {}

    double calculateTax() const override {
        return basePrice * 0.18; 
    }

    bool needsRestock() const override {
        return quantity < 10; 
    }

    string getCategory() const override {
        return "Electronic";
    }

    int getWarrantyMonths() const { return warrantyMonths; }

    void displayDetails() const override {
        cout << "+-----------------------------------------+" << endl;
        cout << "| SKU       : " << sku << endl;
        cout << "| Name      : " << name << endl;
        cout << "| Category  : Electronic" << endl;
        cout << "| Quantity  : " << quantity << endl;
        cout << "| Base Price: $" << basePrice << endl;
        cout << "| Tax (18%) : $" << calculateTax() << endl;
        cout << "| Warranty  : " << warrantyMonths << " months" << endl;
        cout << "| Restock?  : " << (needsRestock() ? "YES (< 10)" : "No") << endl;
        cout << "+-----------------------------------------+" << endl;
    }
};

#endif
