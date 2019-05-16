#ifndef __TREE_H__
#define __TREE_H__
#include "syshead.h"
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
#endif
