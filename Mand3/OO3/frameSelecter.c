#include "frameSelecter.h"
#include <stdlib.h>
#define LRUTIME 200

void frameSelectFifo(struct frame_table *ft, int npages, int nframes, int* freeFrame, void* data){
	struct FIFOData* fifdat = data;
	*freeFrame = fifdat->nextFrame;
	fifdat->nextFrame = (*freeFrame + 1) % nframes;
}

void frameSelectRand(struct frame_table *ft, int npages, int nframes, int* freeFrame, void* data){
	*freeFrame = lrand48() % nframes;
}

void frameSelectCust(struct frame_table *ft, int npages, int nframes, int* freeFrame, void* data){
	struct LRUData* LRUData = data;
	int f, frame, bits;
	
	//Find den page der har den laveste history, af pages der er mappet til frames.  
	unsigned int min = 0xffffffff;
	for(f = 0; f < nframes; f++){
		int page = ft->map[f];
		unsigned int hist = LRUData->page_history[page];
		if(hist <= min){
			*freeFrame = f;
			min = hist;
		}
	}	
}

struct LRUData* createLRUData(int pages){
	struct LRUData *LRUData;

	if (!(LRUData = malloc(sizeof (struct LRUData) ) ) ){
		printf("Fejl\n");
		return 0;
	}

	if (!(LRUData->page_history = malloc(sizeof (int) * pages) ) ){
		printf("Fejl\n");
		return 0;
	}

	if (!(LRUData->page_bits = malloc(sizeof (int) * pages) ) ){
		printf("Fejl\n");
		return 0;
	}

	return LRUData;
}

struct frame_table* createFrameTable(int frames){
	struct frame_table* ft;

	if(!(ft = malloc(sizeof (struct frame_table)))){
		printf("Fejl\n");
		return 0;
	}
	if(!(ft->map = malloc(sizeof (int) * frames))){
		printf("Fejl\n");
		return 0;
	}

	return ft;
}

void (*getFifo()) (struct frame_table*, int nframes, int npages, int*, void*){
	return &frameSelectFifo;
}

void (*getRand()) (struct frame_table*, int nframes, int npages, int*, void*){
	return &frameSelectRand;
}

void (*getCustom()) (struct frame_table*, int nframes, int npages, int*, void*){
	return &frameSelectCust;
}