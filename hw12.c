/*
 * Homework 12
 * Zubin Sidhu
 * CS 240, Spring 2025
 * Purdue University
 */

#include "hw12.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function to print a tree node by dynamically dispatching the print function
 * stored in the node on the data stored in the node
 */

void print_node(node_t *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);
  (n_ptr->print)(n_ptr->data);
} /* print_node() */

/*
 * Function to recursively print the tree using an in-order traversal
 */

void print_tree(node_t *root) {
  if (!root) {
    return;
  }
  print_tree(root->left_child);
  print_node(root);
  print_tree(root->right_child);
} /* print_tree() */

/*
 * Create Node - Dynamically allocates and initializes a new node_t structure
 */

void create_node(node_t **node, void *data, void (*print)(void *),
                 void (*delete)(void **), int (*compare)(void *, void *)) {
  assert(node);
  assert(!*node);
  assert(data);
  assert(print);
  assert(delete);
  assert(compare);

  *node = malloc(sizeof(node_t));
  assert(*node);

  (*node)->left_child = NULL;
  (*node)->right_child = NULL;
  (*node)->data = data;
  (*node)->print = print;
  (*node)->delete = delete;
  (*node)->compare = compare;
}/* create_node() */

/*
 * Delete Node - Uses the delete callback stored in the node to free the
 * dynamically allocated data before freeing the node itself and setting the
 * pointer to NULL
 */

void delete_node(node_t **node) {
  assert(node != NULL);
  assert(*node != NULL);
  assert((*node)->left_child == NULL);
  assert((*node)->right_child == NULL);

  (*node)->delete(&((*node)->data));
  free(*node);
  *node = NULL;
} /* delete_node() */

/*
 * Insert Node - Recursive function that inserts the given node into the tree
 * based on the compare function provided in the existing tree
 */

void insert_node(node_t **root, node_t *new_node) {
  assert(root != NULL);
  assert(new_node != NULL);
  if (*root == NULL) {
    *root = new_node;
    return;
  }
  int comp = (*root)->compare(new_node->data, (*root)->data);
  if (comp <= 0) {
    insert_node(&((*root)->left_child), new_node);
  }
  else {
    insert_node(&((*root)->right_child), new_node);
  }
} /* insert_node() */

/*
 * Count Matches - Helper function to count the number of nodes that have a
 * matching data to the root and returns the count
 */

static int count_matches(node_t *root, void *target,
                         int (*compare)(void *, void *)) {
  if (!root) {
    return 0;
  }
  int count = (compare(root->data, target) == 0) ? 1 : 0;
  if (root->left_child != NULL) {
    count += count_matches(root->left_child, target, compare);
  }
  if (root->right_child != NULL) {
    count += count_matches(root->right_child, target, compare);
  }
  return count;
} /* count_matches() */

/*
 * Fill Matches - Helper function to recursively search through a binary tree
 * and fill an array of all the nodes that match the root's data
 */

static void fill_matches(node_t *root, void *target,
                         int (*compare)(void *, void *), node_t **arr,
                         int *index) {
  if (!root) {
    return;
  }
  if (compare(root->data, target) == 0) {
      arr[(*index)++] = root;
  }
  if (root->left_child != NULL) {
    fill_matches(root->left_child, target, compare, arr, index);
  }
  if (root->right_child != NULL) {
    fill_matches(root->right_child, target, compare, arr, index);
  }
} /* fill_matches() */

/*
 * Find Nodes - Searches the tree using a prefix traversal to find all nodes
 * whose data matches the given target according to the compare function
 */

node_t **find_nodes(node_t *root, void *target, int *count) {
  assert(root);
  assert(target);
  assert(count);

  *count = count_matches(root, target, root->compare);
  if (*count == 0) {
    return NULL;
  }
  node_t **matches = malloc(*count * sizeof(node_t *));
  assert(matches);
  int index = 0;
  fill_matches(root, target, root->compare, matches, &index);
  return matches;
} /* find_nodes() */

/*
 * Delete Tree - Recursive function that frees all nodes in the tree where each
 * node uses its internal delete function to free their internal data
 */

void delete_tree(node_t **root) {
  assert(root != NULL);
  if ((*root) != NULL) {
    if ((*root)->left_child != NULL) {
      delete_tree(&((*root)->left_child));
    }
    if ((*root)->right_child != NULL) {
      delete_tree(&((*root)->right_child));
    }
    delete_node(root);
  }
} /* delete_tree() */
#if 0
/*
 * Find Max Node - Starting from the root node, find the largest right child
 * and return its parent
 */

