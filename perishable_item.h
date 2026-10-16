#ifndef PERISHABLEITEM_H
#define PERISHABLEITEM_H

#include <string>
#include "Item.h"

using namespace std;

class PerishableItem : public Item {
private:
    string expirationDate;

public:
PerishableItem(string s, string n, int q, double p, string exp) 
    : Item(s, n, q, p), expirationDate(exp) {}

double calculateTax() const override {
    return basePrice * 0.05; 
    }

    bool needsRestock() const override {
    return quantity < 20; 
    }
};

#endif