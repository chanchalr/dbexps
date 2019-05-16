#include "data.h"
#include "syshead.h"
#include "pager.h"

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
    uint32_t page_num = 0;
    page_num = row_num/ROWS_PER_PAGE;
    void *page = get_page(table->pager,page_num);

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page+byte_offset;

}


Table_t *db_open(char *filename){
    Table_t *table = NULL;
    Pager_t *pager = NULL;
    pager = pager_open(filename);
    table = (Table_t *)malloc(sizeof(Table_t));
    table->num_rows = pager->file_length/(ROW_SIZE);
    table->pager = pager;
    return table;

}


void db_close(Table_t *table){
    uint32_t i = 0;
    uint32_t  num_additional_rows = 0;
    Pager_t *pager = table->pager;
    uint32_t num_full_pages =  table->num_rows/(ROWS_PER_PAGE);
    for(i =0;i<num_full_pages;i++){
        if(pager->pages[i] == NULL){
            continue;
        }
        pager_flush(pager,i,PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] =  NULL;
    }
    //pager->pages[i] = NULL;
    num_additional_rows = (table->num_rows % (ROWS_PER_PAGE));
    if(num_additional_rows > 0){
        pager_flush(pager,num_full_pages,num_additional_rows * ROW_SIZE);
        free(pager->pages[num_full_pages]);
        pager->pages[num_full_pages] = NULL;
    }
    close(pager->fd);
    pager->fd = 0;
    free(pager);
    free(table);

}


void print_row(Row_t *row){
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}
