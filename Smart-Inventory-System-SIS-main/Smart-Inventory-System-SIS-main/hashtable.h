// Member 1: Search & Hash Table Module
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>
#include "Item.h"

using namespace std;

class HashTable {
private:
    struct Node {
        string key;
        Item* value;
        Node* next;
        Node(string k, Item* v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int capacity;

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % capacity;
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
                delete temp->value; // free the Item* owned by this table
                delete temp;
            }
        }
        delete[] table;
    }

    // O(1) Average Insertion
    void insert(const string& key, Item* item) {
        int index = hashFunction(key);
        // Check if key already exists; update if so
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = item;
                return;
            }
            current = current->next;
        }
        Node* newNode = new Node(key, item);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // O(1) Average Search
    Item* search(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) return current->value;
            current = current->next;
        }
        return nullptr;
    }

    // O(1) Average Removal (does NOT delete the Item* itself)
    bool remove(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr)
                    table[index] = current->next;
                else
                    prev->next = current->next;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // Display every item stored in the table
    void displayAll() {
        bool found = false;
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                current->value->displayDetails();
                found = true;
                current = current->next;
            }
        }
        if (!found) {
            cout << "[Info] Inventory is empty." << endl;
        }
    }

    // Iterate helper: calls a function-pointer for every stored item (used by FileManager)
    void forEach(void (*callback)(Item*, void*), void* context) {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                callback(current->value, context);
                current = current->next;
            }
        }
    }

    // Collect all items into a caller-provided array; returns count
    int getAllItems(Item** outArray, int maxSize) {
        int count = 0;
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr && count < maxSize) {
                outArray[count++] = current->value;
                current = current->next;
            }
        }
        return count;
    }
};

#endif
