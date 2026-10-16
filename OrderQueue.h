#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include <string>

using namespace std;

struct Order {
    string sku;
    int requestedQty;
};

struct OrderNode {
    Order data;
    OrderNode* next;
    OrderNode(Order o) : data(o), next(nullptr) {}
};

class OrderQueue {
private:
    OrderNode* frontNode;
    OrderNode* rearNode;

public:
    OrderQueue() : frontNode(nullptr), rearNode(nullptr) {}
    
    ~OrderQueue() {
        while (hasOrders()) dequeueOrder();
    }

    void enqueueOrder(string sku, int qty) {
        OrderNode* newNode = new OrderNode({sku, qty});
        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
            return;
        }
        rearNode->next = newNode;
        rearNode = newNode;
    }

    bool hasOrders() { return frontNode != nullptr; }

    Order dequeueOrder() {
        if (frontNode == nullptr) return {"", 0};
        OrderNode* temp = frontNode;
        Order data = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr) rearNode = nullptr;
        delete temp;
        return data;
    }
};

#endif