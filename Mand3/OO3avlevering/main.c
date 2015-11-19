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

#define LRUTIME 40

//ressources
struct frame_table *ft;
struct disk *disk;
struct LRUData *LRUData;
char *physmem;


//statisics
int diskWrites = 0, diskReads = 0, pageReq = 0, writeReq = 0, LRUFaults = 0;

//frameselection:
void* fsData;

void (*frameSelecter)(struct frame_table*, int, int, int*,  void*);

char *int2bit(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);
    int i;
    for (i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

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

void standard_page_fault_handler( struct page_table * pt, int page ){
	pageReq++; //Statistik

	int bits, frame, history;
	page_table_get_entry(pt, page, &frame, &bits );
	
	int npages, nframes;
	npages = page_table_get_npages(pt);
	nframes = page_table_get_nframes(pt);


	//Check if this is a "write-request"
	if((bits & PROT_READ) == PROT_READ){
		page_table_set_entry(pt, page, frame, PROT_READ | PROT_WRITE );
		writeReq++;
		return;
	}
	
	// 1. Find a frame
	// 	a. If there is a free frame
	int freeFrame;

	// 	b. If there is no free frame 
	// 		b1. Use a page-replacement algorithm to select a victim frame
	if(!findFreeFrame(pt, &freeFrame)){
		frameSelecter(ft, npages, nframes,&freeFrame, fsData);
		
		int tempFrame, bits, oldPage = ft->map[freeFrame];
		page_table_get_entry(pt, oldPage, &tempFrame, &bits);
	
		//  b2. Write the victim frame to the diske; update the page and frame tables accordingly	
		if((bits & PROT_WRITE) == PROT_WRITE ){
			disk_write(disk, oldPage, &physmem[freeFrame * PAGE_SIZE]);
			diskWrites++;
		}
		page_table_set_entry(pt, oldPage, 0, 0);
	}

	// 2. Read the desired page into the selected frame; change the page and frame tables.
	ft->map[freeFrame] = page;
	
	disk_read(disk, page, &physmem[freeFrame * PAGE_SIZE]);
	page_table_set_entry(pt, page, freeFrame, PROT_READ);
    diskReads++;

	// 3. Continue the user process
}

void LRU_page_fault_handler( struct page_table * pt, int page ){
	pageReq++; //Statistik

	int bits, frame, history;
	page_table_get_entry(pt, page, &frame, &bits );

	
	int npages, nframes;
	npages = page_table_get_npages(pt);
	nframes = page_table_get_nframes(pt);

    //saetter alle skriveretigheder til 0 og bitshifter alt page_history
	int p, tempbits, tempFrame;
	double c = clock(); 
	if(c - LRUData->timestamp > LRUTIME){
		for(p = 0; p < npages; p++){
			// char print[33];
			// print[32] = '\0';
			// int2bit(LRUData->page_history[p], print, 32);	
			//printf("%d: %s\n", p, print);
			LRUData->page_history[p] = LRUData->page_history[p]>>1; 
			page_table_get_entry(pt, p, &tempFrame, &tempbits);
			LRUData->page_bits[p] = (LRUData->page_bits[p] > tempbits) ? LRUData->page_bits[p] : tempbits;
			page_table_set_entry(pt, p, tempFrame, 0);
		}
		LRUData->timestamp = c;
	}
	//printf("\n");
	
	//Markere at denne page er blevet efterspurgt i perioden, ved at saette leftmost bit til 1
	LRUData->page_history[page] = LRUData->page_history[page] | (0x8000000); 
	
	//Checking if this request is caused by a LRU reset
	if(bits == 0 && LRUData->page_bits[page] > 0){
	
		page_table_set_entry(pt, page, frame, LRUData->page_bits[page]);
		//LRUData->page_bits[page] = 0;
		LRUFaults++;
		return;
	}

	//Check if this is a "write-request"
	if((bits & PROT_READ) == PROT_READ){
		page_table_set_entry(pt, page, frame, PROT_READ | PROT_WRITE );
		writeReq++;
		return;
	}
	
	// 1. Find a frame
	// 	a. If there is a free frame
	int freeFrame;

	// 	b. If there is no free frame 
	// 		b1. Use a page-replacement algorithm to select a victim frame
	if(!findFreeFrame(pt, &freeFrame)){
		frameSelecter(ft, npages, nframes,&freeFrame, fsData);
		
		int bits, oldPage = ft->map[freeFrame];
		page_table_get_entry(pt, oldPage, &tempFrame, &bits);
	
		// b2. Write the victim frame to the diske; update the page and frame tables accordingly	
		if((bits & PROT_WRITE) == PROT_WRITE || 
		   (LRUData->page_bits[oldPage] & PROT_WRITE) == PROT_WRITE ){

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
}
 
int main( int argc, char *argv[] )
{
	srand48(time(NULL));
	int freeLRU = 0;
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program = argv[4];
	
	//Initialising Frame table
	if(!(ft = createFrameTable(nframes))){
		printf("Frame table couldn't be allocated\n");
		return 1;
	}
	int f;
	for(f = 0; f < nframes; f++){
		ft->map[f] = -1;
	}
	struct page_table* pt;

	if (!strcmp(algorithm,"custom")){
		//printf("%s\n", "Custom algorithm - LRU:");
		
		//Initialising LRUData
		if(! (LRUData = createLRUData(npages))){
			printf("LRUData couldn't be allocated\n");
			return 1;
		}
		LRUData->timestamp = clock();


		//setting frameselecter
		frameSelecter = getCustom();
		fsData = LRUData; 

		//setting pagetable
		pt = page_table_create( npages, nframes, LRU_page_fault_handler );

		//eventually we should free LRU:
		freeLRU = 1;
	}
	else if (!strcmp(algorithm,"fifo")){
		struct FIFOData* fifdat = malloc(sizeof(struct FIFOData));

		fifdat->nextFrame = 0;
		fsData = (void*)fifdat;
		//printf("%s\n", "Fifo algorithm:");
		frameSelecter = getFifo();

		//setting pagetable
		pt = page_table_create( npages, nframes, standard_page_fault_handler );
	}
	else if (!strcmp(algorithm,"rand"))
	{
		//printf("%s\n", "Random algorithm:");
		frameSelecter = getRand();

		//setting pagetable
		pt = page_table_create( npages, nframes, standard_page_fault_handler );
	}else if (!strcmp(algorithm,"randopt"))
	{
		//printf("%s\n", "Random algorithm:");
		frameSelecter = getRandOpt();

		//setting pagetable
		pt = page_table_create( npages, nframes, standard_page_fault_handler );

		//setting data
		struct RANDData* dat = malloc(sizeof(struct RANDData));
		dat->pt = pt;
		fsData = (void*)dat;
	}
	else{
		printf("Algorithms to choose from are rand|fifo|custom|randopt\n");
		return 1;
	}

	disk = disk_open("myvirtualdisk",npages);

	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}
	
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	} 
 	
 	char *virtmem = page_table_get_virtmem(pt);
	physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
//		printf("sort:\n");
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
//		printf("scan:\n");
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
//		printf("focus:\n");
		focus_program(virtmem,npages*PAGE_SIZE);

	}else if(!strcmp(program,"test")){
		test_program(virtmem,npages*PAGE_SIZE);

	} else{
		fprintf(stderr,"unknown program: %s\n",argv[3]);		
	}

 //printf("PageRequests: %d\n", pageReq);
 //printf("writeReq: %d\n", writeReq);
printf("%d;", diskWrites);
printf("%d\n", diskReads);
//printf("LRUFaults: %d\n", LRUFaults);

	//freeing mem
	free(ft->map);
	free(ft);
	if(freeLRU){
		free(LRUData->page_history);
		free(LRUData->page_bits);
		free(LRUData);
	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}