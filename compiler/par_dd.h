/*********************************************************************
 *
 *      Function and global variable declarations
 *      File Name: par_dd.h
 *
 *********************************************************************/
#ifndef _PAR_DD_H_
#define _PAR_DD_H_

#include "defs.h"
#include "datadict.h"


#define TOKEN_LEN      4096/* token buffer length */
#define IN_BUF_SIZE    4096/* input buffer size */
#define EOS            '\0'    /* string termination symbol */


/*********************************************************************
**  Token codes and type declarations
*********************************************************************/

typedef enum token_code {

   ID=10, STRING, STAR, L_PAREN, R_PAREN, MINUS, PLUS,
   EQUAL, COLON, SEMICOLON, COMMA, PERIOD, SLASH,
   OP_BRC, CL_BRC, DBL_QUOTE,
   /* PML reserved words */
   PROCESS, SEQUENCE, TASK, BRANCH, SELECTION,
   ITERATION, ACTION, MANUAL, EXECUTABLE, REQUIRES,
   PROVIDES, TOOL, AGENT, SCRIPT, INPUT, OUTPUT,
   CREATES

} token;

struct resword {
    char *str_ptr;  /* pointer to word string */
    token code;     /* word code */
};

static struct resword rw[] = {
    {"process", PROCESS}, {"sequence", SEQUENCE}, {"task", TASK},
    {"branch", BRANCH}, {"selection", SELECTION}, {"iteration", ITERATION},
    {"action", ACTION}, {"manual", MANUAL}, {"executable", EXECUTABLE},
    {"requires", REQUIRES}, {"provides", PROVIDES}, {"tool", TOOL},
    {"agent", AGENT}, {"script", SCRIPT}, {"input", INPUT},
    {"output", OUTPUT}, {"creates", CREATES}, NULL,
};


/*********************************************************************
**   Messages for compiler errors
*********************************************************************/

static char *error_message[] = {
    NULL,
    " - string too long (max length = 4096)!",
    " - syntax error!",
    " - error parsing comments!",
};


/*********************************************************************
**   Function prototypes
*********************************************************************/
int parse_pml(char* pml_filename, data_dictionary_struct* dictionary_ptr);
void syntax_error(char *msg);
int next_token();
void push_back();
void clear_token_buffer();
int lexical_analyzer();
int check_resword();
void match_token(int t);
int primlist_func(data_dictionary_struct* dictionary_ptr, char *parent_name);
int speclist_func(data_dictionary_struct* dictionary_ptr, char *action_name);

/*********************************************************************
**   Declarations of the global variables
*********************************************************************/

FILE *in_fptr;
token look_a_head;
int line_no;
char token_buffer[TOKEN_LEN];
int num_unnamed_types;
int error_flag;
#endif
