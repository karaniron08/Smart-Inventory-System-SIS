#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "Item.h"
using namespace std;


// =====================================================================
// MEMBER 1: SEARCH & HASH TABLE SPECIALIST (DSA TRACK)
// Responsibilities: O(1) custom Hash Map using dynamic arrays & chaining.
// =====================================================================
class HashTable {
private:
    // Chaining method for collision resolution[cite: 1]
    struct Node { 
        string key; 
        Item* value; 
        Node* next; 
        Node(string k, Item* v) : key(k), value(v), next(nullptr) {}
    };

    Node** table; // Replaced vector with raw dynamic array of pointers
    int capacity;

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % capacity;
        return hash;
    }

public:
    HashTable(int cap = 100) : capacity(cap) {
        // Allocate and initialize array elements to nullptr
        table = new Node*[capacity](); 
    }

    ~HashTable() {
        // Memory Safety: Prevent memory leaks on teardown[cite: 1]
        for(int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // $O(1)$ Average Insertion
    void insert(const string& key, Item* item) {
        int index = hashFunction(key);
        Node* newNode = new Node(key, item);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // $O(1)$ Average Search[cite: 1]
    Item* search(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) return current->value;
            current = current->next;
        }
        return nullptr; // Not found
    }
};