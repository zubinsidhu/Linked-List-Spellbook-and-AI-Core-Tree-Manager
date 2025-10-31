/*
 * Homework 12 main program
 * CS 240, Spring 2025
 * Purdue University
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw12.h"

enum main_menu_choice {
  QUIT,
  SPELLBOOK_TREE_MANIPULATIONS,
  AI_CORE_TREE_MANIPULATIONS,
};
#define CREATE_NODE (1)
#define INSERT_NODE (2)
#define FIND_NODES (3)
#define REMOVE_NODE (4)
#define DELETE_NODE (5)
#define DELETE_TREE (6)
#define PRINT_TREE (7)
#define MAX_STRING_LENGTH (100)

static int read_int(const char *);
static void read_line(const char *restrict, char *restrict, size_t);
static float read_float(const char *);

/*
 *  This function is used to run the functions in hw12.c. User input is used
 *  to determine which function to run and what is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main(int argc, char *argv[]) {
  node_t **found_array = NULL;
  node_t *root_node = NULL;
  node_t *node = NULL;
  void *data_node = NULL;

  while (1) {
    int data_type = 0;
    int user_choice = 0;

    /* Print choices */

    printf("\nSelect an option:\n"
           "  %d) Quit\n"
           "  %d) Spellbook tree manipulations\n"
           "  %d) AI core tree manipulations\n",
           QUIT, SPELLBOOK_TREE_MANIPULATIONS, AI_CORE_TREE_MANIPULATIONS);

    /* Read choice */

    while (1) {
      user_choice = read_int("Enter your choice: ");
      if ((user_choice >= QUIT) &&
          (user_choice <= AI_CORE_TREE_MANIPULATIONS)) {
        break;
      }
      printf("Invalid choice.\n");
    }

    if (user_choice == QUIT) {
      return 0;
    }
    else {
      data_type = user_choice;
    }

    int exit = 0;
    while (!exit) {

      /* Spellbook data */

      char title[MAX_STRING_LENGTH] = {};
      int spell_count = 0;
      float arcane_density = 0.0f;
      int year_written = 0;

      /* AI core data */

      char model_id[MAX_STRING_LENGTH] = {};
      float processor_speed = 0.0f;
      int num_algorithms = 0;
      int firmware_version = 0;

      user_choice = 0;
      printf("Select an option:\n"
             "  0) Return to main menu\n"
             "  1) Create node\n"
             "  2) Insert node\n"
             "  3) Find nodes\n"
             "  4) Remove node\n"
             "  5) Delete node\n"
             "  6) Delete tree\n"
             "  7) Print tree\n");

      while (1) {
        user_choice = read_int("Enter your choice: ");
        if ((user_choice >= QUIT) && (user_choice <= PRINT_TREE)) {
          break;
        }
        printf("Invalid choice.\n");
      }

      if (user_choice == QUIT) {
        printf("Cleaning up.\n");
        if (root_node) {
          delete_tree(&root_node);
        }
        if (node) {
          delete_node(&node);
        }
        exit = 1;
      }

      if ((user_choice == CREATE_NODE) && (!node)) {

        if (data_type == SPELLBOOK_TREE_MANIPULATIONS) {
          read_line("Title: ", title, sizeof(title));
          spell_count = read_int("Spell count: ");
          arcane_density = read_float("Arcane density: ");
          year_written = read_int("Year written: ");
        }
        else {
          read_line("Model ID: ", model_id, sizeof(model_id));
          processor_speed = read_float("Processor speed: ");
          num_algorithms = read_int("Number of algorithms: ");
          firmware_version = read_int("Firmware version: ");
        }
      }

      if (((user_choice == FIND_NODES) || (user_choice == REMOVE_NODE)) &&
          (!node)) {
        if (data_type == SPELLBOOK_TREE_MANIPULATIONS) {
          spell_count = read_int("Spell count: ");
          arcane_density = read_float("Arcane density: ");
          year_written = read_int("Year written: ");
        }
        else {
          processor_speed = read_float("Processor speed: ");
          num_algorithms = read_int("Number of algorithms: ");
          firmware_version = read_int("Firmware version: ");
        }
      }

      switch (user_choice) {
        case CREATE_NODE:
          if (node) {
            printf("There's a node created already. Try "
                   "inserting it in the tree or deleting it before "
                   "allocating another node.\n");
            break;
          }

          if (data_type == SPELLBOOK_TREE_MANIPULATIONS) {
            // Create the data node of the Generic node

            create_spellbook(&data_node, title, spell_count, arcane_density,
                             year_written);

            /* Use the pointer returned by create_spellbook to
             * create the generic node with a pointer to the data
             * node created above */

            create_node(&node, data_node, print_spellbook, delete_spellbook,
                        compare_spellbooks);
          }
          else {
            create_ai_core(&data_node, model_id, processor_speed,
                           num_algorithms, firmware_version);
            create_node(&node, data_node, print_ai_core, delete_ai_core,
                        compare_ai_cores);
          }

          data_node = NULL;
          printf("Created node is:\n");
          print_node(node);
          break;

        case INSERT_NODE:
          if (!node) {
            printf("Create a node first.\n");
            break;
          }
          printf("The tree before insertion:\n");
          print_tree(root_node);
          printf("Now inserting node:\n");
          print_node(node);
          insert_node(&root_node, node);
          printf("\nThe new tree looks like:\n");
          print_tree(root_node);
          node = NULL;
          break;

        case FIND_NODES:
          if (!root_node) {
            printf("There is no tree.\n");
            break;
          }

          if (node) {
            printf("There's a node created already. Try "
                   "inserting it in the tree or deleting it before "
                   "allocating another node.\n");
            break;
          }

          if (data_type == AI_CORE_TREE_MANIPULATIONS) {
            /* Create a node with only the comparison fields */

            create_spellbook(&data_node, "", spell_count, arcane_density,
                             year_written);
          }
          else {
            /* Create a node with only the comparison fields */

            create_ai_core(&data_node, "", processor_speed, num_algorithms,
                           firmware_version);
          }

          /* Now use that node to find nodes of the tree that
           * match the comparison.
           * found_array holds pointers to the found nodes. */

          int n_nodes = 0;
          found_array = find_nodes(root_node, data_node, &n_nodes);
          printf("Found %d matching nodes\n", n_nodes);
          for (int i = 0; i < n_nodes; i++) {
            printf("Node %d: ", i + 1);
            print_node(found_array[i]);
          }

          // Free the 'dummy' node used to pass the comparison element
          // this function deallocates both the node and its data

          data_node = NULL;
          break;

        case REMOVE_NODE:
          if (!root_node) {
            printf("There is no tree!\n");
            break;
          }
          if (data_type == SPELLBOOK_TREE_MANIPULATIONS) {
            /* Create a node with only the comparison fields */

            create_spellbook(&data_node, "", spell_count, arcane_density,
                             year_written);
            create_node(&node, data_node, print_spellbook, delete_spellbook,
                        compare_spellbooks);
          }
          else {
            /* Create a node with only the comparison fields */

            create_ai_core(&data_node, "", processor_speed, num_algorithms,
                           firmware_version);

            create_node(&node, data_node, print_ai_core, delete_ai_core,
                        compare_ai_cores);
          }

          /* Now use that node to find nodes of the tree that
           * match the comparison function.
           * found_array holds pointers to the found nodes. */

          n_nodes = 0;
          found_array = find_nodes(root_node, data_node, &n_nodes);
          printf("Found %d matching nodes.\n", n_nodes);
          for (int i = 0; i < n_nodes; i++) {
            printf("Node %d: ", i + 1);
            print_node(found_array[i]);
          }

          if (n_nodes == 0) {
            printf("Cannot remove nodes since none were found.\n");
            break;
          }
          int index = read_int("Enter the index of node to remove: ");

          printf("Removing ");
          print_node(found_array[index]);
          remove_node(&root_node, found_array[index]);
          printf("New tree looks like: \n");
          print_tree(root_node);

          /* Free the 'dummy' node used to pass the comparison element
           * this function deallocates both the node and its data. */

          delete_node(&node);
          data_node = NULL;
          if (found_array) {
            free(found_array);
            found_array = NULL;
          }
          break;

        case DELETE_NODE:
          if (!node) {
            printf("Create a node first.\n");
            break;
          }
          printf("Deleting node:\n");
          print_node(node);
          delete_node(&node);
          break;

        case DELETE_TREE:
          printf("Deleting entire tree.\n");
          delete_tree(&root_node);
          break;

        case PRINT_TREE:
          printf("Current tree: \n");
          print_tree(root_node);
          break;
      }
    }
  }
} /* main() */

