/*********************************************************************
 *
 *      Function and global variable declarations
 *      File Name: par_dd.h
 *
 *********************************************************************/
#ifndef _PAR_DD_H_
#define _PAR_DD_H_

#ifndef _DATADICT_H_
#include "datadict.h"
#endif

#ifndef _DEFS_H_
#include "defs.h"
#endif

#ifndef _PML_MSG_H_
#include "pml_msg.h"
#endif
#define BUFFER_SIZE 	256
#define EOS            '\0'    /* string termination symbol */


/*********************************************************************
**  Token codes and type declarations
*********************************************************************/


typedef enum token_code {
	ID=10, STRING, STAR, L_PAREN, R_PAREN, MINUS, PLUS,
	EQUAL, COLON, SEMICOLON, COMMA, PERIOD, SLASH,
	OP_BRC, CL_BRC, DBL_QUOTE, AND,
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
	{NULL,0},{NULL,1},{NULL,2},{NULL,3},{NULL,4},
	{NULL,5},{NULL,6},{NULL,7},{NULL,8},{NULL,9},
	{"name",ID},{"literal",STRING},{"asterisk",STAR},{"left paren",L_PAREN},
	{"right paren",R_PAREN},{"minus sign",MINUS},{"plus sign",PLUS},
	{"equal sign",EQUAL},{"colon",COLON},{"semicolon",SEMICOLON},
	{"comma",COMMA},{"period",PERIOD},{"slash",SLASH},{"open brace",OP_BRC},
	{"close brace",CL_BRC},{"double quote",DBL_QUOTE},{"and operator",AND},
	{"process", PROCESS}, {"sequence", SEQUENCE}, {"task", TASK},
	{"branch", BRANCH}, {"selection", SELECTION}, {"iteration", ITERATION},
	{"action", ACTION}, {"manual", MANUAL}, {"executable", EXECUTABLE},
	{"requires", REQUIRES}, {"provides", PROVIDES}, {"tool", TOOL},
	{"agent", AGENT}, {"script", SCRIPT}, {"input", INPUT},
	{"output", OUTPUT}, {"creates", CREATES}, NULL,
};

/*********************************************************************
**   Function prototypes
*********************************************************************/
int parse_pml(char* pml_filename,
    data_dictionary_struct* dictionary_ptr,Boolean);
int next_token(char **);
void push_back(long);
void clear_token_buffer(char **);
int lexical_analyzer(char **);
int check_resword(char **);
void match_token(int t,char **,data_dictionary_struct *);
int primlist_func(data_dictionary_struct*,char *,char **);
int speclist_func(data_dictionary_struct*,char *,char **);

/*********************************************************************
**   Declarations of the global variables
*********************************************************************/

FILE *in_fptr;
token look_a_head;
int line_no;
int num_unnamed_types;
int error_flag;
#endif
