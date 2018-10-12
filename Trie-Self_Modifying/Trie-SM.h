#ifndef __FANCY_TIRE_H
#define __FANCY_TRIE_H

// Largest string to be held
#define MAX_WORD_LENGTH 1023

typedef struct trieNode
{
  char *indexes;               // Indexes when last updated
  int word;                    // If known word 1, else 0
  int size;                    // Number of node pointers
  struct trieNode **nodes;     // Initalize with only lowercase
} Trie;

typedef struct headNode
{
  char *indexes;               // Master list of indexes
  int capacity;                // Number of indexes
  Trie *root;                  // Root to trie sculpture
} Tree;

#endif
