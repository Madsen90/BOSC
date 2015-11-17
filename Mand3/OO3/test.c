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

static void page_fault_handler_mock( struct page_table *pt, int page ){
	printf("%s\n", "THIS SHOULD NEVER HAPPEN! START OVER");
	//intentionally left blank
}

static char * test_FIFO() {
	// int returnFrame;

	// struct FIFOData* fifdat = malloc(sizeof(struct FIFOData));
	// fifdat->nextFrame = 0;
	// void* data = fifdat;

	// //mocks
	// struct page_table* mockPageTable = page_table_create( FIFOPAGES, FIFOFRAMES, page_fault_handler_mock);
	// struct frame_table* mockFrameTable;
	
	// int results[FIFORUNS];
	// frameSelecter = getFifo();

	// int i;
	// for(i = 0; i < FIFORUNS; i++){
	// //	frameSelecter(mockPageTable, mockFrameTable, &returnFrame, data);
	// 	results[i] = returnFrame;
	// }

	// for(i = 0; i < FIFORUNS-1; i++){
	// 	if(results[i] != (i%FIFOFRAMES))
	// 		break;
	// }
	// //start of error message construction
	// char error[44];
	// char iAsChar[3];
	// sprintf(iAsChar,"%d", i);
	// char iModTenChar = (i%FIFOFRAMES) + '0';
	// char resultChar = results[i] + '0';
	// strcpy(error,"Error, expected result[");
	// strncat(error, &iAsChar, 3);
	// strcat(error, "] to be ");
	// strncat(error, &iModTenChar, 1);
	// strcat(error, " but was ");
	// strncat(error, &resultChar, 1);
	// //end of error message construction	

	// page_table_delete(mockPageTable);
	// free(fifdat);

	// mu_assert(error, i == FIFORUNS);
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

	mu_assert("Selected wrong crossed frame table", freeFrame == 3);

	return 0;
}

static char * all_tests() {
	//mu_run_test(test_FIFO);
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