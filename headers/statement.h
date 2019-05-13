#ifndef __STATEMENT_H__
#define __STATEMENT_H__
#include "syshead.h"
#include "interface.h"
#include "data.h"
typedef enum MetaCommandResult__{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult_t;


typedef enum ExecuteResult__{
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
}ExecuteResult_t;

typedef enum PrepareResult__{
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult_t;


typedef enum StatementType__{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType_t;

typedef struct Statement__{
    StatementType_t type;
    Row_t row_to_insert;
}Statement_t;

PrepareResult_t  prepare_statement(InputBuffer_t *buf,Statement_t *st);
MetaCommandResult_t do_meta_command(InputBuffer_t *buf);
void execute_statement(Statement_t *st);
ExecuteResult_t execute_insert(Statement_t *s,Table_t *t);
ExecuteResult_t execute_select(Statement_t *s,Table_t *t);
#endif
