# Smart Inventory System (SIS)

A lightweight, terminal-based warehouse management system written in **C++**. SIS combines **Object-Oriented Programming** with hand-built **Data Structures & Algorithms**, with no STL containers such as `std::vector`, `std::unordered_map` or `std::queue`. Everything runs from the command line with no external database.

> **Project status:** 100% Complete & Fully Functional.

---

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Example Run](#example-run)
- [Time Complexity](#time-complexity)
- [Team](#team)
- [License](#license)

---

## Features

| Feature | Description | Status |
| --- | --- | --- |
| **Interactive CLI menu** | Fully working role-based terminal UI (Admin / Storekeeper dashboards) | Working |
| **O(1) product lookup** | Custom hash table with chaining, keyed by SKU | Working |
| **Restock priority tracking** | Custom min-heap ordered by stock quantity | Working |
| **FIFO order processing** | Linked-list queue of customer orders | Working |
| **BFS warehouse pathfinding** | O(V+E) breadth-first search finds the optimal picking route on an 8x8 warehouse grid | Working |
| **LIFO undo stack** | Custom stack to undo the most recent inventory operations | Working |
| **CSV state persistence** | Automatically saves to and loads from `inventory_data.csv` using file handling | Working |
| **Extended polymorphism** | Abstract `Item` base with `PerishableItem` (5% tax, expiration dates) and `ElectronicItem` (18% tax, warranty tracking) | Working |
| **Transaction logging** | Sales are appended to `sales_log.txt` | Working |

## Architecture

The project follows an **MVC-style** layout. `InventoryManager` acts as a central **facade** that ties the custom data structures to user actions.

```
                 +----------------------+
                 |  main()  (CLI menu)  |
                 +----------+-----------+
                            |
                 +----------v-----------+
                 |   InventoryManager   |   (facade / controller)
                 +--+----+----+----+--+-+
                    |    |    |    |  |
      +-------------v+ +-v--------+ | +v-----------+  +-----------+
      |  HashTable   | | MinHeap  | | | OrderQueue |  | Undo Stack|
      |  (SKU map)   | | (restock)| | | (FIFO)     |  | (LIFO)    |
      +------+-------+ +----+-----+ | +------------+  +-----------+
             |              |       |
             +------+-------+       +--> BFS Pathfinder (8x8 grid)
                    v
              +-----------+              +--------------+
              |   Item    |<-------------| FileManager  |
              | (abstract)|              | (CSV + log)  |
              +-----+-----+              +--------------+
                    |
          +---------+----------+
          |                    |
+---------v------+   +---------v--------+
| PerishableItem |   |  ElectronicItem  |
| (5% tax)       |   | (18% tax)        |
+----------------+   +------------------+
```

### Core components

- **`Item`** (abstract base class): holds SKU, name, quantity and base price. Declares the pure virtual methods `calculateTax()` and `needsRestock()`.
- **`PerishableItem`**: derives from `Item`, adds an expiration date, applies a 5% tax rate and flags restock when quantity drops below 20.
- **`ElectronicItem`**: derives from `Item`, adds warranty tracking and applies an 18% tax rate.
- **`HashTable`**: a raw dynamic array of node pointers (`Node** table`) with **separate chaining** for collision resolution. The destructor frees every chain node to avoid leaks.
- **`MinHeap`**: a raw dynamic array (`Item** heap`) ordered by quantity. The item with the lowest stock sits at the top via `peekMin()`.
- **`OrderQueue`**: a singly linked list (`OrderNode`) with explicit `frontNode` / `rearNode` pointers for FIFO order handling.
- **Undo `Stack`**: a custom LIFO stack that records inventory operations so the most recent one can be reverted.
- **BFS pathfinder**: breadth-first search over an 8x8 warehouse grid to compute the shortest picking route.
- **`InventoryManager`**: the facade that adds and removes items, queues orders, validates stock, processes orders, triggers restock alerts, runs pathfinding and handles undo.
- **`FileManager`**: uses `<fstream>` to load and save the full inventory state to `inventory_data.csv` and to append each completed sale to `sales_log.txt`.

## Project Structure

```
Smart-Inventory-System-SIS-/
├── main.cpp              # Interactive role-based CLI driver
├── item.h                # Abstract base class Item
├── perishable_item.h     # PerishableItem (5% tax, expiration dates)
├── electronic_item.h     # ElectronicItem (18% tax, warranty tracking)
├── hashtable.h           # Custom hash table with chaining
├── MinHeap.h             # Custom min-heap priority queue
├── OrderQueue.h          # Custom linked-list FIFO queue and LIFO undo stack
├── inventory_manager.h   # Facade connecting all components (incl. BFS pathfinding)
├── filemanager.h         # CSV persistence and transaction log
├── inventory_data.csv    # Auto-generated saved inventory state
└── sales_log.txt         # Auto-generated transaction log
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

On first launch the system starts with an empty inventory. On every later launch it restores its state from `inventory_data.csv` automatically.

## Example Run

On startup, `main.cpp` loads saved data and presents the role-based menu. Illustrative session:

```
--- Smart Inventory CLI ---
Select role: 1) Admin  2) Storekeeper
> 2

[Storekeeper] 1) Add item  2) Queue order  3) Process order
              4) Search SKU  5) Find picking route  6) Undo last action
              7) Restock alerts  0) Save & exit
```

Completed sales are appended to `sales_log.txt`:

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
| Push / pop undo action | `Stack` | O(1) |
| Optimal picking route | BFS on warehouse grid | O(V + E) |
| Save / load inventory | CSV (`FileManager`) | O(N) |

## Team

Built as a Phase 2 Project-Based Learning (PBL) submission for **B.Tech CSE**.

| Member | Role | Module(s) |
| --- | --- | --- |
| **Gauri Thapliyal** | Search & Hash Table Module | `hashtable.h` |
| **Prince Narwar** | Priority Queues & LIFO Stack | `MinHeap.h`, `OrderQueue.h` |
| **Karan Duhan** | Abstract OOP Domain Model & Master Controller Facade | `item.h`, `perishable_item.h`, `inventory_manager.h` |
| **Kartik Jadli** | CSV Persistence & Interactive CLI Driver | `main.cpp`, `filemanager.h` |

## License

No license has been specified yet. Add a `LICENSE` file if you plan to open-source the project.
