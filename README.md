# CacheHash

**CacheHash** is a cache-friendly hash table implementation written entirely from scratch in modern C++. The project was built to understand how high-performance hash tables work internally by implementing every major component manually instead of relying on `std::unordered_map`.

The repository currently contains two open addressing collision resolution strategies:

* **Linear Probing**
* **Robin Hood Hashing**

Both implementations share the same core hash table interface while demonstrating different approaches to collision resolution, deletion, and performance optimization.

---

## Features

* Generic template-based implementation
* Open addressing
* Linear probing
* Robin Hood hashing
* Dynamic rehashing
* Configurable load factor
* Duplicate key updates
* Backward Shift Deletion (Robin Hood)
* Tombstone deletion (Linear Probing)
* Cache-friendly contiguous storage using `std::vector`

---

## Project Structure

### Linear Probing

The linear probing implementation resolves collisions by sequentially probing the next available bucket until an empty slot is found.

Features:

* Sequential probing
* Tombstone-based deletion
* Automatic rehashing
* Duplicate key updates

Although simple to implement, long probe chains can develop as the table becomes full, increasing lookup times.

---

### Robin Hood Hashing

Robin Hood hashing improves probe length distribution by ensuring elements that have traveled farther from their ideal bucket receive priority during insertion.

Instead of allowing early elements to monopolize nearby buckets, entries with larger probe distances "steal" positions from entries that have traveled a shorter distance.

This significantly reduces probe length variance and provides more predictable lookup performance.

Implemented features:

* Probe distance tracking
* Robin Hood insertion
* Reinsertion-based rehashing
* Backward Shift Deletion
* Duplicate key updates

---

## Backward Shift Deletion

Unlike traditional open addressing implementations that rely on tombstones, the Robin Hood implementation uses **Backward Shift Deletion**.

When an element is removed:

1. The deleted bucket becomes an empty hole.
2. Following elements with a probe distance greater than zero are shifted backward.
3. Their probe distance is decremented after every shift.
4. The process stops when an empty bucket or an element in its home position is encountered.

This completely removes the need for tombstones while preserving correct search behavior and maintaining probe sequence continuity.

---

## Rehashing

Whenever the load factor exceeds the configured threshold, the table automatically grows.

Instead of copying buckets directly into the new table, every occupied element is reinserted using the appropriate insertion algorithm.

This is required because changing the table capacity changes every bucket's ideal position, making direct copying incorrect.

---

## Public Interface

The hash table currently supports:

* Insert
* Find
* Contains
* Erase
* Clear
* Size
* Capacity
* Load Factor
* Automatic Rehashing

---

## Data Structures Used

* `std::vector`
* Template classes
* Custom bucket structure
* `std::hash`
* Move semantics
* Enumerations for bucket state management

---

## Concepts Explored

This project focuses on practical implementation of:

* Hash Tables
* Open Addressing
* Linear Probing
* Robin Hood Hashing
* Probe Distance Management
* Collision Resolution
* Dynamic Rehashing
* Backward Shift Deletion
* Cache-Friendly Data Structures
* Template Programming
* Move Semantics
* Undefined Behavior Debugging
* Separation of Algorithms from Storage

---

## Future Improvements

* Custom hash functors
* Iterator support
* Custom allocators
* Exception safety guarantees
* SIMD-friendly bucket layout
* Benchmark suite
* Performance comparison with `std::unordered_map`
* Support for heterogeneous lookup

---

## Why This Project?

The primary goal of CacheHash was not simply to create another hash table, but to understand the design decisions behind modern hash map implementations.

Rather than treating the hash table as a black box, every major algorithm was implemented manually, including insertion, deletion, probing, collision handling, and rehashing. The project provided hands-on experience with algorithm design, cache-aware data structures, debugging undefined behavior, and maintaining correctness through comprehensive testing.

---

## License

This project is intended for educational purposes and as a systems programming learning project.
