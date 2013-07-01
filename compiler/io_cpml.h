/******************************************************************************
**  File Information: prototype for generating output modules 
******************************************************************************/

#ifndef _IO_CPML_H_
#define _IO_CPML_H_

#include <gdbm.h>

#ifndef _DATADICT_H_
#include "datadict.h"
#endif

#ifndef _DEFS_H_
#include "defs.h"
#endif

#ifndef _PML_MSG_H_
#include "pml_msg.h"
#endif

typedef struct output {
	GDBM_FILE dbf;
	FILE *fptr;
	Boolean list;
} OUTPUT_STRUCT;

typedef struct vars {
	char var_name[64];
	char var_desc[128];
} VAR_STRUCT;

#define MAX_VARS 100

void add_var(char* name, char* desc);

int check_for_var(char* name);

void add_to_ready(char* name);

int check_for_ready(char* name);

Boolean write_cpml(char* pml_filename, data_dictionary_struct* dictionary_ptr,
    char* filetype,Boolean debug,Boolean list);

char* print_children(char* output_str, data_dict_element_struct* element_ptr);

char* get_actions_query(char action_str[1024],
    data_dict_element_struct* element_ptr,
    Boolean iteration_flag, char* attr_type);

char* get_actions(char action_str[1024],
    data_dict_element_struct* element_ptr,Boolean iteration_flag);

char* get_tokens(char action_str[1024],char temp_str[1024],
    int index, int number);

char* print_end(char action_str[1024]);

char* print_allbut_end(char action_str[1024], char temp_str[1024]);

data_dict_element_struct* get_next_action(data_dict_element_struct*
    element_ptr);

int write_data(OUTPUT_STRUCT output,data_dict_element_struct* element_ptr,
    char* filetype,int line_count);

int write_cpml_recursively(OUTPUT_STRUCT output,int current_line,
    data_dict_element_struct* element_ptr, char* filetype);

Boolean write_cpml_data(int level, char* data_str, char* mode,
    OUTPUT_STRUCT output);

int path_length(data_dict_element_struct* element_ptr);
int num_children(data_dict_element_struct* element_ptr);

char *create_cpml_filename (char *pml, char *cpml, char* filetype); 

GDBM_FILE create_gdbm(char *fn, char *mode);
void store_gdbm_data(GDBM_FILE dbf, int key_data, char *content_data);
char* retrieve_gdbm_data(GDBM_FILE dbf, int key_data);
void test_retrieval(OUTPUT_STRUCT output);

#endif	/* _IO_CPML_H_ */

