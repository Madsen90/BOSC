#include <stdio.h>
#include <pthread.h>
#include "minunit.h"
#include "list.h"
#include <stdlib.h>

int tests_run = 0;

static void* addToList(void* l){

	List* list = l;
	list_add(l, node_new());
}

static char * test_add() {
	printf("%s\n", "1");
	pthread_t threads[1000];

	printf("%s\n", "2");
	List *l;

	printf("%s\n", "3");
	l = list_new();

	printf("%s\n", "4");

	int i;
	printf("%s\n", "5");
	for(i = 0; i < 1000; i++){

		printf("%s%d\n", "i=", i);
		pthread_create(&threads[i], NULL, &addToList, (void *)l);
	}

	printf("%s\n", "6");
	for(i = 0; i < 1000; i++){
		printf("%s%d\n", "i2=", i);
		pthread_join(threads[i],NULL);
	}

	printf("%s\n", "7");
	mu_assert("error, len != 1000", l->len == 1000);

	printf("%s\n", "8");
	Node *n = l->first;
	i = 0;
	while(n->next == NULL){
		n = n->next;
		i++;
	}

	printf("%s\n", "9");
	mu_assert("error, nbOfNodes != 1000", i == 1000);

	printf("%s\n", "10");
	return 0;	

}
static char * all_tests() {
     mu_run_test(test_add);
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