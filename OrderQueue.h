// Member 2: FIFO Order Queue & LIFO Undo Stack Module
#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include <string>
#include <iostream>

using namespace std;

// ===== FIFO Order Queue =====

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

    void displayPendingOrders() {
        if (!hasOrders()) {
            cout << "[Info] No pending orders in the queue." << endl;
            return;
        }
        cout << "\n===== PENDING ORDERS (FIFO Queue) =====" << endl;
        OrderNode* current = frontNode;
        int position = 1;
        while (current != nullptr) {
            cout << "  " << position << ". SKU: " << current->data.sku
                 << " | Qty Requested: " << current->data.requestedQty << endl;
            current = current->next;
            position++;
        }
        cout << "=======================================\n" << endl;
    }
};

// ===== LIFO Undo Stack =====

struct UndoAction {
    string sku;
    int quantityDelta;   // the amount that was added (positive) or removed (negative)
    string actionType;   // "RESTOCK", "ORDER_SOLD", etc.
};

struct UndoNode {
    UndoAction data;
    UndoNode* next;
    UndoNode(UndoAction a) : data(a), next(nullptr) {}
};

class UndoStack {
private:
    UndoNode* topNode;

public:
    UndoStack() : topNode(nullptr) {}

    ~UndoStack() {
        while (!isEmpty()) pop();
    }

    void push(string sku, int delta, string actionType) {
        UndoNode* newNode = new UndoNode({sku, delta, actionType});
        newNode->next = topNode;
        topNode = newNode;
    }

    UndoAction pop() {
        if (topNode == nullptr) return {"", 0, ""};
        UndoNode* temp = topNode;
        UndoAction data = temp->data;
        topNode = topNode->next;
        delete temp;
        return data;
    }

    UndoAction peek() const {
        if (topNode == nullptr) return {"", 0, ""};
        return topNode->data;
    }

    bool isEmpty() const { return topNode == nullptr; }
};

#endif
