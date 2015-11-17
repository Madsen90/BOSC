/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
    
#include "page_table.h"
#include "disk.h"
#include "program.h"
#include "frameSelecter.h"



struct frame_table *ft;
struct disk *disk;
struct LRUData *LRUData;
char *physmem;

int diskWrites = 0, diskReads = 0, pageReq = 0, writeReq = 0, LRUFaults = 0;

void* fsData;

void (*frameSelecter)(struct page_table*, struct frame_table*, int*,  void*);


void print_mapping(struct page_table *pt){
	int npages = page_table_get_npages(pt);
	int nframes = page_table_get_nframes(pt);
	
	// 1. Find a free frame
	// a. If there is a free frame
	int p, frame, bits;
	printf("P - F - B - Pb\n");
	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, &bits);	

		printf("%d - %d - %d - %d\n", p, frame, bits, LRUData->page_bits[p]);
	}
	printf("Frame: \n" );

	for(p = 0; p < nframes; p++){
		printf("%d\n", ft->map[p]);	
	}
}

int findFreeFrame(struct page_table *pt, int* retFrame){
	int f, nframes = page_table_get_nframes(pt);

	for(f = 0; f < nframes; f++){
		if(ft->map[f] == -1){
			*retFrame = f;
			return 1;
		}
	}
	return 0;
}

void page_fault_handler( struct page_table *pt, int page )
{	

	pageReq++; //Statistik

	int bits, frame, history;
	page_table_get_entry(pt, page, &frame, &bits );

	//Markere at denne page er blevet efterspurgt i denne periode
	LRUData->page_history[page] = LRUData->page_history[page] |= 1 << 31;

	
	//Checking if this request is caused by a LRU
	if(bits == 0 && LRUData->page_bits[page] > 0){
		page_table_set_entry(pt, page, frame, LRUData->page_bits[page]);
		LRUData->page_bits[page] = 0;
		LRUFaults++;
		return;
	}

	//Check if this is a "write-request"
	if((bits & PROT_READ) == PROT_READ){
		page_table_set_entry(pt, page, frame, PROT_READ | PROT_WRITE );
		writeReq++;
		return;
	}
	
	// return;
	int npages, nframes;
	npages = page_table_get_npages(pt);
	nframes = page_table_get_nframes(pt);
	
	// 1. Find a frame
	// 	a. If there is a free frame
	int freeFrame;

	// 	b. If there is no free frame 
	// 		b1. Use a page-replacement algorithm to select a victim frame
	if(!findFreeFrame(pt, &freeFrame)){
		frameSelecter(pt, ft, &freeFrame, fsData);
		
		int tpFrame, bits, oldPage = ft->map[freeFrame];
//		printf("Freemframe: %d \n", freeFrame);
		page_table_get_entry(pt, oldPage, &tpFrame, &bits);
	
		//  b2. Write the victim frame to the diske; change the page and frame tables accordingly	
		if((bits & PROT_WRITE) == PROT_WRITE || 
			(LRUData->page_bits[oldPage] & PROT_WRITE) == PROT_WRITE ){
			//printf("write\n");
			disk_write(disk, oldPage, &physmem[freeFrame * PAGE_SIZE]);
			diskWrites++;
		}
		page_table_set_entry(pt, oldPage, 0, 0);
		LRUData->page_bits[oldPage] = 0;
	}

	// 2. Read the desired page into the selected frame; change the page and frame tables.
	
	ft->map[freeFrame] = page;
	
	disk_read(disk, page, &physmem[freeFrame * PAGE_SIZE]);
	page_table_set_entry(pt, page, freeFrame, PROT_READ);
    diskReads++;

	// 3. Continue the user process
	// print_mapping(pt);
	// printf("\n");
}
 
int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program = argv[4];

	//Initialising LRUData
	if(   !(LRUData = malloc(sizeof (struct LRUData)))
       || !(LRUData->page_history = malloc(sizeof (int) * npages))
       || !(LRUData->page_bits = malloc(sizeof (int) * npages))){
		printf("LRUData couldn't be allocated\n");
		return 1;
	}
	LRUData->timestamp = clock();
	
	//Initialising Frame table
	if(   !(ft = malloc(sizeof (struct frame_table)))
       || !(ft->map = malloc(sizeof (int) * nframes))){
		printf("Frame table couldn't be allocated\n");
		return 1;
	}
	int f;
	for(f = 0; f < nframes; f++){
		ft->map[f] = -1;
	}

	if (!strcmp(algorithm,"custom")){
		printf("%s\n", "Custom algorithm:");
		frameSelecter = getCustom();
		fsData = LRUData; 
	}
	else if (!strcmp(algorithm,"fifo")){
		struct FIFOData* fifdat = malloc(sizeof(struct FIFOData));

		fifdat->nextFrame = 0;
		fsData = (void*)fifdat;
		printf("%s\n", "Fifo algorithm:");
		frameSelecter = getFifo();
	}
	else if (!strcmp(algorithm,"rand"))
	{
		printf("%s\n", "Random algorithm:");
		frameSelecter = getRand();
	}
	else{
		printf("Algorithms to choose from are rand|fifo|custom\n");
		return 1;
	}


	disk = disk_open("myvirtualdisk",npages);

	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	} 
 	
 	char *virtmem = page_table_get_virtmem(pt);
	physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	}else {
		test_program(virtmem,npages*PAGE_SIZE);
//		fprintf(stderr,"unknown program: %s\n",argv[3]);
	}

	printf("PageRequests: %d\n", pageReq);
	printf("writeReq: %d\n", writeReq);
	printf("diskWrites: %d\n", diskWrites);
	printf("diskReads: %d\n", diskReads);
	printf("LRUFaults: %d\n", LRUFaults);

	//FREE FRAME TABLE AND LRUDATA


	page_table_delete(pt);
	disk_close(disk);

	return 0;
}