/*
 * Prints the prompt, then reads a line containing a single integer from
 * standard input. Repeats the prompt/input stage until a valid input is read.
 * Returns the integer read.
 */

static int read_int(const char *prompt) {
  do {
    int num = 0;
    fputs(prompt, stdout);
    if ((scanf("%d", &num) == 1) && (getchar() == '\n')) {
      return num;
    }
    else {
      while (getchar() != '\n') {
      }
      printf("Invalid input. Try again.\n");
    }
  } while (1);
} /* read_int() */

/*
 * Prints the given prompt, then reads a line from standard input, and stores it
 * in the given buffer. Truncates the stored string to the given size, including
 * the NUL terminator.
 */

static void read_line(const char *restrict prompt, char *restrict buf,
                      size_t size) {
  fputs(prompt, stdout);
  fgets(buf, size, stdin);
  size_t len = strlen(buf);
  if (buf[len - 1] == '\n') {
    buf[len - 1] = '\0';
  }
  else {
    while (getchar() != '\n') {
    }
  }
} /* read_line() */

/*
 * Prints the prompt, then reads a line containing a single float from
 * standard input. Repeats the prompt/input stage until a valid input is read.
 * Returns the float read.
 */

static float read_float(const char *prompt) {
  do {
    float value = 0.0f;
    fputs(prompt, stdout);
    if ((scanf("%f", &value) == 1) && (getchar() == '\n')) {
      return value;
    }
    else {
      while (getchar() != '\n') {
      }
      printf("Invalid input. Try again.\n");
    }
  } while (1);
} /* read_float() */
