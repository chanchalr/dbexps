#ifndef __DATA_H__
#define __DATA_H__
#include "syshead.h"
#include "pager.h"
#include "tree.h"

#define PAGE_SIZE        4096
#define ROWS_PER_PAGE    ((PAGE_SIZE)/(ROW_SIZE))
#define TABLE_MAX_ROWS   ((ROWS_PER_PAGE) * (TABLE_MAX_PAGES))
typedef struct Table__{
    uint32_t num_rows;
    Pager_t *pager;
}Table_t;

typedef struct Cursor__{
    Table_t *table;
    uint32_t row_num;
    bool     end_of_table;
}Cursor_t;

void serialize_row(Row_t *row,void *dst);
void deserialize_row(void *sr,Row_t *dest);
void *row_slot(Table_t *table,uint32_t row_num);
Table_t * db_open(char *filename);
void db_close(Table_t *t);
void print_row(Row_t *row);
Cursor_t *table_start(Table_t *table);
Cursor_t *table_end(Table_t *table);
void cursor_advance(Cursor_t *cursor);
void * cursor_value(Cursor_t *c);
#endif

