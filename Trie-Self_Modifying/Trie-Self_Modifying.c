// Trie that automatically adds to array create new indexes - Jared Johansson
// Created March 2017
// Last updated May 2018: added ability to print and destroy tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie-SM.h"

#define DEBUG 0

// Allocates memory for a trieNode, O(1)
Trie *createNode(char *master, int size)
{
  if (DEBUG) printf("Entered: createNode\n");
  Trie *new;

  if ((new = malloc(sizeof(Trie))) == NULL)
    return NULL;

  if ((new->nodes = calloc(size, sizeof(Trie *))) == NULL)
  {
    free(new);
    return NULL;
  }

  if ((new->indexes = malloc(sizeof(char) * size)) == NULL)
  {
    free(new->nodes);
    free(new);
    return NULL;
  }

  new->word = 0;
  new->size = size;
  strcpy(new->indexes, master);

  if (DEBUG) printf("->Exit: createNode\n");
  return new;
}

// Initalizes struct with root trie, O(1)
Tree *createTree(void)
{
  if (DEBUG) printf("Entered: createTree\n");
  Tree *new;

  // Allocating all space
  if ((new = malloc(sizeof(Tree))) == NULL)
    return NULL;

  if ((new->indexes = malloc(sizeof(char) * 27)) == NULL)
    return NULL;

  // Inializing
  new->capacity = 26;
  strcpy(new->indexes, "abcdefghijklmnopqrstuvwxyz");
  new->root = createNode(new->indexes, new->capacity);

  if (DEBUG) printf("->Exit: createTree\n");
  return new;
}

// Adds new index to string of known indexes, O(1)
void addIndex(Tree *tree, int ch)
{
  if (DEBUG) printf("Entered: addIndex\n");
  int i = 0, j = 0;
  char *new;

  // Space for current string plus null char and new char
  if ((new = malloc(sizeof(char) * (tree->capacity + 2))) == NULL)
    return;

  // Get location of where index needs to be added
  while (ch > tree->indexes[i])
    i++;

  // Add all letters before insertion
  while (j < i)
    new[j] = tree->indexes[j++];

  // Insert and add remaining
  new[i] = ch;
  while(i < tree->capacity)
    new[i + 1] = tree->indexes[i++];

  free(tree->indexes);
  tree->indexes = new;

  tree->capacity++;
  tree->indexes[tree->capacity] = '\0';
  if (DEBUG) printf("->Exit: addIndex\n");
}

// Checks if new char and adds to indexes if so, O()
void ifnew(Tree *tree, int ch)
{
  if (DEBUG) printf("Entered: ifnew\n");
  int i, in = 0;

  for (i = 0; i < tree->capacity; i++)
  {
    // skip a - z, because they will not match
    if (tree->indexes[i] == 'a')
      i += 25;

    if (tree->indexes[i] == ch)
      in = 1;
  }

  if (!in)
    addIndex(tree, ch);
  if (DEBUG) printf("->Exit: ifnew\n");
}

// Checks string to see if new index needs to be added, O()
void checkStr(Tree *tree, char *str, int len)
{
  if (DEBUG) printf("Entered: checkStr\n");
  int i;

  for (i = 0; i < len; i++)
  {
    // Only check if char has already been added if not a - z
    if ((str[i] >= ' ' && str[i] < 'a') || (str[i] > 'z' && str[i] <= '~'))
      ifnew(tree, str[i]);
  }
  if (DEBUG) printf("->Exit: checkStr\n");
}

// Checks if trie node has updated indexes, O()
void checkIndexes(Tree *tree, Trie *root)
{
  if (DEBUG) printf("Entered: checkIndexes\n");
  int i = 0, j = 0;
  char *newIndexes;
  Trie **newNodes;

  if (tree->capacity != root->size)
  {
    if ((newNodes = calloc(tree->capacity, sizeof(Trie *))) == NULL)
      return;

    if ((newIndexes = malloc(sizeof(char) * (tree->capacity + 1))) == NULL)
    {
      free(newNodes);
      return;
    }

    // Copy addresses to new trie node pointer array
    while(j < tree->capacity)
    {
      if (tree->indexes[j] == root->indexes[i])
      {
        newNodes[j] = root->nodes[i];
        i++;  j++;
      }
	  else
	  {
		newNodes[j] = NULL;
		j++;
	  }
    }

    free(root->nodes);
    root->nodes = newNodes;

    strcpy(newIndexes, tree->indexes);
    free(root->indexes);
    root->indexes = newIndexes;
  }
  if (DEBUG) printf("->Exit: checkIndexes\n");
}

