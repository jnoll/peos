/******************************************************************************
**  File Information:  The data dictionary is a set of routines to store and
**                     retrieve all of data associated with PML and CPML files.
**
**                     A routine is also provided to populate the level
**                     information, to be used in CPML.
**
**                     The data is stored in a tree format, with 0 or more
**                     branches at each tree element.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datadict.h"

/******************************************************************************
**    Function/Method Name: data_dict_add_root
**    Precondition:
**    Postcondition:        Returns "FALSE" if there is a memory allocation
**                          problem and writes an error to stdio
**                          Otherwise it creates a new element, populates it,
**                          and assigns it to the data dictionary root.
**    Description:          Create, populate, and assign a new element to act
**                          as the data dictionary root.
******************************************************************************/

Boolean data_dict_add_root(data_dictionary_struct *dictionary_ptr, char *level,
    char *name, char *type, char *mode)
{
	char *module = "data_dict_add_root";
	Boolean returnval = TRUE;
	
	/* set the root = NULL */
	dictionary_ptr->root = NULL;

	/* 
	 * Allocate memory for the new data dictionary element, 
	 * setting all values to NULL using the calloc().
	 */
	if((dictionary_ptr->root = (data_dict_element_struct*)calloc(1,
  	    sizeof(data_dict_element_struct))) == NULL) {
		pmlprint(ERROR,PMLMEM,module,NULL);
		returnval = FALSE;
	}

	/* 
	 * Each of datadict elements are composed of pointers to 
	 * strings. Allocate the required memory then populate
	 * the root with the specified information.
	 */
	if(returnval == TRUE) {
		if((dictionary_ptr->root->level = 
		    (char *)calloc(strlen(level)+1,sizeof(char))) 
		    != NULL) {
			strcpy(dictionary_ptr->root->level, level);
		} else {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}

	if(returnval == TRUE) {
		if((dictionary_ptr->root->name = 
		    (char *)calloc(strlen(name)+1,
		     sizeof(char))) != NULL) {
			strcpy(dictionary_ptr->root->name, name);
		} else {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}

	if(returnval == TRUE) {
		if((dictionary_ptr->root->type = 
		    (char *)calloc(strlen(type)+1,
		    sizeof(char))) != NULL) {
			strcpy(dictionary_ptr->root->type, type);
		} else {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}

	if(returnval == TRUE) {
		if((dictionary_ptr->root->mode = 
		    (char *)calloc(strlen(mode)+1,
	 	    sizeof(char))) != NULL) {
			strcpy(dictionary_ptr->root->mode, mode);
		} else {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}

	if(returnval == FALSE) {
		if(dictionary_ptr->root != NULL){
			if(dictionary_ptr->root->level != NULL) {
				free(dictionary_ptr->root->level);
				dictionary_ptr->root->level = NULL;
			}
			if(dictionary_ptr->root->name != NULL) {
				free(dictionary_ptr->root->name);
				dictionary_ptr->root->name = NULL;
			}
			if(dictionary_ptr->root->type != NULL) {
				free(dictionary_ptr->root->type);
				dictionary_ptr->root->type = NULL;
			}
			if(dictionary_ptr->root->mode != NULL) {
				free(dictionary_ptr->root->mode );
				dictionary_ptr->root->mode = NULL;
			}
			free(dictionary_ptr->root);
			dictionary_ptr->root = NULL;
		}
	}
	return returnval;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_root
**    Precondition:         dictionary_ptr may or may not be NULL.
**    Postcondition:        If dictionary_ptr is not NULL, a pointer to the
**                          root element is returned, otherwise NULL is returned
**    Description:          Get the root element associated with a data
**                          dictionary.
******************************************************************************/

data_dict_element_struct* data_dict_get_root(
    data_dictionary_struct *dictionary_ptr)
{
	if (dictionary_ptr != NULL) {
		return dictionary_ptr->root;
	}
	return (data_dict_element_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_element
**    Precondition:         dictionary_ptr points to a valid data dictionary
**                          struct.
**    Postcondition:        Returns the element associated with the given name.
**                          Otherwise NULL is returned.
**    Description:          Returns the element associated with the given name.
******************************************************************************/

data_dict_element_struct* data_dict_get_element(
    data_dictionary_struct *dictionary_ptr, char* name)
{
	int found_match = FALSE;

	/*
	 * Perform a depth first search on the tree starting at the root,
	 * with the element name we are trying to find and a flag stating
	 * we have not found a match.
	 */
	return data_dict_dfs_find_element(dictionary_ptr->root,
	    name, &found_match);
}

/******************************************************************************
**    Function/Method Name: data_dict_add_entry
**    Precondition:         dictionary_ptr already has been assigned a root and
**                          points to a valid data dictionary struct.
**    Postcondition:        Returns "FALSE" and writes an error message to stdio
**                          if any of the following errors occur:
**                          - There is not an element associated with the parent
**                            name.
**                          - Memory allocation problems.
**                          - Unable to add the child to the parents list
**                          Otherwise creates a new element, populates it with
**                          the specified information and inserts it into the
**                          data dictionary.
**    Description:          Creates a new entry containing the specified
**                          information and adds it to the data dictionary.
******************************************************************************/

Boolean data_dict_add_entry(data_dictionary_struct *dictionary_ptr,
    char *parent_name, char *level, char *name, char *type, char *mode)
{
	char *module = "data_dict_add_entry";
	char msg[MSG_SIZE];
	data_dict_element_struct* parent_ptr = NULL;
	data_dict_element_struct* new_element_ptr = NULL;
	Boolean returnval = TRUE;
	
	/* get the parent element */
	if((parent_ptr = data_dict_get_element(dictionary_ptr,parent_name))
	   == NULL) {
		/*
		 * If there is not an entry associated with the parent 
		 * name then error.
		 */
		sprintf(msg,"Unable to find entry for %s",parent_name);
		pmlprint(ERROR,PMLDD,module,msg);
		returnval = FALSE;
	}

	/*
	 * Allocate memory for the new data dictionary element, 
	 * setting all values to NULL using the calloc()i
	 */
	if(returnval == TRUE) {
		if((new_element_ptr = (data_dict_element_struct*)calloc(1,
		    sizeof(data_dict_element_struct))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}

	/* populate the entry with the specified information */
	if(returnval == TRUE) {
		if((new_element_ptr->level = (char *)calloc(strlen(level)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		} else {
			strcpy(new_element_ptr->level, level);
		}
	}

	if(returnval == TRUE) {
		if((new_element_ptr->name = (char *)calloc(strlen(name)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		} else {
			strcpy(new_element_ptr->name, name);
		}
	}

	if(returnval == TRUE) {
		if((new_element_ptr->type = (char *)calloc(strlen(type)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		} else {
			strcpy(new_element_ptr->type, type);
		}
	}

	if(returnval == TRUE) {
		if((new_element_ptr->mode = (char *)calloc(strlen(mode)+1,
  	 	   sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		} else {
			strcpy(new_element_ptr->mode, mode);
		}
	}
	if(returnval == TRUE) {
		new_element_ptr->parent_ptr = parent_ptr;

		/* add the new entry to the parents list of children */
		returnval = data_dict_add_child_element(parent_ptr,
		   new_element_ptr);
		if(returnval == FALSE) {
			sprintf(msg,"Unable to add child element %s",
			    new_element_ptr->name);
			pmlprint(ERROR,PMLDD,module,msg);	
		}
	}

	/*
	 * Something caused a problem, so free any
	 * memory allocated.  
	 */
	if(returnval == FALSE) {
		if(new_element_ptr->mode != NULL) {
			free(new_element_ptr->mode);
			new_element_ptr->mode = NULL;
		}
		if(new_element_ptr->type != NULL) {
			free(new_element_ptr->type);
			new_element_ptr->type = NULL;
		}
		if(new_element_ptr->name != NULL) {
			free(new_element_ptr->name);
			new_element_ptr->name = NULL;
		}
		if(new_element_ptr->level != NULL) {
			free(new_element_ptr->level);
			new_element_ptr->level = NULL;
		}
		if(new_element_ptr != NULL) {
			free(new_element_ptr);
		}
	}
	return returnval;
}

/******************************************************************************
**    Function/Method Name: data_dict_action_add_desc
**    Precondition:         dictionary_ptr must point to valid dictionary struct
**    Postcondition:        Returns "FALSE" and writes an error to stdio if any
**                          of the following events occur.
**                          - there is not an action associated with the
**                            specified action_name.
**                          - memory allocation problems.
**                          Otherwise the attribute information is added to the
**                          element associated with action_name and "TRUE" is
**                          returned.
**    Description:          Adds the specified attribute information to the
**                          element named action_name.
******************************************************************************/

Boolean data_dict_action_add_desc(data_dictionary_struct *dictionary_ptr,
    char *action_name, char *desc_type, char *desc_text)
{
	char *module = "data_dict_action_add_desc";
	char msg[MSG_SIZE];
	Boolean returnval = TRUE;
	Boolean found_attribute;
	data_dict_element_struct *action_ptr = NULL;
	data_dict_attribute_list_struct *temp_attr_ptr = NULL;
	data_dict_attribute_list_struct *prev_attr_ptr = NULL;
	data_dict_attribute_list_struct *curr_attr_ptr = NULL;
	struct string_list_struct *temp_string_ptr = NULL;
	struct string_list_struct *prev_string_ptr = NULL;
	struct string_list_struct *new_string_ptr = NULL;

	/* find the desired action */
	if((action_ptr = data_dict_get_element(dictionary_ptr,
	   action_name)) == NULL) {
		sprintf(msg,"Could not find element in dict %s",
		    action_name);
		pmlprint(ERROR,PMLDD,module,msg);
		returnval = FALSE;
	}

	/*
	 * Search to see if we already have an attribute of
	 * the same type.
	 */
	if(returnval == TRUE) {
		temp_attr_ptr = action_ptr->first_attribute_ptr;
		prev_attr_ptr = temp_attr_ptr;
		found_attribute = FALSE;
		while((temp_attr_ptr != NULL) && 
		    (found_attribute == FALSE)) {
			if(strcmp(temp_attr_ptr->attribute,
			    desc_type) == 0) {
				found_attribute = TRUE;
				curr_attr_ptr = temp_attr_ptr;
			} else {
				prev_attr_ptr = temp_attr_ptr;
				temp_attr_ptr = 
				    temp_attr_ptr->next_attribute;
			}
		}

		/* 
		 * If the attribute type does not exist, then we need to 
		 * add it to the end of the attribute list.
		 */
		if(found_attribute == FALSE) {
			if((curr_attr_ptr = 
			    (data_dict_attribute_list_struct*)calloc(1,
			    sizeof(data_dict_attribute_list_struct))) == NULL) {
				pmlprint(ERROR,PMLMEM,module,NULL);
				returnval = FALSE;
			}

			if(returnval == TRUE) {
				/* populate the new attribute */
				if((curr_attr_ptr->attribute = 
				    (char *)calloc(strlen(desc_type)+1,
				    sizeof(char))) == NULL) {
					pmlprint(ERROR,PMLMEM,module,NULL);
					returnval = FALSE;
				} else {
					strcpy(curr_attr_ptr->attribute,
					    desc_type);
				}
			}

			if(returnval == TRUE) {
				if(action_ptr->first_attribute_ptr == NULL) {
					temp_attr_ptr = curr_attr_ptr;
					action_ptr->first_attribute_ptr = 
					    curr_attr_ptr;
				} else {
					prev_attr_ptr->next_attribute = 
					    curr_attr_ptr;
				}
			}
		}
	}

	if(returnval == TRUE) {
		/* 
		 * Add the description to the end of the curr_attr_ptr 
		 * description list.
		 */
		temp_string_ptr = curr_attr_ptr->first_description;
		prev_string_ptr = temp_string_ptr;
		while(temp_string_ptr != NULL) {
			prev_string_ptr = temp_string_ptr;
			temp_string_ptr = temp_string_ptr->next_string;
		}
	
		/* allocate memory for the new string list element */
		if((new_string_ptr = (struct string_list_struct*)calloc(1,
		    sizeof(string_list_struct))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}

		if(returnval == TRUE) {
			/* populate the new string */
			if((new_string_ptr->cstring =
			    (char *)calloc(strlen(desc_text)+1,
		  	    sizeof(char))) == NULL){
				pmlprint(ERROR,PMLMEM,module,NULL);
			} else {
				strcpy(new_string_ptr->cstring, desc_text);
		
				/* add the new string to the list */
				if(curr_attr_ptr->first_description == NULL) {
					curr_attr_ptr->first_description = 
				    	new_string_ptr;
				} else {
					prev_string_ptr->next_string = 
					    new_string_ptr;
				}
			}
		}
	}
	if(returnval == FALSE) {
		if(new_string_ptr->cstring != NULL) {
			free(new_string_ptr->cstring);
			new_string_ptr->cstring = NULL;
		}
		if(new_string_ptr != NULL) {
			free(new_string_ptr);
			new_string_ptr = NULL;
		}
		if(curr_attr_ptr->attribute != NULL) {
			free(curr_attr_ptr->attribute);
			curr_attr_ptr->attribute = NULL;
		} 
		if(curr_attr_ptr != NULL) {
			free(curr_attr_ptr);	
			curr_attr_ptr = NULL;
		}
	}
	return returnval;
}

/******************************************************************************
**    Function/Method Name: data_dict_print_to_screen
**    Precondition:         dictionary_ptr points to a valid structure
**    Postcondition:        The contents of the data dictionary are written to
**                          stdio.
**    Description:          Prints the contents of the data dictionary to stdio
******************************************************************************/

void data_dict_print_to_screen(data_dictionary_struct *dictionary_ptr)
{
	pmlprint(INFO,PMLDD,"data_dict_print_to_screen",NULL);
	data_dict_dfs_print(dictionary_ptr->root);
}

/******************************************************************************
**    Function/Method Name: data_dict_destroy
**    Precondition:         dictionary_ptr points to a valid structure
**    Postcondition:        The contents of the data dictionary are released.
**    Description:          Removes elements of data dictionary and frees 
**			    memory allocated via recursive calls to 
**			    data_dict_dfs_remove.
******************************************************************************/

void data_dict_destroy(data_dictionary_struct *dictionary_ptr)
{
	data_dict_dfs_remove(dictionary_ptr->root);
}

/******************************************************************************
**    Function/Method Name: data_dict_get_level
**    Precondition:         element_ptr may or may not be NULL.
**    Postcondition:        If element_ptr is not null, level will be set to
**                          the level associated with the element.
**    Description:          Get the level associated with the given element.
******************************************************************************/

void data_dict_get_level(data_dict_element_struct* element_ptr, char **level)
{
	if (element_ptr != NULL)
	{
		if ((*level = (char *)calloc(strlen(element_ptr->level)+1,
		  sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,"data_dict_get_level",NULL);
		} else {
			strcpy(*level, element_ptr->level);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_get_name
**    Precondition:         element_ptr may or may not be NULL.
**    Postcondition:        If element_ptr is not null, name will be set to
**                          the name associated with the element.
**    Description:          Get the name associated with the given element.
******************************************************************************/

void data_dict_get_name(data_dict_element_struct* element_ptr, char **name)
{
	if(element_ptr != NULL) {
		if((*name = (char *)calloc(strlen(element_ptr->name)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,"data_dict_get_name",NULL);
		} else {
			strcpy(*name, element_ptr->name);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_get_type
**    Precondition:         element_ptr may or may not be NULL.
**    Postcondition:        If element_ptr is not null, type will be set to
**                          the type associated with the element.
**    Description:          Get the type associated with the given element.
******************************************************************************/

void data_dict_get_type(data_dict_element_struct* element_ptr, char **type)
{
	if(element_ptr != NULL) {
		if((*type = (char *)calloc(strlen(element_ptr->type)+1,
  		  sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,"data_dict_get_type",NULL);
		} else {
			strcpy(*type, element_ptr->type);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_get_mode
**    Precondition:         element_ptr may or may not be NULL.
**    Postcondition:        If element_ptr is not null, mode will be set to
**                          the mode associated with the element.
**    Description:          Get the mode associated with the given element.
******************************************************************************/

void data_dict_get_mode(data_dict_element_struct* element_ptr, char **mode)
{
	if(element_ptr != NULL) {
		if((*mode = (char *)calloc(strlen(element_ptr->mode)+1,
		    sizeof(char))) != NULL) {
			strcpy(*mode, element_ptr->mode);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_get_child_levels
**    Precondition:         element_ptr may or may not be NULL
**                          child_levels must be the following size
**    Postcondition:        If element_ptr is not NULL, child_levels will
**                          contain level information for all of the elements
**                          children, otherwise NULL is returned.
**    Description:          Get a string containing level information for all
**                          of an elements children.
******************************************************************************/

void data_dict_get_child_levels(data_dict_element_struct* element_ptr,
    char *child_levels)
{
	data_dict_element_list_struct *temp_child_ptr = NULL;
	data_dict_element_struct* temp = NULL;

	if(element_ptr != NULL) {
		temp_child_ptr = element_ptr->first_child_ptr;
		if(temp_child_ptr != NULL) {
			temp = temp_child_ptr->data_ptr;
			sprintf(child_levels, "%s", temp->level);
			temp_child_ptr = temp_child_ptr->next_element;

			while(temp_child_ptr != NULL) {
				temp = temp_child_ptr->data_ptr;
				strcat(child_levels, " ");
				strcat(child_levels, temp->level);
				temp_child_ptr = temp_child_ptr->next_element;
			}
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_get_parent
**    Precondition:         element_ptr may or may not be NULL.
**    Postcondition:        If element_ptr is not NULL, the corresponding
**                          parent_ptr is returned, otherwise NULL is returned.
**    Description:          Return the parent_ptr associated with the element
******************************************************************************/

data_dict_element_struct* data_dict_get_parent(
    data_dict_element_struct* element_ptr)
{
	if(element_ptr != NULL) {
		return element_ptr->parent_ptr;
	}
	return (data_dict_element_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_child_list
**    Precondition:         element_ptr may or may not be NULL
**    Postcondition:        If element_ptr is not NULL, the corresponding child
**                          list is returned, otherwise NULL is returned.
**    Description:          Return a pointer to the child list for the given
**                          element.
******************************************************************************/

data_dict_element_list_struct* data_dict_get_child_list(
    data_dict_element_struct *element_ptr)
{
	if(element_ptr != NULL) {
		return element_ptr->first_child_ptr;
	}
	return (data_dict_element_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_child
**    Precondition:         element_list_ptr may or may not be NULL
**    Postcondition:        If element_list_ptr is not NULL, the corresponding
**                          child ptr is returned, otherwise NULL is
**                          returned.
**    Description:          Return the child associated with a given child list
**                          element.
******************************************************************************/

data_dict_element_struct* data_dict_get_child(
    data_dict_element_list_struct *element_list_ptr)
{
	if(element_list_ptr != NULL) {
		return element_list_ptr->data_ptr;
	}
	return (data_dict_element_struct*)NULL;
}

/******************************************************************************
**
** Function/Method Name: data_dict_get_next_sibling
** Precondition:	 A valid pointer into the data dictionary.
** Postcondition:	 A valid pointer into the data dictionary or
**			 NULL is returned.
** Description:		 The function traverses the tree to find the 
**			 next sibling or its parent's sibling, etc in
**			 the tree.  If none are found then it returns NULL.
**
******************************************************************************/

data_dict_element_struct* data_dict_get_next_sibling(
    data_dict_element_struct* element_ptr)
{
	data_dict_element_struct* parent_ptr;
	data_dict_element_struct* child_element_ptr;
	data_dict_element_list_struct* child_list_ptr;

	parent_ptr = data_dict_get_parent(element_ptr);
	child_list_ptr = data_dict_get_child_list(parent_ptr);
	child_element_ptr = data_dict_get_child(child_list_ptr);
	while (child_element_ptr != element_ptr) {
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
	}
	child_list_ptr = data_dict_get_next_child(child_list_ptr);
	return data_dict_get_child(child_list_ptr);
}

/******************************************************************************
**    Function/Method Name: data_dict_get_next_child
**    Precondition:         element_list_ptr may or may not be NULL
**    Postcondition:        If element_list_ptr is not NULL, the corresponding
**                          next child ptr is returned, otherwise NULL is
**                          returned.
**    Description:          Return the next child ptr.
******************************************************************************/

data_dict_element_list_struct* data_dict_get_next_child(
    data_dict_element_list_struct *element_list_ptr)
{
	if(element_list_ptr != NULL) {
		return element_list_ptr->next_element;
	}
	return (data_dict_element_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_attribute_list
**    Precondition:         element_ptr may or may not be NULL
**    Postcondition:        If element_ptr is not NULL, the first attribute is
**                          returned, otherwise NULL is returned.
**    Description:          Return the first attribute of an element.
******************************************************************************/

data_dict_attribute_list_struct* data_dict_get_attribute_list(
    data_dict_element_struct* element_ptr)
{
	if(element_ptr != NULL) {
		return element_ptr->first_attribute_ptr;
	}
	return (data_dict_attribute_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_next_attribute
**    Precondition:         attr_list_ptr may or may not be NULL
**    Postcondition:        If attr_list_ptr is not NULL, the next attribute is
**                          returned, otherwise NULL is returned.
**    Description:          Return the next attribute.
******************************************************************************/

data_dict_attribute_list_struct* data_dict_get_next_attribute(
    data_dict_attribute_list_struct* attr_list_ptr)
{
	if(attr_list_ptr != NULL) {
		return attr_list_ptr->next_attribute;
	}
	return (data_dict_attribute_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_attribute_type
**    Precondition:         attr_list_ptr may or may not be NULL
**    Postcondition:        If attr_list_ptr is not NULL, attr_type will
**                          contain the corresponding attribute type.
**    Description:          Get the attribute type associated with the given
**                          attribute.
******************************************************************************/

void data_dict_get_attribute_type(
    data_dict_attribute_list_struct* attr_list_ptr, char** attr_type)
{
	if(attr_list_ptr != NULL) {
		if((*attr_type = 
		    (char *)calloc(strlen(attr_list_ptr->attribute)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,
			    "data_dict_get_attribute_type",NULL);
		} else {
			strcpy(*attr_type, attr_list_ptr->attribute);
		}
	}
}


/******************************************************************************
**    Function/Method Name: data_dict_get_attribute_desc_list
**    Precondition:         attr_list_ptr my or may not be NULL.
**    Postcondition:        If attr_list_ptr is not NULL, the first attribute
**                          description is returned, otherwise NULL is returned
**    Description:          Return the first attribute description.
******************************************************************************/

struct string_list_struct* data_dict_get_attribute_desc_list(
    data_dict_attribute_list_struct* attr_list_ptr)
{
	if(attr_list_ptr != NULL) {
		return attr_list_ptr->first_description;
	}
	return (struct string_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_next_attribute_desc
**    Precondition:         attr_desc_list_ptr may or may not be NULL.
**    Postcondition:        If attr_desc_list_ptr is not NULL, the next
**                          attribute description is returned, otherwise NULL is
**                          returned.
**    Description:          Return the next attribute description.
******************************************************************************/

struct string_list_struct* data_dict_get_next_attribute_desc(
    struct string_list_struct* attr_desc_list_ptr)
{
	if(attr_desc_list_ptr != NULL) {
		return attr_desc_list_ptr->next_string;
	}
	return (struct string_list_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_get_attribute_desc
**    Precondition:         attr_desc_list_ptr may or may not be NULL
**    Postcondition:        If attr_desc_list_ptr is valid, attr_desc will be
**                          populated with the corresponding attribute
**                          description.
**    Description:          Get the attribute description associated with the
**                          given attribute description list.
******************************************************************************/

void data_dict_get_attribute_desc(struct string_list_struct* attr_desc_list_ptr,
    char** attr_desc)
{
	if(attr_desc_list_ptr != NULL) {
		if((*attr_desc = (char *)calloc(
		    strlen(attr_desc_list_ptr->cstring)+1,sizeof(char)))
		    == NULL) {
			pmlprint(ERROR,PMLMEM,
			    "data_dict_get_attribute_desc",NULL);
		} else {
			strcpy(*attr_desc, attr_desc_list_ptr->cstring);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_set_level
**    Precondition:         element_ptr is a pointer to the node to be modified
**    Postcondition:        If the element_ptr is not null, its level will be
**                          set to the specified value.
**    Description:          Set the level of the given data dictionary element.
******************************************************************************/

void data_dict_set_level(data_dict_element_struct* element_ptr, char* level)
{
	if(element_ptr != NULL) {
		if((element_ptr->level = (char *)calloc(strlen(level)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,
			    "data_dict_set_level",NULL);
		} else {
 			 strcpy(element_ptr->level, level);
		} 
	}
}


/******************************************************************************
**    Function/Method Name: data_dict_set_name
**    Precondition:         element_ptr is a pointer to the node to be modified
**    Postcondition:        If the element_ptr is not null, its name will be
**                          set to the specified value.
**    Description:          Set the name of the given data dictionary element.
******************************************************************************/

void data_dict_set_name(data_dict_element_struct* element_ptr, char* name)
{
	if(element_ptr != NULL) {
		if((element_ptr->name = (char *)calloc(strlen(name)+1,
		    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,
			    "data_dict_set_name",NULL);
		} else {
			strcpy(element_ptr->name, name);
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_set_type
**    Precondition:         element_ptr is a pointer to the node to be modified
**    Postcondition:        "FALSE" is returned if the element_ptr is NULL or
**                          the specified type is.  An error message is logged
**                          to stdio.
**                          "TRUE" is returned if the the element's type field
**                          was set correctly.
**    Description:          Set the mode field in the given element.
******************************************************************************/

Boolean data_dict_set_type(data_dict_element_struct* element_ptr, char* type)
{
	char *module = "data_dict_set_type";
	Boolean returnval = TRUE;
	if(element_ptr == NULL) {
		pmlprint(ERROR,PMLDD,module,
        	    "Unable to set type. NULL ptr detected");
		returnval = FALSE;
	}

	if(returnval == TRUE) {
		if((element_ptr->type = (char *)calloc(strlen(type)+1,
	   	    sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
		} else {
			strcpy(element_ptr->type, type);
	}
	return returnval;
}

/******************************************************************************
**    Function/Method Name: data_dict_set_mode
**    Precondition:         element_ptr is a pointer to the node to be modified
**                          mode is the desired mode and can be no more than
**    Postcondition:        "FALSE" is returned if the element_ptr is NULL or
**                          the specified mode is an invalid type.  An error
**                          message is logged to stdio.
**                          "TRUE" is returned if the the element's mode field
**                          was set correctly.
**    Description:          Set the mode field in the given element if the mode
**                          is valid.
******************************************************************************/

Boolean data_dict_set_mode(data_dict_element_struct* element_ptr, char* mode)
{
	char *module = "data_dict_set_mode";
	Boolean returnval = TRUE;
	if(element_ptr == NULL) {
		pmlprint(ERROR,PMLDD,module,
		    "Unable to set mode. NULL ptr detected");
		returnval = FALSE;
	}

	if(returnval == TRUE) {
		if((element_ptr->mode = (char *)calloc(strlen(mode)+1,
		    sizeof(char))) != NULL) {
			strcpy(element_ptr->mode, mode);
		} else {
			pmlprint(ERROR,PMLMEM,module,NULL);
			returnval = FALSE;
		}
	}
	return returnval;
}

/******************************************************************************
**    Function/Method Name: add_level_info_to_data_dictionary
**    Precondition:         dictionary_ptr is a pointer to the data dictionary
**    Postcondition:        "FALSE" is returned if the dictionary_ptr is NULL or
**                          if there is no data in the dictionary.  An error
**                          message is logged to stdio.
**                          "TRUE" is returned if all of the level information
**                          was added correctly.
**    Description:          populate the data dictionary with information about
**                          its level
******************************************************************************/

Boolean add_level_info_to_data_dictionary(data_dictionary_struct*dictionary_ptr)
{
	char *module = "add_level_info_to_data_dictionary";
	data_dict_element_struct *element_ptr = NULL;

	if(dictionary_ptr == NULL) {
		pmlprint(ERROR,PMLDD,module,
		    "Unable to add level info.  NULL data dictionary detected");
		return FALSE;
	}

	/* get the root element in the data dictionary */
	element_ptr = data_dict_get_root(dictionary_ptr);

	if(element_ptr == NULL) {
		pmlprint(ERROR,PMLDD,module,
		    "Unable to add level info.  NULL data dictionary detected");
		return  FALSE;
	}

	/* element_ptr is the root node, set its level = 1 */
	data_dict_set_level(element_ptr, "1");

	/* use a dfs algorithm to set the level appropriately */
	add_level_info_recursively(element_ptr);

	return TRUE;
}

/******************************************************************************
**    Function/Method Name: add_level_info_recursively
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**    Postcondition:        Every element in the data dictionay will have its
**                          level field populated according to the notation
**                          described in the official CPML file document.
**    Description:          Recursive function used to populate the data
**                          dictionary with information about its level.
******************************************************************************/

void add_level_info_recursively(data_dict_element_struct* element_ptr)
{
	char *module = "add_level_info_recursively";
	char **parent_level;
	char *child_level;
	int  num_children;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct* child_element_ptr = NULL;

	if(element_ptr == NULL) {
		return;
	}

	/* reset the parent and child level */
	if ((parent_level = malloc(sizeof(char *))) == NULL) {
		pmlprint(ERROR,PMLMEM,module,NULL);
		return;
	}

	/* get the elements level */
	data_dict_get_level(element_ptr, parent_level);

	/* get the children associated with the current element */
	child_list_ptr = data_dict_get_child_list(element_ptr);

	/* simple test to show how to access the data dictionary */
	num_children = 0;
	while(child_list_ptr != NULL) {
		num_children++;
		child_element_ptr = data_dict_get_child(child_list_ptr);
	
		/* set the child's level */
		if((child_level = (char *)calloc(strlen(*parent_level)+
		    sizeof(int)+2,sizeof(char))) == NULL) {
			pmlprint(ERROR,PMLMEM,module,NULL);
		} else {
			sprintf(child_level,"%s.%d", 
			    *parent_level, num_children);
			data_dict_set_level(child_element_ptr, child_level);
			free(child_level);
		}
		
		/* recursively traverse the data dictionary tree */
		add_level_info_recursively(child_element_ptr);

		/* get the next child ptr */
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
	}
	free(parent_level);
}

/******************************************************************************
**    Function/Method Name: data_dict_add_child_element
**    Precondition:         parent_ptr is a pointer to a valid element.
**                          child_ptr is a pointer to a valid element.
**    Postcondition:        "FALSE" is returned if the parent_ptr is NULL, or
**                          if there is an error allocating memory for the new
**                          child pointer in the parent.  An error message will
**                          be displayed to stdio.
**                          "TRUE" is returned and a pointer will be added to
**                          the parent to point to the specified child element
**                          if there are no problems.
**    Description:          Add the child ptr to the specified parent ptr
******************************************************************************/

Boolean data_dict_add_child_element(data_dict_element_struct *parent_ptr,
    data_dict_element_struct *child_ptr)
{
	char *module = "data_dict_add_child_element";
	data_dict_element_list_struct *temp_ptr;
	data_dict_element_list_struct *new_ptr;

	if(parent_ptr == NULL) {
		pmlprint(ERROR,PMLDD,module,
		   "Unable to add child, NULL parent");
		return FALSE;
	}

	/* add the child to the end of the parent's children list */
	temp_ptr = parent_ptr->first_child_ptr;

	/* allocate memory for the new child list ptr */
	if((new_ptr = (data_dict_element_list_struct*)calloc(1,
	   sizeof(data_dict_element_list_struct))) == NULL) {
		pmlprint(ERROR,PMLMEM,module,NULL);
		return FALSE;
	}

	/* populate the new ptr */
	new_ptr->data_ptr = child_ptr;

	if(temp_ptr == NULL) {
		temp_ptr = new_ptr;
		parent_ptr->first_child_ptr = new_ptr;
	} else {
		while(temp_ptr->next_element != NULL) {
			temp_ptr = temp_ptr->next_element;
		}
		temp_ptr->next_element = new_ptr;
	}

	return TRUE;
}

/******************************************************************************
**    Function/Method Name: data_dict_dfs_find_element
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**                          match_found is an int* that initially points to
**                          "FALSE"
**    Postcondition:        If the name specified was found in an element in
**                          the data dictionary, match_found will point to
**                          "TRUE" and a pointer to that element will be
**                          returned.
**                          If the name specified was not found in an element
**                          in the data dictionary, match_found will point to
**                          "FALSE" and a NULL pointer will be returned.
**    Description:          Perform a depth first search on the tree starting
**                          at the data dictionary root, with the element name
**                          we are trying to find and a flag telling if we have
**                          found a match.
******************************************************************************/

data_dict_element_struct* data_dict_dfs_find_element(
    data_dict_element_struct *element, char* name, int* match_found)
{
	data_dict_element_list_struct *temp_ptr = NULL;
	data_dict_element_struct *result_ptr = NULL;

	/* check to see if we have reached a tree leaf */
	if(element == NULL) {
		return (data_dict_element_struct*)NULL;
	}

	/* check to see if we have a match */
	if(strcmp(element->name, name) == 0) {
		*match_found = TRUE;
		return element;
	}

	/* recursively call the dfs find function on all remaining children */
	temp_ptr = element->first_child_ptr;

	while(temp_ptr != NULL) {
		/* make the recursive call */
		result_ptr = data_dict_dfs_find_element(temp_ptr->data_ptr,
		    name,match_found);

		/* if a match was found, then terminate the search and return */
		if(*match_found == TRUE) {
				return result_ptr;
		}

		temp_ptr = temp_ptr->next_element;
 	}

	/* if we made it this far, then a match was not found */
	return (data_dict_element_struct*)NULL;
}

/******************************************************************************
**    Function/Method Name: data_dict_dfs_print
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**    Postcondition:        The contents of the data dictionary have been
**                          displayed to stdio.
**    Description:          Print out the data dictionary to stdio using a
**                          depth first search algorithm.  This function is
**                          intended to be used to debug the data dictionary.
******************************************************************************/

void data_dict_dfs_print(data_dict_element_struct *element)
{
	data_dict_element_list_struct *temp_ptr = NULL;

	/* check to see if we have reached a tree leaf */
	if(element != NULL) {
		/* print out the current element */
		data_dict_print_element(element);

		/*
		 * Recursively call the dfs find function on all
		 * remaining children.
		 */
		temp_ptr = element->first_child_ptr;

		while(temp_ptr != NULL) {
			/* make the recursive call */
			data_dict_dfs_print(temp_ptr->data_ptr);
			temp_ptr = temp_ptr->next_element;
		}
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_print_element
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**    Postcondition:        The contents of the element has been displayed to
**                          stdio.
**    Description:          Print out the contents of the element to stdio.
**                          This is routine is intendeded to be used to debug
**                          the data dictionary
******************************************************************************/

void data_dict_print_element(data_dict_element_struct *element)
{
	data_dict_attribute_list_struct *temp_attr_ptr = NULL;
	struct string_list_struct *temp_string_ptr = NULL;
	data_dict_element_list_struct *temp_child_ptr = NULL;

	data_dict_element_struct* temp = NULL;

	if(element != NULL) {
		printf("\n\n\tLevel          : %s", element->level);
		printf("\n\tName           : %s", element->name);
		printf("\n\tType           : %s", element->type);
		printf("\n\tMode           : %s", element->mode);

		temp_attr_ptr = element->first_attribute_ptr;
		while(temp_attr_ptr != NULL) {
			printf("\n\tAttribute      : %s", 
			    temp_attr_ptr->attribute);

			temp_string_ptr = temp_attr_ptr->first_description;
			while(temp_string_ptr != NULL) {
				printf("  \"%s\"", temp_string_ptr->cstring);
				temp_string_ptr = temp_string_ptr->next_string;
			}

			temp_attr_ptr = temp_attr_ptr->next_attribute;
		}

		if(element->parent_ptr != NULL) {
			temp = element->parent_ptr;
			printf("\n\tParent name    : %s", temp->name);
		} else {
			printf("\n\tParent name    : root");
		}
		temp_child_ptr = element->first_child_ptr;
		printf("\n\tChildren names : ");
		while(temp_child_ptr != NULL) {
			temp = temp_child_ptr->data_ptr;
			printf("  %s", temp->name);
			temp_child_ptr = temp_child_ptr->next_element;
		}
		printf("\n");
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_dfs_remove
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**    Postcondition:        The contents of the data dictionary have been
**                          displayed to stdio.
**    Description:          Remove elements from the data dictionary.
******************************************************************************/

void data_dict_dfs_remove(data_dict_element_struct *element)
{
	data_dict_element_list_struct *temp_ptr = NULL;
	data_dict_element_list_struct *delete_ptr = NULL;

	/* check to see if we have reached a tree leaf */
	if(element != NULL) {

		/*
		 * Recursively call the dfs find function on all
		 * remaining children.
		 */
		temp_ptr = element->first_child_ptr;

		while(temp_ptr != NULL) {
			/* make the recursive call */
			data_dict_dfs_remove(temp_ptr->data_ptr);
			delete_ptr = temp_ptr;
			temp_ptr = temp_ptr->next_element;
			free(delete_ptr);
			delete_ptr = NULL;
			
		}
		free(element);
		element = NULL;
	}
}

/******************************************************************************
**    Function/Method Name: data_dict_delete_element_
**    Precondition:         element is a pointer to a data_dict_element_struct
**                          and may or may not be NULL.
**    Postcondition:        The contents of the element has been displayed to
**                          stdio.
**    Description:          Delete the contents of each element.
******************************************************************************/

void data_dict_delete_element(data_dict_element_struct *element)
{
	data_dict_attribute_list_struct *temp_attr_ptr = NULL;
	data_dict_attribute_list_struct *delete_attr_ptr = NULL;
	struct string_list_struct *temp_string_ptr = NULL;
	struct string_list_struct *delete_string_ptr = NULL;

	data_dict_element_struct* temp = NULL;

	if(element != NULL) {
		if(element->level != NULL) {
			free(element->level);
		}
		if(element->name != NULL) {
			free(element->name);
		}
		if(element->type!= NULL) {
			free(element->type);
		}
		if(element->mode!= NULL) {
			free(element->mode);
		}

		delete_attr_ptr = element->first_attribute_ptr;
		temp_attr_ptr = element->first_attribute_ptr;
		while(temp_attr_ptr != NULL) {

			temp_string_ptr = temp_attr_ptr->first_description;
			delete_string_ptr = temp_attr_ptr->first_description;
			while(temp_string_ptr != NULL) {
				temp_string_ptr = temp_string_ptr->next_string;
				free(delete_string_ptr);
				delete_string_ptr = temp_string_ptr;
			}
			temp_attr_ptr = temp_attr_ptr->next_attribute;
			free(delete_attr_ptr->attribute);
			delete_attr_ptr = temp_attr_ptr;

		}
	}
}
