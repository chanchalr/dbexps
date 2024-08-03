#include "syshead.h"
#include "interface.h"

InputBuffer_t *new_input_buffer(){
    InputBuffer_t *input_buffer = malloc(sizeof(InputBuffer_t));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void read_input(InputBuffer_t *buf){
    ssize_t bytes_read =0 ;
    bytes_read = getline(&(buf->buffer),&(buf->buffer_length),stdin);
    buf->input_length = bytes_read - 1;
    buf->buffer[bytes_read-1] = 0;
}

void print_prompt(){
    printf("db> ");
}

void close_input_buffer(InputBuffer_t *buf){
    free(buf->buffer);
    free(buf);
}
