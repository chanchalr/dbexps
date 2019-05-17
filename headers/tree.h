#ifndef __TREE_H__
#define __TREE_H__
#include "syshead.h"
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
typedef enum  NodeType__{
	NODE_INTERNAL,
	NODE_LEAF
}NodeType_t;

#define NODE_TYPE_SIZE          sizeof(uint8_t)
#define NODE_TYPE_OFFSET        0
#define IS_ROOT_SIZE            sizeof(uint8_t)
#define IS_ROOT_OFFSET          NODE_TYPE_SIZE
#define PARENT_POINTER_SIZE     sizeof(uint32_t)
#define PARENT_POINTER_OFFSET   (IS_ROOT_OFFSET+IS_ROOT_SIZE)
#define COMMON_NODE_HEADER_SIZE (NODE_TYPE_SIZE+IS_ROOT_SIZE+PARENT_POINTER_SIZE)

#define LEAF_NODE_NUM_CELL_SIZE   sizeof(uint8_t)
#define LEAF_NODE_NUM_CELL_OFFSET COMMON_NODE_HEADER_SIZE
#define LEAF_NODE_HEADER_SIZE     (COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELL_SIZE)


#define LEAF_NODE_KEY_SIZE        sizeof(uint32_t)
#define LEAF_NODE_KEY_OFFSET      0
#define LEAF_NODE_VALUE_SIZE      ROW_SIZE
#define LEAF_NODE_VALUE_OFFSET    (LEAF_NODE_KEY_OFFSET+LEAF_NODE_KEY_SIZE)
#define LEAF_NODE_CELL_SIZE       (LEAF_NODE_KEY_SIZE+LEAF_NODE_VALUE_SIZE)
#define LEAF_NODE_SPACE_FOR_CELLS (PAGE_SIZE-LEAF_NODE_HEADER_SIZE)
#define LEAF_NODE_MAX_CELLS       (LEAF_NODE_SPACE_FOR_CELLS/LEAF_NODE_CELL_SIZE)


uint32_t * leaf_node_num_cells(void *node);
void * leaf_node_cell(void *node,uint32_t cell_num);
uint32_t *leaf_node_key(void *node,uint32_t cell_num);
void * leaf_node_value(void *node,uint32_t cell_num);
void initialize_leaf_node(void* node);
#endif
