#ifndef ITEM_H
#define ITEM_H

#include <string>

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

    string getSKU() const { return sku; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    
    void updateQuantity(int amount) { quantity += amount; }
};

#endif
