#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "syshead.h"
typedef struct InputBuffer__{
        char *buffer;
        size_t buffer_length;
        size_t input_length;
}InputBuffer_t;

InputBuffer_t *new_input_buffer();
void read_input(InputBuffer_t *buf);
void print_prompt();
void close_input_buffer(InputBuffer_t *buf);
#endif
