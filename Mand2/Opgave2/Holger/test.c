#include <pthread.h>
#include <stdio.h>
#include "list.h"
#include <stdlib.h>

pthread_mutex_t mutex;

typedef struct td_ThreadData {
	int i;
	List* l;
} ThreadData;

//Evaluation funtion for actually counting the numbers of elements
int listCount(List *list);

//Methods for manipulating the list
void * listAdder(void* param);
void * listRemover(void* param);
void * listBoth(void* param);

//listThread calls threadStarter with the given function f
//and evaluates if the list is in the expected state after executing
int listTest(int threads, int actions, List* list, int expected, void* (*f)(void*));

//Thread starter starts threads and gives them list and actions as paramteres
//*f is a function-pointer to the function to be executed
void threadStarter(int threads, int actions, List* list, void* (*f)(void*));


int main(int argc, char const *argv[])
{
	pthread_mutex_init(&mutex, NULL);

	if (argc != 3) {
		fprintf(stderr, "usage:./testfifo <integer value>:#thread <integer value>:#actions\n");
		return -1;
	}

	if(atoi(argv[1]) < 1){
		fprintf(stderr, "%d must be >= 1\n", atoi(argv[1]));
		return -1;
	}
	if(atoi(argv[2]) < 1){
		fprintf(stderr, "%d must be >= 1\n", atoi(argv[2]));
		return -1;
	}

	int threads = atoi(argv[1]);
	int actions = atoi(argv[2]);

	List* l = list_new();
	
	if(!listTest(threads, actions, l, threads * actions, listAdder)){
		printf("Addtest failed\n");
		return -1;
	}else{
		printf("Addtest Succeeded\n");
	}

	if(!listTest(threads, actions, l, 0, listRemover)){
		printf("Removetest failed\n");
		return -1;
	}else{
		printf("Removetest Succeeded\n");
	}

	actions -= (actions % 2 == 1) ? 1 : 0;

	if(!listTest(threads, actions, l, 0, listBoth)){
		printf("Bothtest failed\n");
		return -1;
	}else{
		printf("Bothtest Succeeded\n");
	}

	return 0;
}

int listCount(List *list){
	int count = 0;
	Node* n = list->first;
	while(n->next != NULL){
		n = n->next;
		count++;
	}

	return count;
}
 
void threadStarter(int threads, int actions, List* list, void* (*f)(void*)){
	int i = -1;
	pthread_t tid[threads];
	ThreadData threadDatas[threads];

	while(++i < threads){
		ThreadData td = threadDatas[i];
		td.l = list;
		td.i = actions;
		pthread_create(&tid[i], NULL, f, (void *) &td);
	}

	i = -1;
	
	while(++i < threads){
		pthread_join(tid[i], NULL);
	}
}

void* listAdder(void *param){
	ThreadData* data = param;

	//Unpacking for performance, had some weird performance while dereferencing
	int actions = data->i;
	List* l = data->l;

	int i = -1;
	while(++i < actions){
		list_add(l, node_new());
	}
}

void* listRemover(void *param){
	ThreadData* data = param;

	int actions = data->i;
	List* l = data->l;

	int i = -1;
	while(++i < actions){
		Node* n = list_remove(l);
		if(n == NULL){
			i--;
			continue;
		}

		free(n);
	}
}


//Lidt noget rod, resultatet af at prøve at generalisere test med funktionspointere
//Hvis man var rigtig ambitøs, så forsøgte man at stoppe trådene på random tidspunkter
//For at tjekke at data'en var konsistent.
int i = 0;
int mutexI(){
	int ret;
	pthread_mutex_lock(&mutex);
	i = (i + 1) % 2;
	ret = i;
	pthread_mutex_unlock(&mutex);
	return ret;
}
void* listBoth(void *param){
	int i = mutexI();

	if(i == 0){
		listAdder(param);
	}else{
		listRemover(param);
	}
}

int listTest(int threads, int actions, List *list, int expected, void* (*f)(void*)){
	threadStarter(threads, actions, list, f);

	int count = listCount(list);

	if(list->len != expected){
		printf("List has length-property: %d. Expected: %d", list->len, expected);
		return -1;
	}
	
	if(count != expected){
		printf("List has elements: %d. Expected: %d", count, expected);
		return -1;
	}
	
	return 1;
}