static node_t *find_max_node(node_t *root) {
  if (root == NULL) {
    printf("DEBUG::Root is NULL unexpected.....\n");
    return NULL;
  }
  if (root->right_child == NULL) {
    printf("DEBUG::Root right child is NULL, returning root\n");
    return root;
  }

  node_t *parent = root;
  while (parent->right_child->right_child != NULL) {
    printf("DEBUG::Traversing right tree of root\n");
    print_node(parent->right_child);
    parent = parent->right_child;
  }
  return parent;
} /* find_max_node() */
#endif
/*
 * Remove Node - Removes the specified node from the tree rooted at the first
 * argument. Utilizes the compare callback to locate the node. If the located
 * has two children, it finds the in-order predecessor (the greatest node in
 * its left subtree). Then moves the deleted node's right child to be the right
 * child of the predecessor, then replace the deleted node with its left child
 */

void remove_node(node_t **root, node_t *target) {
  assert(root);
  assert(target);

  if ((*root) == NULL) {
    return;
  }

  print_tree(*root);

  int comp = (*root)->compare(target->data, (*root)->data);
  printf("DEBUG::Comp = %d, Target Node = %p, Root = %p\n", comp, target, *root);
  print_node(target);
  print_node(*root);
  if (*root == target) {
    node_t *to_remove = *root;
    if ((to_remove->left_child == NULL) && (to_remove->right_child == NULL)) {
      printf("DEBUG::Root Node has no children, deleting\n");
      delete_node(root);
    }
    else if ((to_remove->left_child == NULL) &&
             (to_remove->right_child != NULL)) {
      printf("DEBUG::Process Right Child Node\n");
      print_node(to_remove->right_child);
      *root = to_remove->right_child;
      to_remove->right_child = NULL;
      delete_node(&to_remove);
    }
    else if ((to_remove->left_child != NULL) &&
             (to_remove->right_child == NULL)) {
      printf("DEBUG::Process Left Child Node\n");
      print_node(to_remove->left_child);
      *root = to_remove->left_child;
      to_remove->left_child = NULL;
      delete_node(&to_remove);
    }
    else {
      node_t *pred = to_remove->left_child;
      printf("DEBUG::Left Child of Root is \n");
      print_node(pred);
      while (pred->right_child != NULL) {
        printf("DEBUG::Traversing to Next Level, Right Child \n");
        print_node(pred->right_child);
        if (pred->left_child != NULL) {
          printf("Left Child \n");
          print_node(pred->left_child);
        }

        pred = pred->right_child;
      }

      pred->right_child = to_remove->right_child;
      *root = to_remove->left_child;

      printf("DEBUG::Promoting Node\n");
      print_node(pred);

      printf("DEBUG::Removing Node\n");
      print_node(to_remove);

      to_remove->left_child = NULL;
      to_remove->right_child = NULL;
      delete_node(&to_remove);
    }
  }
  else if (comp <= 0) {
    if ((*root)->left_child != NULL) {
      printf("DEBUG::Traversing Left Child\n");
      print_node((*root)->left_child);
    }
    remove_node(&((*root)->left_child), target);
  }
  else if (comp > 0) {
    if ((*root)->right_child != NULL) {
      printf("DEBUG::Traversing right Child\n");
      print_node((*root)->right_child);
    }
    remove_node(&((*root)->right_child), target);
  }
  printf("DEBUG::Final Tree.......\n");
  print_tree(*root);
} /* remove_node() */

/*
 * Create Spellbook - Allocates a new spellbook_t and initializes its fields
 * using the arguments provided. The pointer passed as the first argument is
 * updated to point to the new spellbook structure, cast to a (void *)
 */

void create_spellbook(void **data, const char *title, int spell_count,
                      float arcane_density, int year_written) {
  assert(data);
  assert(!(*data));
  assert(title);

  spellbook_t *spell_book = malloc(sizeof(spellbook_t));
  assert(spell_book);

  spell_book->title = malloc(strlen(title) + 1);
  assert(spell_book->title);
  strcpy(spell_book->title, title);

  spell_book->spell_count = spell_count;
  spell_book->arcane_density = arcane_density;
  spell_book->year_written = year_written;
  *data = (void *)spell_book;
} /* create_spellbook() */

/*
 * Function to print the internal data of a spellbook_t struct passed as a void
 * pointer to standard out.
 */

void print_spellbook(void *p) {
  spellbook_t *ptr = (spellbook_t *) p;
  assert(ptr && ptr->title);
  printf("Title: %s\nSpell Count: %d\nArcane Density: %.2f\n"
         "Year Written: %04d\n",
         ptr->title, ptr->spell_count, ptr->arcane_density, ptr->year_written);
} /* print_spellbook() */

