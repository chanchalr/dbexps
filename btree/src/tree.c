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

uint32_t *internal_node_key(void *node,uint32_t cell_num){
    return node+INTERNAL_NODE_HEADER_SIZE+(cell_num*INTERNAL_NODE_CELL_SIZE)+INTERNAL_NODE_CHILD_SIZE;
}


uint32_t *internal_node_num_keys(void *node){
    return (uint32_t *)(node+INTERNAL_NODE_NUM_KEYS_OFFSET);
}

uint32_t *internal_node_right_child(void *node){
    return (uint32_t *)(node+INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}

uint32_t *internal_node_cell(void *node,uint32_t cell_num){
    return (uint32_t *)(node+INTERNAL_NODE_HEADER_SIZE+(cell_num*INTERNAL_NODE_CELL_SIZE));
}


uint32_t *internal_node_child(void *node,uint32_t cell_num){
    uint32_t num_keys = *internal_node_num_keys(node);
    if(cell_num > num_keys){
        printf("Access tp child num > num_keys");
        exit(EXIT_FAILURE);
    }
    if(cell_num == num_keys){
        return internal_node_right_child(node);
    }else{
        return internal_node_cell(node,cell_num);
    }
}

void * leaf_node_value(void *node,uint32_t cell_num){
    return node+LEAF_NODE_HEADER_SIZE+(cell_num*LEAF_NODE_CELL_SIZE)+LEAF_NODE_KEY_SIZE;
}

uint32_t get_node_max_key(void* node){
    switch(get_node_type(node)){
        case NODE_INTERNAL:
            return *internal_node_key(node,*internal_node_num_keys(node)-1);
        case NODE_LEAF:
            return *leaf_node_key(node,*leaf_node_num_cells(node)-1);
    }
    return 0;
}
void initialize_leaf_node(void* node) {
    *leaf_node_num_cells(node) = 0;
    set_node_root(node, false);
    set_node_type(node,NODE_LEAF);

}

void initialize_internal_node(void *node){
    set_node_type(node, NODE_INTERNAL);
    set_node_root(node, false);
    *internal_node_num_keys(node) = 0;
}


void leaf_node_split_and_insert(Cursor_t *c,uint32_t key,Row_t *value){
    void *destination_page = NULL,*destination_cell = NULL;
    void *old_page    = get_page(c->table->pager,c->page_num);
    uint32_t new_page_num = get_unused_page(c->table->pager);
    void *new_page = get_page(c->table->pager,new_page_num);
    int i = 0;
    uint32_t index = 0;
    initialize_leaf_node(new_page);
    for(i=0;i<LEAF_NODE_MAX_CELLS;i++){
        if(i>=LEAF_NODE_LEFT_SPLIT_COUNT){
            destination_page = new_page;
        }else{
            destination_page = old_page;
        }
        index  =i%(LEAF_NODE_LEFT_SPLIT_COUNT);
        destination_cell = leaf_node_cell(destination_page,index);
        
        if(i == c->cell_num){
            serialize_row(value,destination_cell);
        }else if(i > c->cell_num){
            memcpy(destination_cell,leaf_node_cell(old_page,i-1),LEAF_NODE_CELL_SIZE);
        }else{
            memcpy(destination_cell,leaf_node_cell(old_page,i),LEAF_NODE_CELL_SIZE);
        }
    }
    *(leaf_node_num_cells(old_page)) = LEAF_NODE_LEFT_SPLIT_COUNT;

    *(leaf_node_num_cells(new_page)) = LEAF_NODE_RIGHT_SPLIT_COUNT;
    if (is_node_root(old_page)) {
        create_new_root(c->table,new_page_num);
    }else{
        printf("Not Implemented\n");
    }
}

void create_new_root(Table_t *t,uint32_t new_page_num){
    void *root_node        =  get_page(t->pager,t->root_page_num);
    //void *right_child      =  get_page(t->pager,new_page_num);
    uint32_t left_page_num =  get_unused_page(t->pager);
    void *left_child       = get_page(t->pager,left_page_num);
    uint32_t left_child_max_key = 0;
    initialize_internal_node(root_node);
    memcpy(left_child,root_node,PAGE_SIZE);
    set_node_root(left_child, false);
    *internal_node_num_keys(root_node) = 1;
    *internal_node_child(root_node, 0) = left_page_num;
    left_child_max_key = get_node_max_key(left_child);
    *internal_node_right_child(root_node) = new_page_num;
    *internal_node_key(root_node, 0) = left_child_max_key;

}


void set_node_root(void *node,bool is_root){
    uint8_t val = is_root;
    *((uint8_t *)(node+IS_ROOT_OFFSET)) = val;
}

bool is_node_root(void *node){
    uint8_t value = *((uint8_t *)(node+IS_ROOT_OFFSET));
    return (bool) value;
}

void leaf_node_insert(Cursor_t *c,uint32_t key,Row_t *value){
    void *node;
    uint32_t num_cells = 0,i=0;
    node = get_page(c->table->pager,c->page_num);
    num_cells  =  *leaf_node_num_cells(node);
    if(num_cells >=  LEAF_NODE_MAX_CELLS) {
        leaf_node_split_and_insert(c,key,value);
        //printf("Need to split leaf node\n");
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


Cursor_t *leaf_node_find(Table_t *t,uint32_t page_num,uint32_t key){
    Cursor_t *c = NULL;
    void *node = get_page(t->pager,page_num);
    uint32_t index = 0;
    uint32_t minindex = 0;
    uint32_t key_at_index = 0;
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t maxindex_plus_one = num_cells; 
    c = (Cursor_t *)malloc(sizeof(Cursor_t));
    c->table = t;
    c->page_num = page_num;
    while(minindex < maxindex_plus_one){
        index = (minindex + maxindex_plus_one)/2;
        key_at_index = *leaf_node_key(node,index);
        if(key == key_at_index){
            c->cell_num = index;
            return c;
        }
        if(key < key_at_index){
            maxindex_plus_one = index;
        }else{
            minindex = index+1;
        }
    }
    c->cell_num = minindex;
    return c;
}

void print_leaf_node(void *node){
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t i= 0;
    printf("leaf (size %d)\n",num_cells);
    for(i=0;i < num_cells;i++){
        printf("index %d, key:%d",i,*leaf_node_key(node,i));
    }
}

NodeType_t get_node_type(void *node){
        uint8_t type = 0;
        type =  *(uint8_t *)(node+NODE_TYPE_OFFSET);
        return (NodeType_t)type;
}

void set_node_type(void *node,NodeType_t nodetype){
    uint8_t type = nodetype;
    *((uint8_t *)node+NODE_TYPE_OFFSET) = type;
}
