#include "syshead.h"
#include "tree.h"

uint32_t * leaf_node_num_cells(void *node){
    return node+LEAF_NODE_NUM_CELL_OFFSET;
}

void *leaf_node_cell(void *node,uint32_t cellnum){
    return node+LEAF_NODE_HEADER_SIZE+(cellnum*LEAD_NODE_CELL_SIZE);
}

uint32_t *leaf_node_key(void *node,uint32_t cell_num){
    return node+LEAF_NODE_HEADER_SIZE+(cell_num*LEAD_NODE_CELL_SIZE);
}

void * leaf_node_value(void *node,uint32_t cell_num){
    return node+LEAF_NODE_HEADER_SIZE+(cell_num*LEAD_NODE_CELL_SIZE)+LEAF_NODE_KEY_SIZE;
}
void initialize_leaf_node(void* node) {
    *leaf_node_num_cells(node) = 0;
}
