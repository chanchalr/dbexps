#ifndef __DATA_H__
#define __DATA_H__
#include "syshead.h"
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct Row__{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row_t;
#define size_of_attr(Struct,attr)sizeof(((Struct *)0)->attr)
const uint32_t ID_SIZE         = size_of_attr(Row_t, id);
const uint32_t USERNAME_SIZE   = size_of_attr(Row_t,username);
const uint32_t EMAIL_SIZE      = size_of_attr(Row_t,email);
const uint32_t ROW_SIZE        = ID_SIZE+USERNAME_SIZE+EMAIL_SIZE;
const uint32_t ID_OFFSET       = 0 
const uint32_t USERNAME_OFFSET = ID_OFFSET+ID_SIZE;
const uint32_t EMAIL_OFFSET    = USERNAME_OFFSET+USERNAME_SIZE;


#define TABLE_MAX_PAGES  100
const uint32_t PAGE_SIZE      = 4096;
const uint32_t ROWS_PER_PAGE  = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;
typedef struct Table__{
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
}Table_t;

void serialize_row(Row_t *row,void *dst);
void deserialize_row(void *sr,Row_t *dest);
void *row_slot(Table_t *table,uint32_t row_num);

#endif

