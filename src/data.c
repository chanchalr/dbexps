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
/*
void *row_slot(Table_t *table,uint32_t row_num){
    uint32_t page_num = 0;
    page_num = row_num/ROWS_PER_PAGE;
    void *page = get_page(table->pager,page_num);

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page+byte_offset;

}*/


Table_t *db_open(char *filename){
    void *root_node;
    Table_t *table = NULL;
    Pager_t *pager = NULL;
    pager = pager_open(filename);
    table = (Table_t *)malloc(sizeof(Table_t));
    table->root_page_num  = 0;
    if(pager->num_pages == 0 ){
        root_node = get_page(pager,0);
        initialize_leaf_node(root_node);
    }
    table->pager = pager;
    return table;

}


void db_close(Table_t *table){
    uint32_t i = 0;
    Pager_t *pager = table->pager;
    for(i =0;i<pager->num_pages;i++){
        if(pager->pages[i] == NULL){
            continue;
        }
        pager_flush(pager,i);
        free(pager->pages[i]);
        pager->pages[i] =  NULL;
    }
    //pager->pages[i] = NULL;
    close(pager->fd);
    pager->fd = 0;
    free(pager);
    free(table);

}

void print_row(Row_t *row){
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}
Cursor_t *table_start(Table_t *t){
    Cursor_t *c = NULL;
    uint32_t num_cells = 0;
    void *root_node = NULL;
    c = (Cursor_t *)malloc(sizeof(Cursor_t));
    c->table = t;
    c->page_num = t->root_page_num;
    root_node = get_page(t->pager,t->root_page_num);
    num_cells = *leaf_node_num_cells(root_node);
    c->end_of_table = (num_cells == 0);
    return c;
}

Cursor_t *table_end(Table_t *t){
    Cursor_t *c = NULL;
    uint32_t num_cells = 0;
    void *root_node = NULL;
    c = (Cursor_t *)malloc(sizeof(Cursor_t));
    c->table = t;
    c->page_num = t->root_page_num;
    root_node =  get_page(t->pager,t->root_page_num);
    num_cells = *leaf_node_num_cells(root_node);
    c->cell_num = num_cells;
    c->end_of_table = true;
    return c;
}

void cursor_advance(Cursor_t *c){
    void *page = get_page(c->table->pager,c->page_num);
    c->cell_num += 1;
    if(c->cell_num >= (*leaf_node_num_cells(page))){
        c->end_of_table = true;
    }
}

void * cursor_value(Cursor_t *c){
    void *page =  NULL;
    page = get_page(c->table->pager,c->page_num);
    return leaf_node_value(page,c->cell_num);
}

