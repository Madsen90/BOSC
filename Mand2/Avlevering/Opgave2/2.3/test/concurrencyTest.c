#include <stdio.h>
#include <pthread.h>
#include "minunit.h"
#include "concurrentList.h"
#include <stdlib.h>

int tests_run = 0;

static void* addToList(void* l){
	ConcurrentList* list = l;
	list_add(l, node_new());
}

static void* remFromList(void* l){
	ConcurrentList* list = l;
	Node *n = list_remove(l);
	if (n == NULL){
		printf("%s\n", "removed null");
	}
}

static char * test_add() {
	pthread_t threads[10000];
	ConcurrentList *l;
	l = list_new();

	int i;
	for(i = 0; i < 10000; i++){
		pthread_create(&threads[i], NULL, &addToList, (void *)l);
	}

	for(i = 0; i < 10000; i++){
		pthread_join(threads[i],NULL);
	}


	Node *n = l->first;
	i = 0;
	while(n->next != NULL){
		n = n->next;
		i++;
	}

	mu_assert("error, len != 1000", l->len == 10000);
	mu_assert("error, nbOfNodes != 1000", i == 10000);
	free(l);
	return 0;	

}

static char * test_rem() {
	pthread_t threads[10000];
	ConcurrentList *l;
	l = list_new();

	int i;
	for(i = 0; i < 10100; i++){
		addToList((void*) l);
	}

	for(i = 0; i < 10000; i++){
		pthread_create(&threads[i], NULL, &remFromList, (void *)l);
	}

	for(i = 0; i < 10000; i++){
		pthread_join(threads[i],NULL);
	}

	Node *n = l->first;
	i = 0;
	while(n->next != NULL){
		n = n->next;
		i++;
	}

	mu_assert("error, len != 100", l->len == 100);
	mu_assert("error, nbOfNodes != 100", i == 100);
	free(l);
	return 0;	


}

static char * test_add_and_rem() {
	pthread_t threads[10000];
	ConcurrentList *l;
	l = list_new();
	int i = 0;
	int j = 1;

	while(i<10000){ //last case: i=9998 & j=9999
		pthread_create(&threads[i], NULL, &addToList, (void *)l);	
		pthread_create(&threads[j], NULL, &remFromList, (void *)l);
		i = i + 2;
		j = j + 2;	
	}
	for(i = 0; i < 10000; i++){
		pthread_join(threads[i],NULL);
	}

	Node *n = l->first;
	i = 0;
	while(n->next != NULL){
		n = n->next;
		i++;
	}

	printf("%s%d\n", "len:", l->len);
	printf("%s%d\n", "i:", i);
	free(l);

	return 0;

}
static char * all_tests() {
     mu_run_test(test_add);
     mu_run_test(test_rem);
     mu_run_test(test_add_and_rem);
     return 0;
 }

 int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
 }