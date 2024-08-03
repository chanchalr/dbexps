#ifndef __DATA_H__
#define __DATA_H__
#include "syshead.h"
#include "pager.h"
#include "tree.h"

#define PAGE_SIZE        4096
typedef struct Table__{
    Pager_t *pager;
    uint32_t root_page_num;
}Table_t;

typedef struct Cursor__{
    Table_t *table;
    uint32_t page_num;
    uint32_t cell_num;
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
Cursor_t *table_find(Table_t *t,uint32_t key_to_insert);
void cursor_advance(Cursor_t *cursor);
void * cursor_value(Cursor_t *c);
void leaf_node_insert(Cursor_t *c,uint32_t key, Row_t *value);
void leaf_node_split_and_insert(Cursor_t *c,uint32_t key,Row_t *value);
Cursor_t* leaf_node_find(Table_t* table, uint32_t page_num, uint32_t key);
void create_new_root(Table_t *t,uint32_t new_page_num);
#endif

