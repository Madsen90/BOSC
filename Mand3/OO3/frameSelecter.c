#include "frameSelecter.h"

int g = 0;

void frameSelectFifo(struct page_table *pt, int* freeFrame, int* oldPage, int* bits, void* data){
	int npages, nframes, p, frame;
	npages = page_table_get_npages(pt);
	nframes = page_table_get_nframes(pt);
	
	*freeFrame = g;
	
	g = (g+1) % nframes;
	
	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, bits);	

		if(frame == *freeFrame){
			*oldPage = p;
			return;
		}
	}

	printf("Should not be possible, didn't find page\n");
	abort();
}


void frameSelectRand(struct page_table *pt, int* freeFrame, int* oldPage, int* bits, void* data){
	int npages, p, frame;

	*freeFrame = 1;
	npages = page_table_get_npages(pt);

	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, bits);	

		if(frame == *freeFrame){
			*oldPage = p;
			return;
		}
	}

	printf("Should not be possible, didn't find page\n");
	abort();
}

void frameSelectCust(struct page_table *pt, int* freeFrame, int* oldPage, int* bits, void* data){
	int npages, p, frame;

	*freeFrame = 1;
	npages = page_table_get_npages(pt);

	for(p = 0; p < npages; p++){
		page_table_get_entry(pt, p, &frame, bits);	

		if(frame == *freeFrame){
			*oldPage = p;
			return;
		}
	}

	printf("Should not be possible, didn't find page\n");
	abort();
}

<<<<<<< HEAD
void* getFifo(){
	return &frameSelectFifo;
}

void* getRand(){
	return &frameSelectRand;
}

void* getCustom(){
=======
void (*getFifo()) (struct page_table*, int*, int*, int*, void*){
	return &frameSelectFifo;
}

void (*getRand()) (struct page_table*, int*, int*, int*, void*){
	return &frameSelectRand;
}

void (*getCustom()) (struct page_table*, int*, int*, int*, void*){
>>>>>>> dbff67bb7222af082fc449f837f6d3ccf0b8c504
	return &frameSelectCust;
}