#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "page_table.h"

extern void frameSelectFifo(struct page_table *pt, int* freeFrame, int* oldPage, int* bits);

#endif