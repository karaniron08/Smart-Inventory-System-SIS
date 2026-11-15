// Member 2: FIFO Order Queue & LIFO Undo Stack Module
#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include <string>
#include <iostream>

using namespace std;

struct OrderNode {
    string sku;
    int quantity;
    OrderNode* next;

    OrderNode(string s, int q) : sku(s), quantity(q), next(nullptr) {}
};

class OrderQueue {
private:
    OrderNode* frontNode;
    OrderNode* rearNode;

public:
    OrderQueue() : frontNode(nullptr), rearNode(nullptr) {}

    ~OrderQueue() {
        string s;
        int q;
        while (!isEmpty()) {
            dequeue(s, q);
        }
    }

    void enqueue(string sku, int qty) {
        OrderNode* newNode = new OrderNode(sku, qty);
        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
            return;
        }
        rearNode->next = newNode;
        rearNode = newNode;
    }

    bool dequeue(string& sku, int& qty) {
        if (isEmpty()) return false;
        OrderNode* temp = frontNode;
        sku = temp->sku;
        qty = temp->quantity;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        delete temp;
        return true;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }
};

struct UndoAction {
    string sku;
    int quantityDelta;
    string actionType;
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
