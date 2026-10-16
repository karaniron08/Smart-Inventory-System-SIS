#ifndef MINHEAP_H
#define MINHEAP_H

#include "Item.h"

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

public:
    MinHeap(int cap = 100) : capacity(cap), size(0) {
        heap = new Item*[capacity];
    }
    
    ~MinHeap() {
        delete[] heap;
    }

    void insert(Item* item) {
        if (size == capacity) return;
        heap[size] = item;
        heapifyUp(size);
        size++;
    }

    Item* peekMin() const {
        if (size == 0) return nullptr;
        return heap[0];
    }
};

#endif