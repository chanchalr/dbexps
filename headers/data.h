#ifndef __DATA_H__
#define __DATA_H__
#include "syshead.h"
#include "pager.h"
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct Row__{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE+1];
    char email[COLUMN_EMAIL_SIZE+1];
}Row_t;
#define size_of_attr(Struct,attr) sizeof(((Struct *)0)->attr)
//const uint32_t ID_SIZE         = size_of_attr(Row_t, id);
#define ID_SIZE         size_of_attr(Row_t, id)
#define USERNAME_SIZE   size_of_attr(Row_t,username)
#define EMAIL_SIZE      size_of_attr(Row_t,email)
#define ROW_SIZE        (ID_SIZE+USERNAME_SIZE+EMAIL_SIZE)
#define ID_OFFSET       0
#define USERNAME_OFFSET (ID_OFFSET+ID_SIZE)
#define EMAIL_OFFSET    (USERNAME_OFFSET+USERNAME_SIZE)


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

