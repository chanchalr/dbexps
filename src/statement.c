#include "syshead.h"
#include "statement.h"
#include "interface.h"
#include "data.h"
#include "pager.h"
PrepareResult_t  prepare_insert(InputBuffer_t *buf,Statement_t *st){
    st->type = STATEMENT_INSERT;
    char *keyword=NULL,*id_string=NULL,*username=NULL,*email=NULL;
    int id;
    keyword   = strtok(buf->buffer," ");
    id_string = strtok(NULL," ");
    username  = strtok(NULL," ");
    email     = strtok(NULL," ");
    if(strcmp(keyword,"insert") != 0){
        return PREPARE_SYNTAX_ERROR;
    }
    if(id_string == NULL || username==NULL||email==NULL){
        return PREPARE_SYNTAX_ERROR;
    }
    id = atoi(id_string);
    if(strlen(email) > COLUMN_EMAIL_SIZE){
        return PREPARE_STRING_TOO_LONG;
    }

    if(strlen(username) > COLUMN_USERNAME_SIZE){
        return PREPARE_STRING_TOO_LONG;
    }
    st->row_to_insert.id  = id;
    strcpy(st->row_to_insert.email,email);
    strcpy(st->row_to_insert.username,username);
    return PREPARE_SUCCESS;
}
PrepareResult_t  prepare_statement(InputBuffer_t *buf,Statement_t *st){
    if(strncmp(buf->buffer,"insert",6) == 0){
        return prepare_insert(buf,st);
        /*
        st->type = STATEMENT_INSERT;
        args_assigned = sscanf(buf->buffer,"insert %d %s %s",&(st->row_to_insert.id),st->row_to_insert.username,st->row_to_insert.email);
        if(args_assigned < 3){
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
        */
    }
    if(strncmp(buf->buffer,"select",6) == 0){
        st->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult_t do_meta_command(InputBuffer_t *buf,Table_t *t){
    if(strcmp(buf->buffer,".exit") == 0){
        db_close(t);
        close_input_buffer(buf);
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}



ExecuteResult_t execute_insert(Statement_t* statement, Table_t* table){
    Row_t *row_to_insert;
    Cursor_t *c = NULL;
    c= table_end(table);

    if(table->num_rows >= TABLE_MAX_PAGES){
        return EXECUTE_TABLE_FULL;
    }

    row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert,cursor_value(c));
    table->num_rows+=1;
    free(c);
    return EXECUTE_SUCCESS;
}

ExecuteResult_t execute_select(Statement_t *statement,Table_t *table){
    Row_t row;
    Cursor_t *c;
    /*for(i=0;i<table->num_rows;i++){
        deserialize_row(row_slot(table,i),&row);
        print_row(&row);
    }*/

    c = table_start(table);
    while(!(c->end_of_table)){
        deserialize_row(cursor_value(c),&row);
        print_row(&row);
        cursor_advance(c);
    }
    free(c);
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
