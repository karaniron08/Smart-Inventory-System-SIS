// Member 2: Priority Queue & Min-Heap Module
#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
#include "item.h"

using namespace std;

class MinHeap {
private:
    Item** heap;
    int capacity;
    int size;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index]->getQuantity() >= heap[parent]->getQuantity()) break;

            Item* temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;

            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left]->getQuantity() < heap[smallest]->getQuantity())
                smallest = left;
            if (right < size && heap[right]->getQuantity() < heap[smallest]->getQuantity())
                smallest = right;

            if (smallest == index) break;

            Item* temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;

            index = smallest;
        }
    }

public:
    MinHeap(int cap = 100) : capacity(cap), size(0) {
        heap = new Item*[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    void insert(Item* item) {
        if (size == capacity) return;
        for (int i = 0; i < size; i++) {
            if (heap[i]->getSKU() == item->getSKU()) return;
        }
        heap[size] = item;
        heapifyUp(size);
        size++;
    }

    Item* peekMin() const {
        if (size == 0) return nullptr;
        return heap[0];
    }

    Item* extractMin() {
        if (size == 0) return nullptr;
        Item* minItem = heap[0];
        heap[0] = heap[size - 1];
        size--;
        if (size > 0) heapifyDown(0);
        return minItem;
    }

    int getSize() const { return size; }

    void rebuildHeap() {
        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void removeItem(const string& sku) {
        for (int i = 0; i < size; i++) {
            if (heap[i]->getSKU() == sku) {
                heap[i] = heap[size - 1];
                size--;
                if (size > 0) {
                    heapifyDown(i);
                    heapifyUp(i);
                }
                return;
            }
        }
    }

    void updateItem(Item* item) {
        removeItem(item->getSKU());
        insert(item);
    }

    void displayRestockAlerts() {
        if (size == 0) {
            cout << "[Info] No items in the restock priority queue." << endl;
            return;
        }
        cout << "\n===== LOW-STOCK RESTOCK ALERTS (Min-Heap Order) =====" << endl;
        Item** tempHeap = new Item*[size];
        int tempSize = size;
        for (int i = 0; i < size; i++) tempHeap[i] = heap[i];

        bool anyAlert = false;
        for (int i = 0; i < tempSize; i++) {
            if (tempHeap[i]->needsRestock()) {
                cout << "  [!] " << tempHeap[i]->getSKU()
                     << " | " << tempHeap[i]->getName()
                     << " | Stock: " << tempHeap[i]->getQuantity()
                     << " | Category: " << tempHeap[i]->getCategory() << endl;
                anyAlert = true;
            }
        }
        if (!anyAlert) {
            cout << "[Info] All items are sufficiently stocked." << endl;
        }
        delete[] tempHeap;
        cout << "====================================================\n" << endl;
    }
};

#endif
