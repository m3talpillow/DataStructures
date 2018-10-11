/*
 * Jared Johansson
 * Spring 2017 - Assignment 
 */

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

// Elements of Linked stack
typedef struct element
{
  node *n;            // Tree node to be pushed onto stack
  int flag;           // Flag if children have already been pushed, 1 if yes
} Element;

// Linked list type struct (stack replacement)
typedef struct link
{
  Element *e;         // Pointer to held element
  struct link *next;  // Points to next link in list
} Link;


// Returns 1 if two trees are reflections, O(n)
int isReflection(node *a, node *b)
{
  if (a == NULL && b == NULL)
    return 1;
  else if (a == NULL || b == NULL)
    return 0;

  // Check data equivalency
  if (a->data == b->data)
  {
    if (isReflection(a->left, b->right) && isReflection(a->right, b->left))
      return 1;
  }

  return 0;
}

// Makes a reflection of tree, O(n)
node *makeReflection(node *root)
{
  node *new;

  if (root == NULL)
    return NULL;

  if ((new = malloc(sizeof(node))) == NULL)
    return NULL;

  new->data = root->data;
  new->left = makeReflection(root->right);
  new->right = makeReflection(root->left);

  return new;
}

// Create a single element, O(1)
Element *createElement(node *pass, int flag) // if node is null, element = NULL
{
  Element *e;

  if (pass == NULL)
    return NULL;

  if ((e = malloc(sizeof(Element))) == NULL)
    return NULL;

  e->n = pass;
  e->flag = flag;

  return e;
}

// Start linked stack, O(1)
Link *createLink(node *pass, int flag)      // if node is null, link = NULL
{
  Link *new;

  if (pass == NULL)
    return NULL;

  if ((new = malloc(sizeof(Link))) == NULL)
    return NULL;

  new->e = createElement(pass, flag); // O(1)
  new->next = NULL;

  return new;
}

// Return element from front of list, move down list, O(1)
Element *pop(Link **list)                   // if list is NULL, element = NULL
{
  Element *catch;
  Link *temp;

  if (list == NULL || *list == NULL)
    return NULL;

  catch = (*list)->e;      // Get element from frist link
  temp = *list;            // Save address of first link
  *list = (*list)->next;   // Move first link back to second link
  free(temp);              // Delete first link

  return catch;
}

// Create Element and Link, then attach to front, O(1)
Link *push(Link *list, node *pass, int flag) // if node is null, nothing added
{
  Link *head;

  if (pass == NULL)
    return list;

  // if empty list, create list
  if (list == NULL)
  {
    list = createLink(pass, flag); // O(1)
    return list;
  }

  // Add new head link
  head = createLink(pass, flag); // O(1)
  head->next = list;

  return head;
}

// Free all memory with Linked Stack, O(n)
void smelt(Link *list)
{
  Link *temp;

  if (list == NULL)
    return;

  while (list != NULL)
  {
    temp = list;
    list = list->next;

    free(temp->e);
    free(temp);
  }
}

// Specific function to free all memory from isPrePost, O(n)
void cleanUp(Element *x, Element *y, Link *a, Link *b)
{
  free(x);
  free(y);
  smelt(a);  // O(n)
  smelt(b);  // O(n)
}

// Moves to next node in pre/post list, O(n)
Element *increment(int p, Link **list)
{
  int check = 1;
  Element *popped;

  if (list == NULL)
    return NULL;

  while (check)
  {
    // Get first link (element)
    if ((popped = pop(list)) == NULL) // O(1)
    {
      check = 0;
      break;
    }
    // If node hasn't pushed children, do so
    if (popped->flag == 0)
    {
      if (p < 0) // If postList
        *list = push(*list, popped->n, 1);      // O(1)

      *list = push(*list, popped->n->right, 0); // O(1)
      *list = push(*list, popped->n->left, 0);  // O(1)

      if (p >= 0) // If preList
        *list = push(*list, popped->n, 1);      // O(1)

      free(popped);
    }
    else
      check = 0;
  }

  return popped;
}

// Returns 1 if x pre traversal is equal to y post traversal
// O(nk) where k = MAX{l,m} of list with most increments - ?
int isPrePost(node *x, node *y)
{
  Link *preList, *postList;
  Element *fresh, *fresh2;

  preList = createLink(x, 0);  // O(1)
  postList = createLink(y, 0); // O(1)

  // Begin comparisions
  while (1)
  {
    // Increment pre-order on x
    fresh = increment(1, &preList); // O(n)

    // Increment post-order on y
    fresh2 = increment(-1, &postList); // O(n)

    // If both reach end at same time, they're Kindred Ones
    if (fresh == NULL && fresh2 == NULL)
      break;
    // If only one is Null, stop checking before Seg fault
    else if (fresh == NULL || fresh2 == NULL)
    {
      cleanUp(fresh, fresh2, preList, postList); // O(n)
      return 0;
    }

    // If pre-x and post-y equals, continue checking
    if (fresh->n->data == fresh2->n->data)
    {
      free(fresh);
      free(fresh2);
    }
    // If not equal stop checking
    else
    {
      cleanUp(fresh, fresh2, preList, postList); // O(n)
      return 0;
    }
  }

  return 1;
}

// Returns 1 if the trees are reverses of each other's
// post-pre order traverses, O(nk)?
int kindredSpirits(node *a, node *b)
{
  return (isPrePost(a, b) || isPrePost(b, a));
}

double difficultyRating(void)
{
  return 4.5;
}

double hoursSpent(void)
{
  return 5;
}
