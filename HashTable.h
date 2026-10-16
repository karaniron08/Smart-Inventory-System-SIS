#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "Item.h"

// =====================================================================
// MEMBER 1: SEARCH & HASH TABLE SPECIALIST (DSA TRACK)
// Responsibilities: O(1) custom Hash Map using dynamic arrays & chaining.
// =====================================================================

class HashTable {
private:
    struct Node { 
        std::string key; 
        Item* value; 
        Node* next; 
        Node(std::string k, Item* v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int capacity;

    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % capacity;
        }
        return hash;
    }

public:
    HashTable(int cap = 100) : capacity(cap) {
        table = new Node*[capacity](); 
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    void insert(const std::string& key, Item* item) {
        int index = hashFunction(key);
        Node* newNode = new Node(key, item);
        newNode->next = table[index];
        table[index] = newNode;
    }

    Item* search(const std::string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) return current->value;
            current = current->next;
        }
        return nullptr;
    }
};

#endif