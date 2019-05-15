#include "syshead.h"
#include "statement.h"
#include "interface.h"
#include "data.h"
#include "pager.h"
PrepareResult_t  prepare_statement(InputBuffer_t *buf,Statement_t *st){
    int args_assigned = 0;
    if(strncmp(buf->buffer,"insert",6) == 0){
        st->type = STATEMENT_INSERT;
        args_assigned = sscanf(buf->buffer,"insert %d %s %s",&(st->row_to_insert.id),st->row_to_insert.username,st->row_to_insert.email);
        if(args_assigned < 3){
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }
    if(strncmp(buf->buffer,"select",6) == 0){
        st->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult_t do_meta_command(InputBuffer_t *buf,Table_t *t){
    if(strcmp(buf->buffer,".exit") == 0){
        close_input_buffer(buf);
        db_close(t);
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}



ExecuteResult_t execute_insert(Statement_t* statement, Table_t* table){
    Row_t *row_to_insert;

    if(table->num_rows >= TABLE_MAX_PAGES){
        return EXECUTE_TABLE_FULL;
    }

    row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert,row_slot(table,table->num_rows));
    table->num_rows+=1;

    return EXECUTE_SUCCESS;
}

ExecuteResult_t execute_select(Statement_t *statement,Table_t *table){
    Row_t row;
    uint32_t i = 0;
    for(i=0;i<table->num_rows;i++){
        deserialize_row(row_slot(table,i),&row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult_t execute_statement(Statement_t *st,Table_t *table){
    switch(st->type){
        case STATEMENT_INSERT:
            //printf("This is where insert happens\n");
            return execute_insert(st,table);
            break;
        case STATEMENT_SELECT:
            printf("This is where select happens\n");
            return execute_select(st,table);
            break;
        default:
            printf("Dont know what this command does\n");
            return EXIT_SUCCESS;
            break;
    }
}