// Retrieves proper index of trie node array
int getIndex(Tree *tree, int ch)
{
  if (DEBUG) printf("Entered: getIndex\n");
  int i = 0;

  while (ch != tree->indexes[i])
    i++;

  if (DEBUG) printf("->Exit: getIndex\n");
  return i;
}

// Add word into trie, O()
void insert(Tree *tree, char *str, int len)
{
  if (DEBUG) printf("Entered: insert\n");
  int i, index;
  Trie *aux = tree->root;

  for (i = 0; i < len; i++)
  {
    // Updating old indices and getting correct index
    checkIndexes(tree, aux);
    index = getIndex(tree, str[i]);

    // Create path while moving forward
    if (aux->nodes[index] == NULL)
      aux->nodes[index] = createNode(tree->indexes, tree->capacity);

    aux = aux->nodes[index];
  }

  aux->word = 1;
  if (DEBUG) printf("->Exit: insert\n");
}

// Main function handle adding word into trie, O(k)
Tree *addWord(Tree *tree, char *str)
{
  if (DEBUG) printf("Entered: addWord\n");
  int len;

  if (str == NULL)
    return tree;

  if ((len = strlen(str)) > MAX_WORD_LENGTH)
  {
    fprintf(stderr, "Error: String too large.\n");
    return tree;
  }

  if (tree == NULL)
    tree = createTree();

  checkStr(tree, str, len); // Master indexes updated
  insert(tree, str, len);

  if (DEBUG) printf("->Exit: addWord\n\n");
  return tree;
}

// Recursive print function to print words, O(n)
void printLoop(Trie *root, char *str, int k)
{
	int i;

	if (root == NULL)
		return;

	str[k + 1] = '\0';
	if (root->word == 1)
		printf("->%s\n", str);

	for (i = 0; i < root->size; i++)
		if (root->nodes[i] != NULL)
		{
			str[k] = root->indexes[i];
			printLoop(root->nodes[i], str, k + 1);
		}

	str[k] = '\0';
}

// Print words from trie struct, O(n)
void printTree(Tree *tree)
{
	int i;
	char str[MAX_WORD_LENGTH + 1];

	if (tree->root == NULL)
		return;

	str[1] = '\0';
	for (i = 0; i < tree->capacity; i++)
		if (tree->root->nodes[i] != NULL)
		{
			str[0] = tree->root->indexes[i];
			printLoop(tree->root->nodes[i], str, 1);
		}
}

// Frees all trie data
void burnTrie(Trie *root)
{
	int i;

	if (root == NULL)
		return;

	for (i = 0; i < root->size; i++)
		burnTrie(root->nodes[i]);

	free(root->indexes);
	free(root->nodes);
	free(root);
}

// Destroys tree by freeing all space held by structure
void destroyTree(Tree *tree)
{
	int i;

	if (tree == NULL)
		return;

	burnTrie(tree->root);
	free(tree->indexes);
	free(tree);
}

int main(void)
{
	Tree *tree = NULL;

	tree = addWord(tree, "add space");
	printf("%s\n", tree->indexes);

	addWord(tree, "add 1 2 3");
	printf("%s\n", tree->indexes);

	addWord(tree, "add 0 9 5");
	printf("%s\n", tree->indexes);

	addWord(tree, "test symb@ls");
	printf("%s\n", tree->indexes);

	addWord(tree, "more #(!*$");
	printf("%s\n", tree->indexes);

	addWord(tree, "existing");
	printf("%s\n", tree->indexes);

	addWord(tree, "EXISTING");
	printf("%s\n", tree->indexes);

	printTree(tree);
	destroyTree(tree);

	return 0;
}
