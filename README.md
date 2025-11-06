# 🚀 Fast_NN_Search

A project to **implement**, **visualize**, and **compare** k-d trees and quadtrees for efficient storage and retrieval of point coordinates.

---

## 📌 Overview

This repository contains implementations of two spatial data structures:

- **k-d Tree** — a binary space partitioning structure optimized for k-dimensional searching (here, 2D)
- **Quadtree** — a hierarchical spatial index that recursively subdivides the plane into four quadrants

The goal is to efficiently store 2D point coordinates and perform **fast Nearest Neighbor (NN) searches**.

A **user interface (UI)** is provided to visualize how these structures partition space and handle queries. Benchmarking tools compare insertion, searching, and deletion time complexities to determine which structure performs best in different scenarios.

---

## 🌲 Features

✅ **K-D Tree** implementation (2D-specific)  
✅ **Quadtree** implementation for spatial indexing  
✅ **Interactive UI** to visualize partitioning, insertion, and queries  
✅ **Performance analysis** (theoretical + empirical)  
✅ **Nearest Neighbor search comparison**  
✅ Clean, modular code structure  

---

## 🎯 Project Goals

This project aims to:

- Implement k-d trees and quadtrees **from scratch**  
- Create a UI to help visualize construction and search  
- Benchmark performance for:
  - Insertion  
  - Searching  
  - Nearest Neighbor queries  
  - Deletion (if implemented)  
- Determine which structure performs best under:
  - Varying data distribution  
  - Point density  
  - Query patterns  

---