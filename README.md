# Efficient-Database-cpp

This project is a simple database system (DSDB) developed in C++ that demonstrates the use of various data structures for efficient data indexing and retrieval.

## Overview

A database is an organized collection of data or information that can be easily accessed and updated. In this project, DSDB stores data in multiple files on the computer file system. It uses two types of trees to index the data:

1. **AVL Tree**
2. **Red-Black Tree**

These trees help in performing various search and update operations efficiently.

## Features

- **Create Index**: Users can create indices on different data fields using AVL tree, or Red-Black tree. 
- **Point Search**: Retrieve specific data tuples based on a key.
- **Range Search**: Retrieve data tuples within a range of key values.
- **Update**: Modify data fields for a given key.
- **Delete**: Remove data tuples based on a key.

## Data

The data pertains to the leading causes of deaths in different states of the USA. Each data entry consists of the following fields:

- ID (Unique for every entry)
- Year
- Cause Name
- State
- Deaths
- Age-adjusted Death Rate

## Indexing

- Each tree index is stored in a separate directory.
- The root node resides in the heap (RAM) while other nodes are stored in separate files.
- Handles duplicates by maintaining a linked list of file names and line numbers for entries with the same key.

## Performance

The DSDB system displays the performance of each operation in terms of the number of disk I/O operations performed.

## Bonus Features

- **Where Clause**: Supports point search, range search, and delete operations with a `where field = value` condition.

## Development

- **Language**: C++
- **Data Structures**: AVL Tree, Red-Black Tree
