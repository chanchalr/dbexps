#include "data.h"
#include "syshead.h"

void serialize_row(Row_t *src, void *dest){
    memcpy(dest+ID_OFFSET,&(src->id),ID_SIZE);
    memcpy(dest+USERNAME_OFFSET,&(src->username),USERNAME_SIZE);
    memcpy(dest+EMAIL_OFFSET,&(src->email),EMAIL_SIZE);

}

void deserialize_row(void *src,Row_t *dest){
    memcpy(&(dest->id),src+ID_OFFSET,ID_SIZE);
    memcpy(&(dest->username),src+USERNAME_OFFSET,USERNAME_SIZE);
    memcpy(&(dest->email),src+EMAIL_OFFSET,EMAIL_SIZE);
}

void *row_slot(Table_t *table,uint32_t row_num){
    uint32_t page_num = row_num/ROWS_PER_PAGE;
    void *page = table->pages[page_num];
    if(page == NULL){
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page+byte_offset;

}


Table_t new_table(){
    Table_t *table;
    int i = 0;
    table = (Table_t *)malloc(sizeof(Table_t));
    for(i=0;i<TABLE_MAX_PAGES;i++){
        table->pages[i] = NULL;
    }
    table->num_rows = 0;
    return table;

}

void free_table(Table_t *t){
    int i = 0;
    for(i=0;i<TABLE_MAX_PAGES;i++){
        free(t->pages[i]);
    }
    free(t);
}

void print_row(Row_t *row){
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}
