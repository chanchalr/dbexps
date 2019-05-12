#include "syshead.h"
#include "statement.h"
#include "interface.h"
PrepareResult_t  prepare_statement(InputBuffer_t *buf,Statement_t *st){
    if(strncmp(buf->buffer,"insert",6) == 0){
        st->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strncmp(buf->buffer,"select",6) == 0){
        st->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult_t do_meta_command(InputBuffer_t *buf){
    if(strcmp(buf->buffer,".exit") == 0){
        close_input_buffer(buf);
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

void execute_statement(Statement_t *st){
    switch(st->type){
        case STATEMENT_INSERT:
            printf("This is where insert happens\n");
            break;
        case STATEMENT_SELECT:
            printf("This is where select happens\n");
            break;
        default:
            printf("Dont know what this command does\n");
            break;
    }
}
