# Grand Archive: A Polymorphic Binary Tree in C
## Overview

The Grand Archive is a C program that implements a generic polymorphic binary tree capable of managing multiple data types through function pointers, type casting, and dynamic memory management.

This system supports two distinct artifact types:

🪶 Spellbooks — magical tomes with mystical properties

🤖 AI Cores — futuristic computational nodes

Each type defines its own print, compare, and delete functions, while sharing a single generic node_t tree structure. The project demonstrates how to implement polymorphism and callback-based design in C.

## Features

Generic binary search tree supporting multiple data types

Polymorphic behavior using function pointers

Safe dynamic memory management with strict assertions

Support for both Spellbooks and AI Cores

Fully recursive tree traversal, insertion, deletion, and search

Type-specific logic encapsulated cleanly with callbacks

## Data Structures
Generic Node
typedef struct node {
    struct node *left_child;
    struct node *right_child;
    void *data;

    void (*print)(void *);
    void (*delete)(void **);
    int  (*compare)(void *, void *);
} node_t;

Spellbook
typedef struct spellbook {
    char *title;
    int spell_count;
    float arcane_density;
    int year_written;
} spellbook_t;

AI Core
typedef struct ai_core {
    char *model_id;
    float processor_speed;
    int num_algorithms;
    int firmware_version;
} ai_core_t;

## Functions Implemented
### Generic Tree Operations
#### Function	Description
create_node()	Dynamically allocates and initializes a new tree node
delete_node()	Frees a node and its data using its callback
insert_node()	Inserts a node recursively based on its compare function
remove_node()	Removes a specific node from the tree and restructures links safely
find_nodes()	Returns an array of all matching nodes based on the compare function
delete_tree()	Recursively frees the entire tree
### Spellbook-Specific Functions
#### Function	Description
create_spellbook()	Allocates and initializes a new spellbook
print_spellbook()	Prints all spellbook fields
delete_spellbook()	Frees all memory used by a spellbook
compare_spellbooks()	Compares two spellbooks by magical potency
### AI Core-Specific Functions
#### Function	Description
create_ai_core()	Allocates and initializes a new AI core
print_ai_core()	Prints all AI core fields
delete_ai_core()	Frees all memory used by an AI core
compare_ai_cores()	Compares two AI cores by processing efficiency
## Comparison Metrics

Spellbooks:
arcane_density × spell_count ÷ (CURRENT_YEAR − year_written + 1)

AI Cores:
processor_speed × num_algorithms ÷ (CURRENT_YEAR − firmware_version + 1)

These formulas determine node order within the tree.

## Example Usage
void example() {
    node_t *root = NULL;
    void *book1 = NULL;
    create_spellbook(&book1, "Book of Shadows", 42, 3.5, 1800);

    node_t *n1 = NULL;
    create_node(&n1, book1, print_spellbook, delete_spellbook, compare_spellbooks);

    insert_node(&root, n1);
    print_tree(root);
    delete_tree(&root);
}

## Building & Running
Compile
gcc hw12.c main.c -o hw12 -Wall -Werror -std=c11

Run
./hw12

Clean
rm -f hw12 *.o

## File Structure
.
├── hw12.c          # Main implementation file
├── hw12.h          # Provided header file (do not modify)
├── main.c          # Example driver for testing
├── README.md       # Project documentation
└── Makefile        # Optional (for convenience)

## Memory Safety

Every allocation is paired with a proper deallocation.

All pointers are validated via assert().

No global variables are used.

Designed to be valgrind-clean.

## Author

Developed by Zubin Sidhu
Computer Science — Polymorphism and Trees Assignment
