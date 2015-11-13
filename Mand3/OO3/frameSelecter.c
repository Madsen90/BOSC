#include "frameSelecter.h"
void frameSelectFifo(struct page_table *pt, int* freeFrame, int* oldPage, int* bits, void* data){
	int npages, p, frame;
	
	fifdata* d = (fifdata*) data;

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

getFifo(){
	return &frameSelectFifo;
}

getRand(){
	return &frameSelectRand;
}

getCustom(){
	return &frameSelectCust;
}