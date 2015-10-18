/******************************************************************************
   list.h 

   Header file with definition of a simple linked list.

******************************************************************************/

#ifndef _CONCURRENTLIST_H
#define _CONCURRENTLIST_H

#include <pthread.h>

/* structures */
typedef struct node {
  void *elm; /* use void type for generality; we cast the element's type to void type */
  struct node *next;
} Node;

typedef struct concurrentList {
  int len;
  Node *first;
  Node *last;
  pthread_mutex_t len_mutex;
  pthread_mutex_t add_mutex;
  pthread_mutex_t rem_mutex;
} ConcurrentList;

/* functions */
ConcurrentList *list_new(void);            /* return a new list structure */
void list_add(ConcurrentList *l, Node *n); /* add node n to list l as the last element */
Node *list_remove(ConcurrentList *l);      /* remove and return the first element from list l*/
Node *node_new(void);            /* return a new node structure */
Node *node_new_str(char *s);     /* return a new node structure, where elm points to new copy of string s */

#endif
