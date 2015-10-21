/******************************************************************************
   list.c

   Implementation of simple linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "concurrentList.h"


/* list_new: return a new list structure */
ConcurrentList *list_new(void)
{
  ConcurrentList *l;

  l = (ConcurrentList *) malloc(sizeof(ConcurrentList));
  l->len = 0;

  /* insert root element which should never be removed */
  l->first = l->last = (Node *) malloc(sizeof(Node));
  l->first->elm = NULL;
  l->first->next = NULL;
  pthread_mutex_init(&l->add_mutex, NULL);
  pthread_mutex_init(&l->rem_mutex, NULL);
  pthread_mutex_init(&l->len_mutex, NULL);
  return l;
}

/* list_add: add node n to list l as the last element */
void list_add(ConcurrentList *l, Node *n)
{
  //if(l->last != NULL) check is not necessary, as the root element is never removed
  pthread_mutex_lock(&l->add_mutex);
  l->last->next = n;
  l->last = n;
  pthread_mutex_unlock(&l->add_mutex);
  pthread_mutex_lock(&l->len_mutex);
  l->len++;
  pthread_mutex_unlock(&l->len_mutex);
}

/* list_remove: remove and return the first (non-root) element from list l */
Node *list_remove(ConcurrentList *l)
{
  Node *n;
  pthread_mutex_lock(&l->len_mutex);
  if(l->len > 0){
    l->len--;
    pthread_mutex_unlock(&l->len_mutex);
    pthread_mutex_lock(&l->rem_mutex);
    n = l->first->next;
    if(n != NULL){
      l->first->next = n->next;
    }
    pthread_mutex_unlock(&l->rem_mutex);
  }else{
    pthread_mutex_unlock(&l->len_mutex);
    n = NULL;
  }


  return n;
}

/* node_new: return a new node structure */
Node *node_new(void)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = NULL;
  n->next = NULL;
  return n;
}

/* node_new_str: return a new node structure, where elm points to new copy of s */
Node *node_new_str(char *s)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = (void *) malloc((strlen(s)+1) * sizeof(char));
  strcpy((char *) n->elm, s);
  n->next = NULL;
  return n;
}
