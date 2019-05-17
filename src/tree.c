#include "syshead.h"
#include "tree.h"
#include "data.h"
uint32_t * leaf_node_num_cells(void *node){
    return node+LEAF_NODE_NUM_CELL_OFFSET;
}

void *leaf_node_cell(void *node,uint32_t cellnum){
    return node+LEAF_NODE_HEADER_SIZE+(cellnum*(LEAF_NODE_CELL_SIZE));
}

uint32_t *leaf_node_key(void *node,uint32_t cell_num){
    return node+LEAF_NODE_HEADER_SIZE+(cell_num*LEAF_NODE_CELL_SIZE);
}

void * leaf_node_value(void *node,uint32_t cell_num){
    return node+LEAF_NODE_HEADER_SIZE+(cell_num*LEAF_NODE_CELL_SIZE)+LEAF_NODE_KEY_SIZE;
}
void initialize_leaf_node(void* node) {
    *leaf_node_num_cells(node) = 0;
}

void leaf_node_insert(Cursor_t *c,uint32_t key,Row_t *value){
    void *node;
    uint32_t num_cells = 0,i=0;
    node = get_page(c->table->pager,c->page_num);
    num_cells  =  *leaf_node_num_cells(node);
    if(num_cells >=  LEAF_NODE_MAX_CELLS) {
        printf("Need to split leaf node\n");
    }

    if(c->cell_num < num_cells){
        for(i = num_cells; i > c->cell_num;i--){
            memcpy(leaf_node_cell(node,i),leaf_node_cell(node,i-1),LEAF_NODE_CELL_SIZE);
        }
    }
    *(leaf_node_num_cells(node)) += 1;
    *(leaf_node_key(node,c->cell_num)) = key;
    serialize_row(value,leaf_node_value(node,c->cell_num));

}

void print_leaf_node(void *node){
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t i= 0;
    printf("leaf (size %d)\n",num_cells);
    for(i=0;i < num_cells;i++){
        printf("index %d, key:%d",i,*leaf_node_key(node,i));
    }
}