/*
 * Delete Spellbook - Casts the void * back to a spell_book_t * and frees any
 * dynamically allocated members before freeing the structure itself
 */

void delete_spellbook(void **data) {
  assert(data);
  assert(*data);

  spellbook_t *spell_book = (spellbook_t *)(*data);
  free(spell_book->title);
  free(spell_book);
  *data = NULL;
} /* delete_spellbook() */

/*
 * Compare Spellbooks - Compares two spellbooks by multiplying its
 * arcane_destiny by its spell_count and dividing that product by the
 * difference of the current year by the year_written + 1. This formula
 * prevents rounding errors and returns 1 if the first spellbook is greater,
 * -1 if the second spellbook is greater, and 0 if they are equal
 */

int compare_spellbooks(void *spell_book_one, void *spell_book_two) {
  assert(spell_book_one);
  assert(spell_book_two);

  spellbook_t *first_spell_book = (spellbook_t *)spell_book_one;
  spellbook_t *second_spell_book = (spellbook_t *)spell_book_two;

  float result_one = (first_spell_book->arcane_density *
                      first_spell_book->spell_count) /
                      ((float)(CURRENT_YEAR -
                       first_spell_book->year_written + 1));
  float result_two = (second_spell_book->arcane_density *
                      second_spell_book->spell_count) /
                      ((float)(CURRENT_YEAR -
                       second_spell_book->year_written + 1));
  if (result_one > result_two) {
    return 1;
  }
  else if (result_one < result_two) {
    return -1;
  }
  else {
    return 0;
  }
} /* compare_spellbooks() */

/*
 * Create AI Core - Allocates a new ai_core_t and initializes its fields
 * using the arguments provided. The pointer passed as the first argument is
 * updated to point to the new spellbook structure, cast to a (void *)
 */

void create_ai_core(void **data, const char *model_id, float processor_speed,
                    int num_algorithms, int firmware_version) {
  assert(data);
  assert(!(*data));
  assert(model_id);

  ai_core_t *ai = malloc(sizeof(ai_core_t));
  assert(ai);

  ai->model_id = malloc(strlen(model_id) + 1);
  assert(ai->model_id);
  strcpy(ai->model_id, model_id);

  ai->processor_speed = processor_speed;
  ai->num_algorithms = num_algorithms;
  ai->firmware_version = firmware_version;
  *data = (void *)ai;
} /* create_ai_core() */

/*
 * Function to print the internal data of an ai_core_t struct passed as a void
 * pointer to standard out.
 */

void print_ai_core(void *p) {
  ai_core_t *ptr = (ai_core_t *) p;
  assert(ptr && ptr->model_id);
  printf("Model ID: %s\nProcessor Speed: %.2f\nNumber of Algorithms: %d\n"
         "Firmware Version: %d\n",
         ptr->model_id, ptr->processor_speed, ptr->num_algorithms,
         ptr->firmware_version);
} /* print_ai_core() */

/*
 * Delete AI Core - Casts the void * back to a ai_core_t * and frees any
 * dynamically allocated members before freeing the structure itself
 */

void delete_ai_core(void **data) {
  assert(data);
  assert(*data);

  ai_core_t *ai = (ai_core_t *)(*data);
  free(ai->model_id);
  free(ai);
  *data = NULL;
} /* delete_ai_core() */

/*
 * Compare AI Cores - Compares two AI cores by multiplying its
 * processor_speed by its num_algorithms and dividing that product by the
 * difference of the current year by the firmware_version + 1. This formula
 * prevents rounding errors and returns 1 if the first spellbook is greater,
 * -1 if the second spellbook is greater, and 0 if they are equal
 */

int compare_ai_cores(void *ai_one, void* ai_two) {
  assert(ai_one);
  assert(ai_two);

  ai_core_t *first_ai = (ai_core_t *)ai_one;
  ai_core_t *second_ai = (ai_core_t *)ai_two;

  float result_one = (first_ai->processor_speed * first_ai->num_algorithms) /
                     ((float)(CURRENT_YEAR - first_ai->firmware_version + 1));
  float result_two = (second_ai->processor_speed * second_ai->num_algorithms) /
                     ((float)(CURRENT_YEAR - second_ai->firmware_version + 1));
  if (result_one > result_two) {
    return 1;
  }
  else if (result_one < result_two) {
    return -1;
  }
  else {
    return 0;
  }
} /* compare_ai_cores() */
