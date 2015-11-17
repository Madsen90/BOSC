#include <stdio.h>
#include "minunit.h"
#include <stdlib.h>
    
#include "page_table.h"
#include "disk.h"
#include "frameSelecter.h"

#define FIFORUNS 50
#define FIFOFRAMES 10
#define FIFOPAGES 100

int tests_run = 0;
void (*frameSelecter)(struct page_table*, struct frame_table*, int*,  void*);

static void page_fault_handler_mock( struct page_table *pt, int page ){
	printf("%s\n", "THIS SHOULD NEVER HAPPEN! START OVER");
	//intentionally left blank
}

static char * test_FIFO() {
	int* returnFrame = malloc(sizeof(int));
	*returnFrame = 123456;
	printf("%d\n", returnFrame);
	printf("%d\n", *returnFrame);

	struct FIFOData* fifdat = malloc(sizeof(struct FIFOData));
	fifdat->nextFrame = 0;
	void* data = fifdat;

	struct page_table* mockPageTable = page_table_create( FIFOPAGES, FIFOFRAMES, page_fault_handler_mock);
	
	struct frame_table* mockFrameTable;
	
	int results[FIFORUNS];
	frameSelecter = getFifo();

	int i;
printf("1\n");
	for(i = 0; i < FIFORUNS; i++){
		frameSelecter(mockPageTable, mockFrameTable, returnFrame, data);
		results[i] = *returnFrame;
	}
printf("2\n");

	for(i = 0; i < FIFORUNS; i++){
		if(results[i] != (i%FIFOFRAMES))
			break;
	}

	//start of error message construction
	char error[44];
	char iAsChar = i + '0';
	char iModTenChar = (i%FIFOFRAMES) + '0';
	char resultChar = results[i] + '0';
	strcpy(error,"Error, expected result[");
	strncat(error, &iAsChar, 1);
	strcat(error, "] to be ");
	strncat(error, &iModTenChar, 1);
	strcat(error, " but was ");
	strncat(error, &resultChar, 1);
	//end of error message construction	



	page_table_delete(mockPageTable);
	printf("%s\n", "1");
	//printf("%s%d\n", "free addr:", &fifdat);
	//free(fifdat);


	mu_assert(error, i == FIFORUNS);
	return 0;
}

static char * all_tests() {
	mu_run_test(test_FIFO);
	//mu_run_test(test_custom);
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