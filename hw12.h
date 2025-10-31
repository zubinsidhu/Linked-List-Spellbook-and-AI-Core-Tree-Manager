#ifndef HW12_H
#define HW12_H

#define CURRENT_YEAR (2025)

enum node_kind {
  SPELLBOOK,
  AI_CORE,
};

/* Generic node structure */

typedef struct node {
  struct node *left_child;
  struct node *right_child;
  void *data;
  void (*print)(void *);
  void (*delete)(void **);
  int (*compare)(void *, void *);
} node_t;

/* Data structure 1: spellbook structure */

typedef struct spellbook {
  char *title;
  int spell_count;
  float arcane_density;
  int year_written;
} spellbook_t;

/* Data structure 2: AI core structure */

typedef struct ai_core {
  char *model_id;
  float processor_speed;
  int num_algorithms;
  int firmware_version;
} ai_core_t;

/*
 * Functions of the Tree
 */

void print_tree(node_t *);
void delete_tree(node_t **);

/*
 * Functions of the Tree-nodes
 */

void print_node(node_t *);
void create_node(node_t **, void *, void (*)(void*),
                 void (*)(void **), int (*)(void *, void *));
void delete_node(node_t **);
void insert_node(node_t **, node_t *);
void remove_node(node_t **, node_t *);
node_t **find_nodes(node_t *, void *, int *);

/*
 * Functions for accessing/manipulating node-data
 */

/* data access/manipulation functions for spellbook structure */

void print_spellbook(void *);
void create_spellbook(void **, const char *, int, float, int);
int compare_spellbooks(void *, void *);
void delete_spellbook(void **);

/* data access/manipulation functions for AI core structure */

void print_ai_core(void *);
void create_ai_core(void **, const char *, float, int, int);
int compare_ai_cores(void *, void *);
void delete_ai_core(void **);

#endif // HW12_H
