#ifndef __PAGE_H__
#define __PAGE_H__
#include "syshead.h"
#define TABLE_MAX_PAGES  100
typedef struct  Pager__{
	int      fd;
	uint32_t file_length;
    uint32_t num_pages;
	void     *pages[TABLE_MAX_PAGES];
}Pager_t;

Pager_t *pager_open(char *filename);
void *get_page(Pager_t *pager,uint32_t page_num);
void pager_flush(Pager_t *pager,uint32_t page_num);
uint32_t get_unused_page(Pager_t *page);
#endif
