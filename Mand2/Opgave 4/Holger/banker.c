#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

// Global variables
int m, n;
//Creating one teststate for less memory allocation and freeing
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  // add randomness
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

int lessOrEqual(int* a1, int* a2, int n){
  int i;
  for(i = 0; i < n; i++){
    if(a1[n] > a2[n])
      return 0;
  }
  return 1;
}


/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
  pthread_mutex_lock(&state_mutex);
  int ret = 0;
  //Step 1
  if(!lessOrEqual(request, s->need[i], n)){
    exit (-1);
  }
  
  if(lessOrEqual(request, s->available, n)){
    int j;

    for(j = 0; j < n; j++){
      s->available[j] += -request[j];
      s->allocation[j] +=  request[j];
      s->need[i][j] += -request[j];
    }
    
    if(!safeState()){
      //reverting
      for(j = 0; j < n; j++){
        s->available[j] += request[j];
        s->allocation[j] += -request[j];
        s->need[i][j] += request[j];
      } 
    }else{
      ret = 1;
    }
  }
  
  pthread_mutex_unlock(&state_mutex);
  return ret;
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
  pthread_mutex_lock(&state_mutex);
  int j;
  for(j = 0; j < n; j++){
    s->available[j] += request[j];
    s->allocation[i][j] -= request[j];
  }
  pthread_mutex_unlock(&state_mutex);
}

int safeState(){
  int work[n];
  int finish[m];

  //Step 1
  int i, j;
  for(i = 0; i < n; i++){
    work[i] = s->available[i];
  }

  for(i = 0; i < m; i++){
    finish[i] = 0;
  }


  while(1){
    for(i = 0; i < m; i++){
      //Step 2
      if(!finish[i] && lessOrEqual(s->need[i], work, n))
      {
        //step 3
        for(j = 0; j < n; j++){
          work[j] += s->allocation[i][j];
        }
        finish[i] = 1;
      }else{
        //Kunne optimeres
        break;
      }
    }

    //step 4
    for(i = 0; i < m; i++)
      if(!finish[i]){
        return 0;
      }

    return 1;
    
  }
  return 0;
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      printf("%d\n", s->need[i][j]);
      request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      printf("%d\n", s->allocation[i][j]);
      request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Releasing resources.\n",i);
}

/* Threads starts here */
void *process_thread(void *param)
{
  /* Process number */
  int i = (int) (long) param, j;
  /* Allocate request vector */
  int *request = malloc(n*sizeof(int));
  while (1) {
    /* Generate request */
    generate_request(i, request);
    while (!resource_request(i, request)) {
      printf("%s\n", "Waiting");
      /* Wait */
      Sleep(100);
    }
    /* Generate release */
    generate_release(i, request);
    /* Release resources */
    resource_release(i, request);
    /* Wait */
    Sleep(1000);
  }
  free(request);
}
int main(int argc, char* argv[])
{
  /* Get size of current state as input */
  int i, j;
  printf("Number of processes: ");
  scanf("%d", &m);
  printf("Number of resources: ");
  scanf("%d", &n);
 
  // Allocate memory for state 
  s = malloc(sizeof(State));
  s->resource   = malloc(m * sizeof(int));
  s->available  = malloc(m * sizeof(int));
  allocate_matrix_int(&s->max, m, n);
  allocate_matrix_int(&s->allocation, m, n);
  allocate_matrix_int(&s->need, m, n);


  /* Get current state as input */
  printf("%s\n","Resource vector: ");
  
  for(i = 0; i < n; i++)
    scanf("%d", &s->resource[i]);
  printf("%s\n","Enter max matrix: ");
  for(i = 0;i < m; i++)
    for(j = 0;j < n; j++)
      scanf("%d", &s->max[i][j]);
  printf("%s\n","Enter allocation matrix: ");
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++) {
      scanf("%d", &s->allocation[i][j]);
    }
  printf("\n");

  /* Calcuate the need matrix */
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      s->need[i][j] = s->max[i][j]-s->allocation[i][j];

  /* Calcuate the availability vector */
  for(j = 0; j < n; j++) {
    int sum = 0;
    for(i = 0; i < m; i++)
      sum += s->allocation[i][j];
    s->available[j] = s->resource[j] - sum;
  }

  // Output need matrix and availability vector 
  printf("Need matrix:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++)
      printf("%d  ",s->need[i][j]);
    printf("\n");
  }
  printf("Availability vector:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < n; j++)
    printf("%d  ",s->available[j]);
  printf("\n");

  /* If initial state is unsafe then terminate with error */
  if(!safeState()){
    printf("Initial state unsafe\n");
    exit(-1);
  }

  /* Seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  
  /* Create m threads */
  pthread_t *tid = malloc(m*sizeof(pthread_t));
  for (i = 0; i < m; i++)
    pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
  
  /* Wait for threads to finish */
  pthread_exit(0);
  free(tid);
  /* Free state memory */
}
