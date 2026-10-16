# Smart Inventory System (SIS)

A lightweight, terminal-based warehouse management system written in **C++**. SIS combines **Object-Oriented Programming** with hand-built **Data Structures & Algorithms**, with no STL containers such as `std::vector`, `std::unordered_map` or `std::queue`. Everything runs from the command line with no external database.

> **Project status:** Work in progress (~50% complete). The core data structures and class hierarchy are working. The interactive menu, full persistence and role-based access are still being built. See the [Roadmap](#roadmap).

---

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Example Run](#example-run)
- [Time Complexity](#time-complexity)
- [Current Status](#current-status)
- [Roadmap](#roadmap)
- [Known Issues](#known-issues)
- [Team](#team)

---

## Features

| Feature | Description | Status |
| --- | --- | --- |
| **O(1) product lookup** | Custom hash table with chaining, keyed by SKU | Working |
| **Restock priority tracking** | Custom min-heap ordered by stock quantity | Working (alerts being integrated) |
| **FIFO order processing** | Linked-list queue of customer orders | Working |
| **Polymorphic items** | Abstract `Item` base class with derived `PerishableItem` | Working |
| **Transaction logging** | Sales are appended to `sales_log.txt` | Working |
| **Interactive CLI menu** | Role-based Admin / Storekeeper dashboards | Planned |
| **Full state persistence** | Restore inventory from disk on startup | Planned |

## Architecture

The project follows an **MVC-style** layout. `InventoryManager` acts as a central **facade** that ties the custom data structures to user actions.

```
                 +----------------------+
                 |  main()  (driver/UI) |
                 +----------+-----------+
                            |
                 +----------v-----------+
                 |   InventoryManager   |   (facade / controller)
                 +--+--------+-------+--+
                    |        |       |
        +-----------v+  +----v-----+ +v-----------+
        | HashTable  |  | MinHeap  | | OrderQueue |
        | (SKU map)  |  | (restock)| | (FIFO)     |
        +-----+------+  +----+-----+ +------------+
              |              |
              +------+-------+
                     v
               +-----------+            +-------------+
               |   Item    |<-----------|  FileManager|
               | (abstract)|            | (sales log) |
               +-----+-----+            +-------------+
                     |
             +-------v--------+
             | PerishableItem |
             +----------------+
```

### Core components

- **`Item`** (abstract base class): holds SKU, name, quantity and base price. Declares the pure virtual methods `calculateTax()` and `needsRestock()`.
- **`PerishableItem`**: derives from `Item`, adds an expiration date, applies a 5% tax rate and flags restock when quantity drops below 20.
- **`HashTable`**: a raw dynamic array of node pointers (`Node** table`) with **separate chaining** for collision resolution. The destructor frees every chain node to avoid leaks.
- **`MinHeap`**: a raw dynamic array (`Item** heap`) ordered by quantity, using manual `heapifyUp`. The item with the lowest stock sits at the top via `peekMin()`.
- **`OrderQueue`**: a singly linked list (`OrderNode`) with explicit `frontNode` / `rearNode` pointers for FIFO order handling.
- **`InventoryManager`**: the facade that adds items, queues orders, validates stock, processes orders and pushes low-stock items into the restock heap.
- **`FileManager`**: uses `<fstream>` to append each completed sale to `sales_log.txt`.

## Project Structure

```
Smart-Inventory-System-SIS-/
├── main.cpp              # Driver: sets up the manager and runs a demo order
├── item.h                # Abstract base class Item
├── perishable_item.h     # PerishableItem (derived from Item)
├── hashtable.h           # Custom hash table with chaining
├── MinHeap.h             # Custom min-heap priority queue
├── OrderQueue.h          # Custom linked-list FIFO queue
├── inventory_manager.h   # Facade connecting all components
└── filemanager.h         # Transaction log persistence
```

## Getting Started

### Prerequisites

- A C++ compiler with C++11 support or later (e.g. `g++`)
- Git

### Clone

```bash
git clone https://github.com/karaniron08/Smart-Inventory-System-SIS-.git
cd Smart-Inventory-System-SIS-
```

### Build and run

```bash
g++ -std=c++11 main.cpp -o sis
./sis          # Linux / macOS
sis.exe        # Windows
```

> A `Makefile` is planned but not yet included.
> **Linux / macOS users:** the `#include` statements use capitalised names (e.g. `"Item.h"`) while some files are lowercase (`item.h`). Windows ignores this, but Linux filesystems are case-sensitive. See [Known Issues](#known-issues).

## Example Run

`main.cpp` currently runs a hard-coded demo: it adds one perishable item, queues an order for 40 units and processes it.

```
[System] Added Item: Fuji Apples

--- Smart Inventory CLI ---
Staff Member entering order...
[System] Order Queued for SKU: SKU100

[Audit] Final Stock for SKU100: 10 (Should trigger restock)
```

`sales_log.txt` is created or appended to with:

```
SKU: SKU100 | Qty Sold: 40
```

## Time Complexity

| Operation | Structure | Complexity |
| --- | --- | --- |
| Insert item | `HashTable` | O(1) average |
| Search by SKU | `HashTable` | O(1) average |
| Insert into restock queue | `MinHeap` | O(log N) |
| Peek lowest-stock item | `MinHeap` | O(1) |
| Enqueue order | `OrderQueue` | O(1) |
| Dequeue order | `OrderQueue` | O(1) |

## Current Status

**Done**

- Custom `HashTable`, `MinHeap` and `OrderQueue` without STL containers
- OOP hierarchy (`Item` and `PerishableItem`) with polymorphism
- `InventoryManager` facade for adding items and processing orders
- Transaction logging to `sales_log.txt`
- Destructors and cleanup to manage raw memory

**In progress**

- Automatic low-stock console alerts wired into the main workflow
- Formatted console output for SKU search and transaction history
- Project report with UML diagrams and flowcharts

## Roadmap

- [ ] Dynamic hash table resizing and automatic rehashing
- [ ] Safe node deletion in the hash table (item removal)
- [ ] `extractMin()` for `MinHeap` so restocked items can be removed
- [ ] Error handling for empty-queue states under high volume
- [ ] More `Item` subclasses for other product categories
- [ ] Item deletion and a full system audit in `InventoryManager`
- [ ] Load inventory and logs from disk on startup (`FileManager`)
- [ ] Binary `.dat` storage for full state persistence
- [ ] Interactive menu loop replacing the hard-coded demo in `main()`
- [ ] Role-based dashboards (Admin vs Storekeeper) with user authentication
- [ ] `Makefile` for automated builds
- [ ] UML diagrams and final project report

## Known Issues

- **Include case mismatch:** headers are included as `Item.h`, `HashTable.h`, `PerishableItem.h`, `InventoryManager.h` and `FileManager.h`, but the files are named in lowercase. This breaks the build on case-sensitive systems (Linux). Renaming the files or the includes so they match fixes it.
- **`hashtable.h` is missing its closing `#endif`** for the `HASHTABLE_H` include guard, which causes a compile error until it is added.
- **`MinHeap::insert` silently drops items** once its capacity (default 100) is reached.
- **Duplicate heap entries:** an item can be inserted into the restock heap more than once, since there is no removal or duplicate check yet.
- **No input validation or authentication** yet: the system is driven by a fixed demo script.

## Team

Built as a Phase 2 Project-Based Learning (PBL) submission for **B.Tech CSE**.

| Member | Role |
| --- | --- |
| **Karan Duhan** (Team Lead) | Search & Hash Table Specialist (DSA track) |
| **Gauri Thapliyal** | Queue & Heap Specialist (DSA track) |
| **Prince Kumar** | Domain Modeling & Master Controller Lead (OOP track) |
| **Kartik Jadli** | User Auth, Persistence & UI Architect (OOP track) |

## License

No license has been specified yet. Add a `LICENSE` file if you plan to open-source the project.
