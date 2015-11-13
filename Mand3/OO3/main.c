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
   
#include "page_table.h"
#include "disk.h"
#include "program.h"
#include "frameSelecter.h"

struct disk *disk;
char *physmem;

void (*frameSelecter)(struct page_table*, int*, int*, int*);

typedef struct FIFOdata{
	int* 
}


void print_mapping(struct page_table *pt){
	int npages = page_table_get_npages(pt);
	
	// 1. Find a free frame
	// a. If there is a free frame
	int p, frame, bits;
	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, &bits);	

		printf("%d - %d\n", p, frame);
	}
}

int findFreeFrame(struct page_table *pt, int* retFrame){
	int npages, nframes;
	npages = page_table_get_npages(pt);
	nframes = page_table_get_nframes(pt);

	int frames[nframes];
	int p, f, frame, bits;

	for(f = 0; f < nframes; f++){
		frames[f] = 0;
	}

	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, &bits);	

		if(bits > 0){
			frames[frame] = 1;
		}
	}

	for(f = 0; f < nframes; f++){
		if(frames[f] == 0){
			*retFrame = f;
			return 1;
		}
	}
	return 0;
}

void page_fault_handler( struct page_table *pt, int page )
{
	int bits, frame;
	page_table_get_entry(pt, page, &frame, &bits );

	//Check if this is a "write-request"
	if(bits & PROT_READ == PROT_READ){
		page_table_set_entry(pt, page, frame, PROT_READ | PROT_WRITE );
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
		int oldPage, bits;
		frameSelecter(pt, &freeFrame, &oldPage, &bits);

		//  b2. Write the victim frame to the diske; change the page and frame tables accordingly	
		if(bits & PROT_WRITE == PROT_WRITE){
			disk_write(disk, oldPage, &physmem[freeFrame * PAGE_SIZE]);
			page_table_set_entry(pt, oldPage, 0, 0);
		}
	}
	
	// 2. Read the desired page into the selected frame; change the page and frame tables.
	disk_read(disk, page, &physmem[freeFrame * PAGE_SIZE]);
	page_table_set_entry(pt, page, freeFrame, PROT_READ);

	// 3. Continue the user process
	printf("SEG ERROR, page: %d\n", page);
}
 
int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];

	frameSelecter = getFifo();

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

	// int i, j;
	// printf("Virt mem %d:\n", PAGE_SIZE);
	// for(i = 0; i < npages; i++){
	// 	printf("Page %d: ", i);
	// 	for(j = 0; j * 2048 < PAGE_SIZE; j++){
	// 		printf("%d ", virtmem[i * PAGE_SIZE + j * 2048]);
	// 	}
	// 	printf("\n");
	// }

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}