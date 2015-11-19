#include <stdio.h>
#include "minunit.h"
#include <stdlib.h>
#include <math.h>
    
#include "page_table.h"
#include "disk.h"
#include "frameSelecter.h"

#define FIFORUNS 200
#define FIFOFRAMES 10
#define FIFOPAGES 100

int tests_run = 0;
void (*frameSelecter)(struct frame_table*, int, int, int*, void*);

static char * test_FIFO() {
	int returnFrame;

	struct FIFOData* fifdat = malloc(sizeof(struct FIFOData));
	fifdat->nextFrame = 0;
	void* data = fifdat;

	//mock
	struct frame_table* mockFrameTable;
	
	int results[FIFORUNS];
	frameSelecter = getFifo();

	int i;
	for(i = 0; i < FIFORUNS; i++){
		frameSelecter( mockFrameTable, FIFOPAGES, FIFOFRAMES, &returnFrame, data);
		results[i] = returnFrame;
	}

	for(i = 0; i < FIFORUNS; i++){
		if(results[i] != (i%FIFOFRAMES))
			break;
	}
	//start of error message construction
	char error[44];
	char iAsChar[3];
	sprintf(iAsChar,"%d", i);
	char iModTenChar = (i%FIFOFRAMES) + '0';
	char resultChar = results[i] + '0';
	strcpy(error,"Error, expected result[");
	strncat(error, &iAsChar, 3);
	strcat(error, "] to be ");
	strncat(error, &iModTenChar, 1);
	strcat(error, " but was ");
	strncat(error, &resultChar, 1);
	//end of error message construction	

	free(fifdat);

	mu_assert(error, i == FIFORUNS);
	return 0;
}

static char * test_custom() {
	frameSelecter = getCustom();
	struct LRUData* LRUData;
	struct frame_table* ft;

	 int freeFrame;
 
	LRUData = createLRUData(5);
	ft = createFrameTable(3);


	LRUData->page_history[0] = 0x8000000;
	LRUData->page_history[1] = 0xf000000;
	LRUData->page_history[2] = 0x4000000;
	LRUData->page_history[3] = 0x2300000;
	LRUData->page_history[4] = 0x5000000;

	ft->map[0] = 0;
	ft->map[1] = 1;
	ft->map[2] = 2;

	frameSelecter(ft, 5, 3, &freeFrame, (void*) LRUData );

	mu_assert("Selected wrong simple", freeFrame == 2);

	ft->map[0] = 0;
	ft->map[1] = 4;
	ft->map[2] = 3;

	frameSelecter(ft, 5, 3, &freeFrame, (void*) LRUData );
	mu_assert("Selected wrong crossed frame table", freeFrame == 2);

	LRUData->page_history[0] = 0;
	LRUData->page_history[1] = 0;
	LRUData->page_history[2] = 0;
	LRUData->page_history[3] = 0;
	LRUData->page_history[4] = 0;
	ft->map[0] = 3;
	ft->map[2] = 2;

	frameSelecter(ft, 5, 3, &freeFrame, (void*) LRUData );
	mu_assert("Empty history error", freeFrame == 2);

	LRUData->page_history[0] = 0xffffffff;
	LRUData->page_history[1] = 0xffffffff;
	LRUData->page_history[2] = 0xffffffff;
	LRUData->page_history[3] = 0xffffffff;
	LRUData->page_history[4] = 0xffffffff;

	frameSelecter(ft, 5, 3, &freeFrame, (void*) LRUData );
	mu_assert("Full history error", freeFrame == 2);

	free(ft->map);
	free(ft);

	ft = createFrameTable(1);
	ft->map[0] = 4;

	frameSelecter(ft, 5, 1, &freeFrame, (void*) LRUData );
	mu_assert("One frame memory", freeFrame == 0);

	free(ft->map);
	free(ft);
	free(LRUData->page_history);
	free(LRUData->page_bits);
	free(LRUData);
	return 0;
}

static char * all_tests() {
	mu_run_test(test_FIFO);
	mu_run_test(test_custom);
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