#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
protected:
    std::string sku;
    std::string name;
    int quantity;
    double basePrice;

public:
    Item(std::string s, std::string n, int q, double p) 
        : sku(s), name(n), quantity(q), basePrice(p) {}
    virtual ~Item() {}

    virtual double calculateTax() const = 0; 
    virtual bool needsRestock() const = 0;

    std::string getSKU() const { return sku; }
    std::string getName() const { return name; }
    int getQuantity() const { return quantity; }
    
    void updateQuantity(int amount) { quantity += amount; }
};

#endif