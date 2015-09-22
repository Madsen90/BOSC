#include <pthread.h>
#include <stdio.h>

typedef struct td_ThreadData {
	int lower;
	int upper;
	double partialSum;
} ThreadData;

double sum = 0; //this data is shared by the thread(s)
void *runner(void *param); //threads call this function

int main(int argc, char const *argv[])
{

	if (argc != 3) {
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}
	if(atoi(argv[1]) < 0){
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
	}
	if(atoi(argv[2]) < 0){
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[2]));
	}


	int threads 	= atoi(argv[1]); 
	int upperLimit  = atoi(argv[2]);
	int sumSegment  = upperLimit / threads; 

	ThreadData threadData[threads]; 
	pthread_t tid[threads]; //Thread identifier
	pthread_attr_t attr[threads]; //set of thread attributes

	{
		int i;
		for(i = 0; i < threads; i++){
			threadData[i].lower 	 = i * sumSegment + 1;
			threadData[i].partialSum = 0;
			
			if(i+1 == threads)
				threadData[i].upper = upperLimit;
			else
				threadData[i].upper = i * sumSegment + sumSegment;
				
			
			//get the default attributes
			pthread_attr_init(&attr[i]);
			//create the thread
			pthread_create(&tid[i], &attr[i], runner, &threadData[i]);
		}
	}

	printf("---- \n");
	
	{
		int i;
		for(i = 0; i < threads; i++){
			pthread_join(tid[i], NULL);
			sum += threadData[i].partialSum;
		}
	}

	printf("sum = %G\n", sum);

	return 0;
}

//The thread will begin control in this function
void *runner(void *param){
	printf("Starting thread \n");

	ThreadData* data = param;
	
	int i;
	for(i = data->lower; i <= data->upper; i++){
		data->partialSum += i;
	}

	pthread_exit(0);
}