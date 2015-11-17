#include "frameSelecter.h"

#define LRUTIME 100

void frameSelectFifo(struct page_table *pt, int* freeFrame, void* data){
	printf("1.1\n");
	struct FIFOData* fifdat = data;
	printf("1.2\n");
	int nframes = page_table_get_nframes(pt);
	printf("1.3\n");
	printf("%d\n", freeFrame);
	*freeFrame = fifdat->nextFrame;
	printf("1.4\n");
	fifdat->nextFrame = (*freeFrame + 1) % nframes;
	printf("1.5\n");

}

void frameSelectRand(struct page_table *pt, struct frame_table *ft, int* freeFrame, void* data){
	// int npages, p, frame;

	// *freeFrame = 1;
	// npages = page_table_get_npages(pt);

	// for(p = 0; p < npages; p++){
	// 	page_table_get_entry(pt, p, &frame, bits);	

	// 	if(frame == *freeFrame){
	// 		*oldPage = p;
	// 		return;
	// 	}
	// }

	// printf("Should not be possible, didn't find page\n");
	// abort();
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
		if(hist < min){
			*freeFrame = f;
			min = hist;
		}

	}

	//Skal ikke ligge her, men sætter alle skriveretigheder til 0 og bitshifter alt page_history
	//Burde nok ikke kun kunne blive triggered af en efterspørgsel på en ny side
	double c = clock();
	if(c - LRUData->timestamp > LRUTIME){
		LRUData->timestamp = c;
		for(p = 0; p < npages; p++){
			LRUData->page_history[p] = LRUData->page_history[p]>>1; 
			page_table_get_entry(pt, p, &frame, &bits);
			bits = (LRUData->page_bits[p] > bits) ? LRUData->page_bits[p] : bits;
			LRUData->page_bits[p] = bits; 
			page_table_set_entry(pt, p, frame, 0);
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