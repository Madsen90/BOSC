#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "page_table.h"

(void *(f)(struct page_table*, int*, int*, int* )) getFifo();
(void *(f)(struct page_table*, int*, int*, int* )) getRand();
(void *(f)(struct page_table*, int*, int*, int* )) getCustom();

#endif