#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

#include "sleep.h"
#include "list.h"

int bufferSize = 10;
int itemNumber = 0;
int itemsLeft;
int itemsTarget;

sem_t mutex, itemNumberMutex;
sem_t empty;
sem_t full;

List* l;

void* producer(void *);
void* consumer(void *);


int main(int argc, char const *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "usage: main.out <integer value>\n");
		return -1;
	}

	int arg_i = 1;
	while(arg_i < 4){
		if(atoi(argv[arg_i]) < 1){
			fprintf(stderr, "%d must be >= 1\n", atoi(argv[1]));
			return -1;
		}
		arg_i++;
	}

	itemsTarget = atoi(argv[1]);
	itemsLeft = itemsTarget;
	int producers = atoi(argv[2]);
	int consumers = atoi(argv[3]);
	int total = producers + consumers;

	//Seeding rand
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	
	//initialising semahpores
	sem_init(&mutex, 0, 1);
	sem_init(&itemNumberMutex, 0, 1);
	sem_init(&empty, 0, bufferSize);
	sem_init(&full, 0, 0);

	l = list_new();

	pthread_t tid[total];

	//Memory allokering fordi at hvis der bare bruges id = i
	//så får alle producers samme id, selvom jeg ikke forstår hvorfor	
	int i = 0;
	while(i < producers){
		int id = i; 
		pthread_create(&tid[i], NULL, producer, (void *) &id);
		i++;
	}

	while(i < total){
		int id = i - producers;
   		pthread_create(&tid[i], NULL, consumer, (void *) &id);
		i++;
	}

	i = -1;
	while(++i < total){
		pthread_join(tid[i], NULL);
	}

	free(l->first);
	free(l);

	return 0;
}


int getItemNumber(){
	int ret;
	sem_wait(&itemNumberMutex);
	ret = itemNumber;
	itemNumber++;
	sem_post(&itemNumberMutex);
	return ret;
}

int decItemsLeft(){
	int ret;
	sem_wait(&itemNumberMutex);
	ret = itemsLeft;
	itemsLeft--;
	sem_post(&itemNumberMutex);
	return ret;
}

void* producer(void * param){
	int id = * (int*) param;
	char item_str[] = "Item_";

	int itemId, numberOfItems;

	while(itemsTarget > (itemId = getItemNumber())){
		//Producing
		char buf[sizeof item_str + 1 + ((int) (itemId / 10))];
		snprintf(buf, sizeof buf, "Item_%d", itemId);
		Node* n = node_new_str(buf);

		//Critical secion
		sem_wait(&empty);
		sem_wait(&mutex);
		list_add(l, n);
		//Denne linje gør mutex semaforen nødvendig.
		//Kunne undlades ved at lade liste returnere len på add og remove
		//da list kan håndtere samtidig adgang
		numberOfItems = l->len;
		sem_post(&mutex);
		sem_post(&full);

		printf("Producer %d produced %s. Items in buffer %d (out of %d).\n", id, buf, numberOfItems, bufferSize);
		Sleep(1000);
	}
}

void* consumer(void* param){
	int id = * (int*) param;
	int itemId, numberOfItems;
	Node* n;

	while(0 < decItemsLeft()){
		//Critical secion
		sem_wait(&full);
		sem_wait(&mutex);
		n = list_remove(l);
		//Samme problem som producer
		numberOfItems = l->len;
		sem_post(&mutex);
		sem_post(&empty);

		printf("Consumer %d consumed %s. Items in buffer %d (out of %d).\n", id, n->elm, numberOfItems, bufferSize);
		Sleep(2000);
	}
}