/******************************************************************************
**  File Information:  datadict.h is the header file for the datadict.c file.
**                     It has been separated into "public" and "private"
**                     functions.  Programs wishing to interface with datadict
**                     should only use the "public" functions.
******************************************************************************/
#ifndef _DATADICT_H_
#define _DATADICT_H_

#ifndef _DEFS_H_
#include "defs.h"
#endif

#ifndef _PML_MSG_H_
#include "pml_msg.h"
#endif

/* linked list of strings */
typedef struct string_list_struct {
	char *cstring;
	struct string_list_struct *next_string;
} string_list_struct;


/* struct to contain attribute information */
typedef struct data_dict_attribute_list_struct {
	char *attribute;
	string_list_struct *first_description;
	struct data_dict_attribute_list_struct* next_attribute;
} data_dict_attribute_list_struct;


/* struct used to contain an entry in the data dictionary */
typedef struct data_dict_element_struct {
	char  *level;/* string used to store level ("1.2.3") */
	char  *name;  /* string used to store name ("mix_martini") */
	char  *type;  /* string used to store type ("process") */
	char  *mode;  /* only valid for "actions" */

	/* only valid for "actions" */
	data_dict_attribute_list_struct *first_attribute_ptr;

	/* set to null for type "process" */
	struct data_dict_element_struct *parent_ptr;

	/* not valid for actions */
	struct data_dict_element_list_struct *first_child_ptr;
} data_dict_element_struct;


/* linked list of data dictionary elements */
typedef struct data_dict_element_list_struct {
	data_dict_element_struct *data_ptr;
	struct data_dict_element_list_struct *next_element;
} data_dict_element_list_struct;


typedef struct data_dictionary_struct {
	data_dict_element_struct *root;
} data_dictionary_struct;


/******************************************************************************
** "Public" Data Dictionary Functions
*******************************************************************************/

/* data dictionary root accessor and mutator functions */
Boolean data_dict_add_root(data_dictionary_struct *dictionary_ptr, char *level,
    char *name, char *type, char *mode);
data_dict_element_struct* data_dict_get_root(
    data_dictionary_struct *dictionary_ptr);


/* get the element associated with the specified parent */
data_dict_element_struct* data_dict_get_element(
    data_dictionary_struct *dictionary_ptr, char* parent_name);


/* routines to populate the data dictionary */
Boolean data_dict_add_entry(data_dictionary_struct *dictionary_ptr,
    char *parent_name, char *level, char *name, char *type, char *mode);
Boolean data_dict_action_add_desc(data_dictionary_struct *dictionary_ptr,
    char *action_name, char *desc_type, char *desc_text);


/* print the contents of the data dictionary to the screen */
void data_dict_print_to_screen(data_dictionary_struct *dictionary_ptr);


/* data dictionary element struct accessor functions */
void data_dict_get_level(data_dict_element_struct* element_ptr, char **level);
void data_dict_get_name(data_dict_element_struct* element_ptr, char **name);
void data_dict_get_type(data_dict_element_struct* element_ptr, char **type);
void data_dict_get_mode(data_dict_element_struct* element_ptr, char **mode);
void data_dict_get_child_levels(data_dict_element_struct* element_ptr,
    char *child_levels);
data_dict_element_struct* data_dict_get_parent(
    data_dict_element_struct* element_ptr);
data_dict_element_list_struct* data_dict_get_child_list(
    data_dict_element_struct* element_ptr);
data_dict_element_struct* data_dict_get_child(
    data_dict_element_list_struct* element_list_ptr);
data_dict_element_list_struct* data_dict_get_next_child(
    data_dict_element_list_struct* element_list_ptr);
data_dict_element_struct* data_dict_get_next_sibling(
    data_dict_element_struct* element_ptr);
				


/* data dictionary attribute type accessor functions */
data_dict_attribute_list_struct* data_dict_get_attribute_list(
    data_dict_element_struct* element_ptr);
data_dict_attribute_list_struct* data_dict_get_next_attribute(
    data_dict_attribute_list_struct* attr_list_ptr);
void data_dict_get_attribute_type(
data_dict_attribute_list_struct* attr_list_ptr, 
    char** attr_type);


/* data dictionary attribute description accessor functions */
string_list_struct* data_dict_get_attribute_desc_list(
    data_dict_attribute_list_struct* attr_list_ptr);
string_list_struct* data_dict_get_next_attribute_desc(
    string_list_struct* attr_desc_list_ptr);
void data_dict_get_attribute_desc(string_list_struct* attr_desc_list_ptr,
    char** attr_desc);


/* data dictionary element struct mutatoror functions */
void data_dict_set_level(data_dict_element_struct* element_ptr, char *level);
void data_dict_set_name(data_dict_element_struct* element_ptr, char *name);
Boolean data_dict_set_type(data_dict_element_struct* element_ptr, char *type);
Boolean data_dict_set_mode(data_dict_element_struct* element_ptr, char *mode);


/* populate the data dictionary with information about its level */
Boolean add_level_info_to_data_dictionary(
    data_dictionary_struct *dictionary_ptr);
void add_level_info_recursively(data_dict_element_struct* element_ptr);
void data_dict_destroy(data_dictionary_struct *dictionary_ptr);

/******************************************************************************
** "Private" Data Dictionary Functions
*******************************************************************************/
Boolean data_dict_add_child_element(data_dict_element_struct *parent_element,
    data_dict_element_struct *child_element);

data_dict_element_struct* data_dict_dfs_find_element(
    data_dict_element_struct *element, char* name, int* match_found);

void data_dict_dfs_print(data_dict_element_struct *element);
void data_dict_print_element(data_dict_element_struct *element);
void data_dict_dfs_remove(data_dict_element_struct *element);
void data_dict_delete_element(data_dict_element_struct *element);

#endif
