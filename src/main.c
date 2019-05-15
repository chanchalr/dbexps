#include "syshead.h"
#include "interface.h"
#include "statement.h"
#include "data.h"
#include "pager.h"
int main(){
    InputBuffer_t *buf = new_input_buffer();
    Statement_t st;
    Table_t *table = NULL;
    table = db_open("database.db");
    while(true){
        print_prompt();
        read_input(buf);
        if(buf->buffer[0] == '.'){
            switch(do_meta_command(buf,table)){
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command %s\n",buf->buffer);
                    continue;

            }
        }
        switch(prepare_statement(buf,&st)){
            case PREPARE_SUCCESS:
                break;
            case PREPARE_SYNTAX_ERROR:
                printf("Couldnt parse the statement %s\n",buf->buffer);
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Urecognized keyword at start of %s\n",buf->buffer);
                continue;
        }

        execute_statement(&st,table);
        printf("Executed\n");
    }
}
