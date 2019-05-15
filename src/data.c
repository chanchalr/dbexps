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
    uint32_t page_num = 0;
    page_num=  (unsigned int) ((unsigned int)row_num/(unsigned int)ROWS_PER_PAGE);
    printf("sizes id:%lu uname:%lu email:%lu row:%lu\n",ID_SIZE,USERNAME_SIZE,EMAIL_SIZE,ROW_SIZE);
    void *page = table->pages[page_num];
    if(page == NULL){
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    printf("slot at %p row_offset:%u byte_offset:%u row_size:%lu row_num:%u,row_per_page:%lu,page_num:%u\n",page+byte_offset,row_offset,byte_offset,ROW_SIZE,row_num,ROWS_PER_PAGE,page_num);
    return page+byte_offset;

}


Table_t * new_table(){
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
