/* 
 * Trie Prediction - Jared Johansson
 * Spring 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

#define DEBUG 0

// Recursive print of all words in Trie, helper to PrintTrie, O(n)
void printLoop(TrieNode *root, char *string, int k, int is_sub, int print_sub)
{
  int i;
  char *sub_string;

  if (root == NULL || (is_sub && !print_sub))
    return;

  // Set next char to NULL char
  string[k + 1] = '\0';

  if (root->count > 0)
    printf("%s%s (%d)\n", (is_sub) ? "- " : "", string, root->count);

  // Check all letters in array
  for (i = 0; i < 26; i++)
  {
    // If a branch exists, add a letter and take branch
    if (root->children[i] != NULL)
    {
      string[k] = (char)(i + 'a');
      printLoop(root->children[i], string, k + 1, is_sub, print_sub);
      string[k + 1] = '\0';
    }
  }

  if (root->subtrie != NULL)
  {
    if ((sub_string = calloc((MAX_CHARACTERS_PER_WORD + 1), sizeof(char))) == NULL)
    {
      fprintf(stderr, "Error: Not enough memory for sub_string!\n");
      return;
    }

    printLoop(root->subtrie, sub_string, 0, 1, print_sub);

    free(sub_string);
  }
}

// Print words from trie struct, O(n)
void printTrie(TrieNode *root, int is_sub, int print_sub)
{
  char string[MAX_CHARACTERS_PER_WORD + 1] = {};

  if (root == NULL)
    return;

  printLoop(root, string, 0, is_sub, print_sub);
}

// Allocates space for a new TrieNode, O(1)
TrieNode *createNode(void)
{
  TrieNode *new;

  if ((new = calloc(1, sizeof(TrieNode))) == NULL)
  {
    fprintf(stderr, "Error: Not enough memory for new TrieNode!\n");
    return NULL;
  }

  return new;
}

// Converts string to lowercase only, O(k)
void checkString(char *string, int *end_char)
{
  int i, k, j = 0, length = strlen(string);
  char *new_string;

  if ((new_string = malloc(sizeof(char) * (length + 1))) == NULL)
  {
    fprintf(stderr, "Error: Not enough memeory for checkString!\n");
    return;
  }

  // Loop through whole string
  for (i = 0; i < length; i++)
  {
    // Uppercase to lowercase
    if (string[i] >= 'A' && string[i] <= 'Z')
      string[i] += ('a' - 'A');

    // End of scentence
    if (string[i] == '.' || string[i] == '!' || string[i] == '?')
      *end_char = 1;

    // Removes anything not lowercase
    if (string[i] >= 'a' && string[i] <= 'z')
    {
      new_string[j] = string[i];
      j++;
    }
  }
  new_string[j] = '\0';

  // Save correct string, free used space
  strcpy(string, new_string);
  free(new_string);
}

// Returns address of last node after string is added to tree, O(k)
TrieNode *addWord(TrieNode *root, char *word, int is_sub)
{
  int i, index, length = strlen(word);
  TrieNode *aux = root;

  // Check if adding to a subtrie
  if (is_sub)
  {
    if (root->subtrie == NULL)
      aux = root->subtrie = createNode();
    else
      aux = root->subtrie;
  }

  // If first entry into non-existant subtrie
  else if (root == NULL)
    aux = root = createNode();

  // Travel through tree, while creating if neccessary
  for (i = 0; i < length; i++)
  {
    index = (int)(word[i] - 'a');

    if (aux->children[index] == NULL)
      aux->children[index] = createNode();

    aux = aux->children[index];
  }

  aux->count++;
  return aux;
}

// Parameter of filename and constructs trie from the file, O(n)
TrieNode *buildTrie(char *filename)
{
  int period = 0;
  TrieNode *new_trie, *previous = NULL;
  FILE *ifp;
  char  separator, *buffer;

  // Allocating memory
  if ((buffer = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1))) == NULL)
  {
    fprintf(stderr, "Error: Not enough memory for buffer!\n");
    return NULL;
  }

  // Get root node
  new_trie = createNode();
  ifp = fopen(filename, "r");

  if (DEBUG)
    printf("     -- Reading file: %s --\n", filename);

  // Begin building Trie
  while (fscanf(ifp, "%1023s", buffer) != EOF)
  {
    // Catch spaces and null characters
    separator = fgetc(ifp);
    if (separator != ' ' && separator != '\n')
      fseek(ifp, -1, SEEK_CUR);

    if (DEBUG)
      printf("%20s", buffer);

    // Change string to only lowercase words
    checkString(buffer, &period);

    // Add string to previous subtrie
    if (previous)
      previous = addWord(previous, buffer, 1);

    // Add string to Trie
    previous = addWord(new_trie, buffer, 0);

    // If at end of line or scentence, clear previous pointer.
    if (separator == '\n' || period)
    {
      previous = NULL;
      period = 0;
    }

    if (DEBUG)
      printf("  |  %-s\n", buffer);
  }

  free(buffer);
  fclose(ifp);
  return new_trie;
}

// Burns down trie in a beautiful display of free's distructive power, O(n)
TrieNode *destroyTrie(TrieNode *root)
{
  int i;

  if (root == NULL)
    return NULL;

  // Free subtrie
  if (root->subtrie)
    root->subtrie = destroyTrie(root->subtrie);

  // Free all children
  for (i = 0; i < 26; i++)
    if (root->children[i])
      root->children[i] = destroyTrie(root->children[i]);

  // Free Node itself
  free(root);

  return NULL;
}

// Returns address of last node in word, Best: O(1), Worst: O(k)
TrieNode *getNode(TrieNode *root, char *string)
{
  int length, i, index;
  TrieNode *aux = root;

  if (root == NULL || string == NULL || (length = strlen(string)) == 0)
    return NULL;

  for (i = 0; i < length; i++)
  {
    index = (int)(string[i] - 'a');

    if (aux->children[index])
      aux = aux->children[index];
    else
      return NULL;
  }

  if (aux->count > 0)
    return aux;
  else
    return NULL;
}

// Recursive function to help find most frequent word, O(n)
int frequencyLoop(TrieNode *root, char **string, char *build, int k, int count)
{
  int i, check_val = 0;

  if (root == NULL)
    return count;

  // If new highest count, update string
  if (root->count > count)
  {
    count = root->count;
    strcpy((*string), build);
  }

  build[k + 1] = '\0';

  // Go through whole tree
  for (i = 0; i < 26; i++)
  {
    // Build new string
    if (root->children[i])
    {
      build[k] = (char)(i + 'a');
      check_val = frequencyLoop(root->children[i], string, build, k + 1, count);

      // Update largest value found
      if (check_val > count)
        count = check_val;
    }
  }

  build[k] = '\0';
  return count;
}

// Search for most common word in trie, then copy to str, O(n)
void getMostFrequentWord(TrieNode *root, char *string)
{
  char *new, build[MAX_CHARACTERS_PER_WORD + 1] = {};

  if ((new = calloc((MAX_CHARACTERS_PER_WORD + 1), sizeof(char))) == NULL)
  {
    fprintf(stderr, "Error: Not enough memory for getMostFrequentWord!\n");
    return;
  }

  frequencyLoop(root, &new, build, 0, 0);
  strcpy(string, new);
  free(new);
}

// If str is in trie (1 or > count), return 1, Best: O(1), Worst: O(k)
int containsWord(TrieNode *root, char *string)
{
  int i, length, index;
  TrieNode *aux = root;

  if (root == NULL || string == NULL || (length = strlen(string)) == 0)
    return 0;

  for (i = 0; i < length; i++)
  {
    index = (int)(string[i] - 'a');

    // If children[] doesn't exit, don't continue
    if (aux->children[index])
       aux = aux->children[index];
    else
      return 0;
  }

  // Must be a word and not part of a word
  if (aux->count > 0)
    return 1;
  else
    return 0;
}

// Adds all counts of words in trie, O(n)
int prefixLoop(TrieNode *root)
{
  int i, total = 0;

  if (root == NULL)
    return 0;

  // Count all children
  for (i = 0; i < 26; i++)
    if (root->children[i])
      total += prefixLoop(root->children[i]);

  // Add own count before returning
  return (total + root->count);
}

// Adds all counts for any words starting with prefix, Best: O(1), Worst: O(n)
int prefixCount(TrieNode *root, char *string)
{
  int i, length, index;
  TrieNode *aux = root;

  if (root == NULL || string == NULL)
    return 0;

  length = strlen(string);

  // Get to end of inserted Prefix-String
  for (i = 0; i < length; i++)
  {
    index = (int)(string[i] - 'a');

    if (aux->children[index])
       aux = aux->children[index];
    else
      return 0;
  }

  // Count everything after prefix-string
  return prefixLoop(aux);
}

// Function to check valid inputs from command line, O(1)
int verifyInput(int argc, char **argv)
{
  int i;
  FILE *tp;

  // Number of arguements
  if (argc != 3)
  {
    fprintf(stderr, "\nProper syntax: %s <corpus_file.txt> <input_file.txt>\n"
                    " corpus_file  contains list of words to build trie with.\n"
                    " input_file   is a list of commands to be issued to program.\n", argv[0]);
    return 0;
  }

  // Verify both files can be found
  for (i = 1; i < 3; i++)
  {
    if ((tp = fopen(argv[i], "r")) == NULL)
      fprintf(stderr, "Error: Failed to open %s!\n", argv[i]);
    fclose(tp);
  }

  return 1;
}

// Prints predicted text to string n times, O(n)
void textPrediction(TrieNode *root, char *string, int n)
{
  int i;
  TrieNode *aux;
  char follow[MAX_CHARACTERS_PER_WORD + 1] = {};

  if (n < 1)
    return;

  printf("%s", string);
  checkString(string, 0); // useless variable?
  strcpy(follow, string);

  // Prediction loop
  for (i = 0; i < n; i++)
  {
    if ((aux = getNode(root, follow)) == NULL)
      break;

    getMostFrequentWord(aux->subtrie, follow);
    if (follow[0] != '\0')
      printf(" %s", follow);
  }

  printf("\n");
}

double difficultyRating(void)
{
  return 3.5;
}

double hoursSpent(void)
{
  return 5;
}

// Takes two input files as parameters
int main(int argc, char **argv)
{
  int num;
  TrieNode *root, *aux;
  char buffer[MAX_CHARACTERS_PER_WORD + 1] = {};
  FILE *ifp = NULL;

  // Check for inproper syntax or bad files
  if (!verifyInput(argc, argv))
    return 0;

  root = buildTrie(argv[1]);
  ifp = fopen(argv[2], "r");

  // Input file loop
  while (fscanf(ifp, "%s", buffer) != EOF)
  {
    // Print trie command
    if (buffer[0] == '!')
    {
      printTrie(root, 0, 0);
      continue;
    }

    // Text prediction command
    if (buffer[0] == '@')
    {
      fscanf(ifp, "%s %d", buffer, &num);
      textPrediction(root, buffer, num);
      continue;
    }

    // Print input string and it's subtrie
    printf("%s\n", buffer);
    checkString(buffer, &num);

    // If exist in trie, print subtrie
    if ((aux = getNode(root, buffer)) != NULL)
    {
      if (aux->subtrie != NULL)
        printTrie(aux->subtrie, 1, 1);
      else
        printf("(EMPTY)\n");
    }
    else
      printf("(INVALID STRING)\n");
  }

  // Watch the world burn
  destroyTrie(root);
  fclose(ifp);

  return 0;
}
