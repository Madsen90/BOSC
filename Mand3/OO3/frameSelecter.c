#include "frameSelecter.h"
#include <stdlib.h>
#define LRUTIME 200

void frameSelectFifo(struct page_table *pt, struct frame_table *ft, int* freeFrame, void* data){
	struct FIFOData* fifdat = data;
	int nframes = page_table_get_nframes(pt);
	*freeFrame = fifdat->nextFrame;
	fifdat->nextFrame = (*freeFrame + 1) % nframes;
}

void frameSelectRand(struct page_table *pt, struct frame_table *ft, int* freeFrame, void* data){
	int nframes;
	nframes = page_table_get_nframes(pt);
	*freeFrame = lrand48()%nframes;
}

void frameSelectCust(struct page_table *pt, struct frame_table *ft, int* freeFrame, void* data){
	struct LRUData* LRUData = data;
	int npages, nframes, p,f, frame, bits;
	nframes = page_table_get_nframes(pt);
	npages = page_table_get_npages(pt);

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

void (*getFifo()) (struct page_table*, struct frame_table*, int*, void*){
	return &frameSelectFifo;
}

void (*getRand()) (struct page_table*, struct frame_table*, int*, void*){
	return &frameSelectRand;
}

void (*getCustom()) (struct page_table*, struct frame_table*, int*, void*){
	return &frameSelectCust;
}