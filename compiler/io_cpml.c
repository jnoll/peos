/******************************************************************************
**  File Information:  This module generates output file (cpml) by traversing
**                     data dictionary.  The program
**                     generates output file with pml (input) file's extension
**                     replaced with "cpml".
**
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <gdbm.h>

#include "datadict.h"
#include "defs.h"
#include "io_cpml.h"

/* line count used for the jump err inst */
static int error_line = 0;
data_dict_element_struct* root;
VAR_STRUCT var_list[MAX_VARS];
int var_list_ptr = 0;
char* ready_list[MAX_VARS];
int ready_list_ptr = 0;

/******************************************************************************
**
**    Function/Method Name: write_cpml
**    Precondition:  data dictionary.
**    Postcomdition: output file generated.
**                   Returns TRUE, if successful or returns FALSE if failed
**                   to generate output file. 
**
**    Description:   This is the interface function for other modules.
**                   This function generates the CPML file, given a 
**                   data_dictionary_struct.  CPML file name is derived from 
**                   "pml_filename".
**
******************************************************************************/
Boolean write_cpml(char* pml_filename, data_dictionary_struct* dictionary_ptr, char* filetype)
{

    data_dict_element_struct *element_ptr = NULL;
    OUTPUT_STRUCT output;
    char *filename;
    int line_count = 0;

    /* Generate cpml file name. */
    if (strlen (pml_filename) <= 0) return FALSE;
    filename = create_cpml_filename (pml_filename, filename, filetype); 
    if (filename == (char *) NULL) {
        printf ("Failed to generate cpml file name.\n");
        return FALSE;
    }
    if (strlen (filename) == 0) {
        printf ("Failed to generate CPML file name.\n");
        return FALSE;
    }

#if DEBUG == 1
printf ("new file name is %s\n", filename);
#endif

    if ((strcmp (filetype, TEXT_MODE) == 0) || 
	(strcmp (filetype, CPML_MODE) == 0)) {
    	output.fptr = fopen (filename, "w");
	if (output.fptr == NULL)
		printf("opening file %s failed: %s\n",filename,strerror(errno));
    }
    else if (strcmp(filetype,GDBM_MODE) == 0) {
	output.dbf = create_gdbm(filename, "write");
    	if (output.dbf == (GDBM_FILE) NULL) {
		printf("Failed to create gdbm.\n");
		return FALSE;
	}
    }

    /* get the root element in the data dictionary */
    root = data_dict_get_root(dictionary_ptr);
    element_ptr = root;
    /* write out the data segment */
    line_count = write_data(output,element_ptr,filetype,line_count);
    error_line = path_length(element_ptr)+line_count+2;
    /* use a dfs algorithm to set the level appropriately */
    write_cpml_recursively(output, line_count, element_ptr,filetype);

    /* test code only, should be commented out in release code 
    test_retrieval(output);
    test code end */

    if ((strcmp (filetype, TEXT_MODE) == 0) || 
	(strcmp (filetype, CPML_MODE) == 0)) 
	fclose (output.fptr);
    else if (strcmp(filetype,GDBM_MODE) == 0)
	gdbm_close(output.dbf);
    else {
	printf("Failed to close output file/gdbm.\n");
	return FALSE;
    }

    printf ("\nCPML generated.\n");
    return TRUE;
}



/******************************************************************************
**
**  Function/Method Name: print_children
**  Precondition:	  Valid output information and a pointer to data dict.
**  Postcondition:	  Output of a list of the names of children.
**  Description:	  The procedure takes a point in the data tree and prints
**			  the names of all the children of that point.
**
******************************************************************************/
char* print_children(char* output_str, data_dict_element_struct* element_ptr)
{
	char **name;
	char **type;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct *child_element_ptr = NULL;

	if ((name = malloc(sizeof(char *))) == NULL) {
	  printf("\nERROR[print_children]:memory allocation\n");
          return NULL;
        }
	if ((type = malloc(sizeof(char *))) == NULL) {
	  printf("\nERROR[print_children]:memory allocation\n");
          return NULL;
        }


	/* get each child and append it to given string */
	child_list_ptr = data_dict_get_child_list(element_ptr);
	while (child_list_ptr != NULL)
	{
		child_element_ptr = data_dict_get_child(child_list_ptr);
		data_dict_get_name(child_element_ptr, name);
		data_dict_get_type(child_element_ptr, type);
		/* if this is a child we do not want to print then skip, otherwise append */
		strcat(output_str,*name);
		strcat(output_str," ");
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
	}
	free(name);
	free(type);
	return output_str; 
}

/******************************************************************************
**
** Function/Method Name: get_actions_query
** Precondition:	 A list to put the actions in, the place to start in the tree,
**			 and a flag all need to be passed in.
** Postcondition:	 The given list will be filled in with actions seperated by spaces.
** Description:		 The function finds all possible actions that can come next,
**			 given a place in the process tree.
**
******************************************************************************/
char* get_actions_query(char action_str[1024], data_dict_element_struct* element_ptr, Boolean iteration_flag, char* attr_type)
{
	char **name;
	char **type;
	char **desc;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct *child_element_ptr = NULL;
	data_dict_attribute_list_struct* attr_list = NULL;
	char temp_str[1024] = "\0";
	strcpy(action_str,"");

	if ((name = malloc(sizeof(char *))) == NULL) {
	  printf("\nERROR[get_actions_query]:memory allocation\n");
          return NULL;
        }
	if ((type = malloc(sizeof(char *))) == NULL) {
	  printf("\nERROR[get_actions_query]:memory allocation\n");
          return NULL;
        }
	if ((desc = malloc(sizeof(char *))) == NULL) {
	  printf("\nERROR[get_actions_query]:memory allocation\n");
          return NULL;
        }

	data_dict_get_type(element_ptr,type);
	if ((strcmp(*type,"action") == 0) || (strcmp(*type,"branch") == 0)) {
		attr_list = data_dict_get_attribute_list(element_ptr);
		data_dict_get_attribute_type(attr_list,type);
		while (strcmp(attr_type,*type) != 0 && attr_list != NULL) {
			attr_list = data_dict_get_next_attribute(attr_list);
			data_dict_get_attribute_type(attr_list,type);
		}
		data_dict_get_attribute_desc(data_dict_get_attribute_desc_list(attr_list),desc);
		sprintf(action_str,"%s ",*desc);
		free(name);
		free(type);
		free(desc);
		return action_str;
	}
	else if ((strcmp(*type,"sequence") == 0) ||
		(strcmp(*type,"task") == 0)) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		strcat(action_str,get_actions_query(temp_str,child_element_ptr,FALSE,*type));
		free(name);
		free(type);
		free(desc);
		return action_str;
	}
	else if (strcmp(*type,"iteration") == 0) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		strcat(action_str,get_actions_query(temp_str,child_element_ptr,FALSE,attr_type));
		/* there are some times when we do not want to find the actions
		 * that come after the iteration */
		if (iteration_flag != TRUE) {
			element_ptr = get_next_action(element_ptr);
			if (element_ptr != NULL) {
				 strcat(action_str,get_actions_query(temp_str,element_ptr,FALSE,attr_type));
			}
		}
		free(name);
		free(type);
		free(desc);
		return action_str;
	}
	else if (strcmp(*type,"selection") == 0) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL) {
			child_element_ptr = data_dict_get_child(child_list_ptr);
			strcat(action_str,get_actions_query(temp_str,child_element_ptr,TRUE,attr_type));
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		free(name);
		free(type);
		free(desc);
		return action_str;
	}
}

/******************************************************************************
**
** Function/Method Name: get_actions
** Precondition:	 A list to put the actions in, the place to start in the tree,
**			 and a flag all need to be passed in.
** Postcondition:	 The given list will be filled in with actions seperated by spaces.
** Description:		 The function finds all possible actions that can come next,
**			 given a place in the process tree.
**
******************************************************************************/
char* get_actions(char action_str[1024], data_dict_element_struct* element_ptr, Boolean iteration_flag)
{
	char **name;
	char **type;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct *child_element_ptr = NULL;
	char temp_str[1024] = "\0";
	strcpy(action_str,"");

        if ((name = malloc(sizeof(char *))) == NULL) {
           printf("\ERROR[get_actions]:memory allocation\n");
           return NULL;
        }
        if ((type = malloc(sizeof(char *))) == NULL) {
           printf("\ERROR[get_actions]:memory allocation\n");
           return NULL;
        }

	data_dict_get_type(element_ptr,type);
	if ((strcmp(*type,"action") == 0) || (strcmp(*type,"branch") == 0)) {
		data_dict_get_name(element_ptr,name);
		sprintf(action_str,"%s ",*name);
		free(*name);
		free(name);
		free(*type);
		free(type);
		return action_str;
	}
	else if ((strcmp(*type,"sequence") == 0) ||
		(strcmp(*type,"task") == 0)) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		free(*type);
		free(type);
		return get_actions(temp_str,child_element_ptr,FALSE);
	}
	else if (strcmp(*type,"iteration") == 0) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		strcat(action_str,get_actions(temp_str,child_element_ptr,FALSE));
		/* there are some times when we do not want to find the actions
		 * that come after the iteration */
		if (iteration_flag != TRUE) {
			element_ptr = get_next_action(element_ptr);
			if (element_ptr != NULL) {
				 strcat(action_str,get_actions(temp_str,element_ptr,FALSE));
			}
		}
		free(*type);
		free(type);
		return action_str;
	}
	else if (strcmp(*type,"selection") == 0) {
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL) {
			child_element_ptr = data_dict_get_child(child_list_ptr);
			strcat(action_str,get_actions(temp_str,child_element_ptr,TRUE));
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		free(*type);
		free(type);
		return action_str;
	}
}

/******************************************************************************
**
** Function/Method Name: get_tokens
** Precondition:	 A list of actions seperated by spaces, a blank list to put
**			 the new list, and a index and range of actions that do
**			 not go into list.
** Postcondition:	 A new list with actions specified by the index and range.
** Description:		 The function goes through the list of actions and puts the
**			 actions in the new list if they are not in the range specified
**			 by the parameters.  Omit range == index...index+number-1.
**
******************************************************************************/
char* get_tokens(char action_str[1024], char temp_str[1024], int index, int number)
{
	char temp[1024];
	char* action;
	int count = 1;

	/* dont want to use action_str to keep it in original form */
	strcpy(temp,action_str);
	strcpy(temp_str,"");
	action = (char*) strtok(temp, " ");
	while ( action != NULL) {
		if ((count < index) || (count >= index+number)) {
			strcat(temp_str,action);
			strcat(temp_str," ");
		}
		action = (char*) strtok(NULL," ");
		count += 1;
	}	
	return temp_str;
}

/******************************************************************************
**
** Function/Method Name: print_end
** Precondition:	 A list of actions seperated by spaces.
** Postcondition:	 A list with only the last action in it.
** Description:		 The function goes through the list and stops at the last action.
**
******************************************************************************/
char* print_end(char action_str[1024])
{
	char* action1;
	char* action2;
	char temp[1024];

	strcpy(temp,action_str);
	action1 = (char*) strtok(temp," ");
	while (action1 != NULL) {
		action2 = (char*) strtok(NULL," ");
		if (action2 == NULL)
			return action1;
		action1 = action2;
	}
}

/******************************************************************************
**
** Function/Method Name: print_allbut_end
** Precondition:	 A list of actions seperated by spaces and a valid blank list.
** Postcondition:	 The blank list will contain all of the actions except the last.
** Description:		 The function goes through the given list and creates a new list
**			 with all of the actions except the last one.
**
******************************************************************************/
char* print_allbut_end(char action_str[1024], char temp_str[1024])
{
	char* action1;
	char* action2;
	char temp[1024];

	strcpy(temp,action_str);
	strcpy(temp_str,"");
	action1 = (char*) strtok(temp," ");
	action2 = (char*) strtok(NULL," ");
	while (action2 != NULL) {
		strcat(temp_str,action1);
		strcat(temp_str," ");
		action1 = action2;
		action2 = (char*) strtok(NULL," ");
	}
	return temp_str;
}

/******************************************************************************
**
** Function/Method Name: get_next_action
** Precondition:	 A valid pointer into the process tree.
** Postcondition:	 A valid pointer into the process tree or NULL is returned.
** Description:		 The function goes through the process tree and finds the next
**			 action after that point.
**
******************************************************************************/
data_dict_element_struct* get_next_action(data_dict_element_struct* element_ptr)
{
	data_dict_element_struct* sibling;

	if (root == element_ptr)
		return NULL;
	sibling = data_dict_get_next_sibling(element_ptr);
	if (sibling == NULL) {
		element_ptr = data_dict_get_parent(element_ptr);
		return get_next_action(element_ptr);
	}
	else 
		return sibling;
}

/******************************************************************************
**
**  Function/Method Name: write_data
**  Precondition:	  Valid output information and a pointer to data dictionary.
**  Postcondition:	  Return current line number and outputs data op-code.
**  Description:	  The procedure traverses the tree and outputs the data types
**			  for the op-code.
**
******************************************************************************/
int write_data(OUTPUT_STRUCT output, data_dict_element_struct* element_ptr,
		char* filetype,int line_count)
{
	int count = line_count;
	char output_str[1024];
	char temp_str[1024];
	char **name;
	char **type;
	char **mode;
	char **attribute_type;
	char **attribute_description;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct *child_element_ptr = NULL;
	data_dict_attribute_list_struct *temp_attr_ptr = NULL;
	string_list_struct *temp_string_ptr = NULL;

	attribute_type = malloc(sizeof(char *));
	attribute_description = malloc(sizeof(char *));
	name = malloc(sizeof(char *));
	type = malloc(sizeof(char *));
	strcpy(output_str,"");

	data_dict_get_name(element_ptr, name);
	data_dict_get_type(element_ptr, type);

	/* if action, print out all of its attributes */
	if (strcmp(*type,"action") == 0)
	{
	        mode = malloc(sizeof(char *));	

		data_dict_get_mode(element_ptr,mode);
		sprintf(output_str,"%s type action mode %s ",*name,*mode);

		/* go through attribute list */
		temp_attr_ptr = data_dict_get_attribute_list(element_ptr);
		while (temp_attr_ptr != NULL)
		{
			data_dict_get_attribute_type(temp_attr_ptr,attribute_type);
			sprintf(temp_str,"%s { ",*attribute_type);
			free(*attribute_type);
			strcat(output_str,temp_str);

			/* print each description */
			temp_string_ptr = data_dict_get_attribute_desc_list(temp_attr_ptr);
			while (temp_string_ptr != NULL)
			{
				data_dict_get_attribute_desc(temp_string_ptr,attribute_description);
				sprintf(temp_str,"%s ",*attribute_description);
				strcat(output_str,temp_str);
				free(*attribute_description);
				temp_string_ptr = data_dict_get_next_attribute_desc(temp_string_ptr);
			}
			strcat(output_str,"} ");
			temp_attr_ptr = data_dict_get_next_attribute(temp_attr_ptr);
		}
		write_cpml_data(count,output_str,filetype,output);
		free(*name);
		free(name);
		free(*type);
		free(type);
		free(attribute_description);
		free(attribute_type);
                free(*mode);
	        free(mode);
		return count+1;
	}
	else if (strcmp(*type,"branch") == 0)
	{
		sprintf(output_str,"%s type %s children ( ",*name,*type);	
		strcpy(output_str,print_children(output_str,element_ptr));
		strcat(output_str,")");
		write_cpml_data(count,output_str,filetype,output);
		count += 1;
	}
	
	/* continue recursively traversing tree to print all data(nodes) */
	child_list_ptr = data_dict_get_child_list(element_ptr);
	while (child_list_ptr != NULL)
	{
		child_element_ptr = data_dict_get_child(child_list_ptr);
		count = write_data(output,child_element_ptr,filetype,count);
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
	}
	free(*name);
	free(name);
	free(*type);
	free(type);
	return count;
}




/******************************************************************************
**
**  Function/Method Name:  write_cpml_recursively
**  Precondition:  Empty output handler.  data dictionary.
**  Postcondition: CPML saved to a output file.  
**  Description:   recursive function used to write the op-code to the output.
**
******************************************************************************/
int write_cpml_recursively(OUTPUT_STRUCT output, int current_line,
                            data_dict_element_struct* element_ptr, char* filetype)
{
	char output_str[1024];
	char temp_str[1024];
	char action_str[1024] = "\0";
	static char var_name[64];
	char* query_str; 
	char* temp_ptr;
	char **name;
	char **type;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_struct *child_element_ptr = NULL;
	int temp_line = 0;
	int new_line = 0;
	int count = 1;
	int i,rc;
	
	strcpy(output_str, "");
	name = malloc(sizeof(char *));
	type = malloc(sizeof(char *));
	
	data_dict_get_type(element_ptr, type);
	data_dict_get_name(element_ptr, name);

	/* if process node, print out start, continue with recursive traversal, write end */
	if (strcmp(*type,"process") == 0)
	{
		strcat(output_str,"start");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			current_line = write_cpml_recursively(output,current_line,child_element_ptr,
								filetype);
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		strcpy(output_str,"end");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"call error");
		write_cpml_data(current_line,output_str,filetype,output);
	}
	else if (strcmp(*type,"iteration") == 0)
	{
		strcpy(output_str,"call select ");
		strcpy(action_str,get_actions_query(temp_str,element_ptr,FALSE,"requires"));
		strcpy(var_name,strtok(action_str,". "));
		rc = check_for_var(var_name);
		query_str = strtok(NULL,"& ");
		if (query_str != NULL) {
			strtok(NULL,". ");
			temp_ptr = strtok(NULL,"& ");
			while (temp_ptr != NULL) {
				strcat(query_str,"&&");
				strcat(query_str,temp_ptr);
				strtok(NULL,". ");
				temp_ptr = strtok(NULL,"& ");
			}
			strcat(output_str,query_str);
		}
		if (rc == -1)
			add_var(var_name, query_str);
		else {
			sprintf(temp_str,"&&id==$%s", var_name);
			strcat(output_str,temp_str);
		}
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		if (rc == -1) {
			sprintf(output_str,"pop %s",var_name);
		} else
			strcpy(output_str,"pop ");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		
		/* save current line so we cna jump back at end of loop */
		temp_line = current_line;
		strcpy(output_str,"call set ready ");
		strcpy(action_str,get_actions(temp_str,element_ptr,FALSE));
		strcat(output_str,action_str);
		strcpy(temp_str,action_str);
		temp_ptr = strtok(temp_str," ");
		while (temp_ptr != NULL) {
			add_to_ready(temp_ptr);
			temp_ptr = strtok(NULL," ");
		}
		if (get_next_action(element_ptr) == NULL)
			strcat(output_str,"end");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		sprintf(output_str,"jzero %d",error_line);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"pop");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"call wait active ");
		strcat(output_str,action_str);
		if (get_next_action(element_ptr) == NULL)
			strcat(output_str,"end");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		sprintf(output_str,"jump %d ",error_line);
		/* jump to either next line or over the iteration completely */
		/* path_length counts all lines of iteration loop, so we subtract 7 for the
		 * lines we already went over up to this point */
		
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		/* There might be several actions for each child in the tree, so 
		 * we must print jump values for each action we produce */
		free(*type);
		data_dict_get_type(child_element_ptr,type);
		if (strcmp(*type,"iteration") == 0) {
			if (get_next_action(child_element_ptr) != get_next_action(element_ptr))
				sprintf(temp_str,"%d %d %d",current_line+1,current_line+1,
					current_line+path_length(element_ptr)-7);
		}
		else if (strcmp(*type,"selection") == 0) {
			for(i=0;i<num_children(child_element_ptr);i++) {
				sprintf(temp_str,"%d ",current_line+1);
				strcat(output_str,temp_str);
			}
			sprintf(temp_str,"%d",current_line+path_length(element_ptr)-7);
		}
		else
			sprintf(temp_str,"%d %d",current_line+1,
				current_line+path_length(element_ptr)-7);
		strcat(output_str,temp_str);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		strcpy(output_str,"call set none ");
		strcpy(temp_str,action_str);
		if (get_next_action(element_ptr) == NULL)
			strcat(output_str,"end");
		else
			strcat(output_str,print_end(temp_str));
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		/* now go through each child in the iteration path */
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			current_line = write_cpml_recursively(output,current_line,child_element_ptr,
								filetype);
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}	

		/* goto start of iteration loop */
		strcpy(output_str,"goto ");
		sprintf(temp_str,"%d",temp_line);
		strcat(output_str,temp_str);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		strcpy(output_str,"call set none ");
		if (get_next_action(element_ptr) == NULL)
			strcat(output_str,action_str);
		else
			strcat(output_str,print_allbut_end(action_str,temp_str));
		write_cpml_data(current_line,output_str,filetype,output);
		free(*name);
		free(name);
		free(*type);
		free(type);
		return current_line+1;
	}
	else if (strcmp(*type,"selection") == 0)
	{
		strcpy(output_str,"call select ");
		strcpy(action_str,get_actions_query(temp_str,element_ptr,FALSE,"requires"));
		strcpy(var_name,strtok(action_str,". "));
		rc = check_for_var(var_name);
		query_str = strtok(NULL,"& ");
		if (query_str != NULL) {
			strtok(NULL,". ");
			temp_ptr = strtok(NULL,"& ");
			while (temp_ptr != NULL) {
				strcat(query_str,"&&");
				strcat(query_str,temp_ptr);
				strtok(NULL,". ");
				temp_ptr = strtok(NULL,"& ");
			}
			strcat(output_str,query_str);
		}
		if (rc == -1)
			add_var(var_name, query_str);
		else {
			sprintf(temp_str,"&&id==$%s", var_name);
			strcat(output_str,temp_str);
		}
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		if (rc == -1) {
			sprintf(output_str,"pop %s",var_name);
		} else
			strcpy(output_str,"pop ");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		strcpy(output_str,"call set ready ");
		strcpy(action_str,get_actions(temp_str,element_ptr,FALSE));
		strcat(output_str,action_str);
		strcpy(temp_str,action_str);
		temp_ptr = strtok(temp_str," ");
		while (temp_ptr != NULL) {
			add_to_ready(temp_ptr);
			temp_ptr = strtok(NULL," ");
		}
		write_cpml_data(current_line,output_str,filetype,output);
		/* compute line so we can jump to end of selection at the end of eack task */
		temp_line = current_line + path_length(element_ptr);
		current_line += 1;
		sprintf(output_str,"jzero %d",error_line);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"pop");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"call wait active ");
		strcat(output_str,action_str);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		/* need to find all places to jump for selection */
		sprintf(output_str,"jump %d",error_line);
		/* get the line numbers for all possible selections of loop */
		child_list_ptr = data_dict_get_child_list(element_ptr);
		child_element_ptr = data_dict_get_child(child_list_ptr);
		/* There might be several actions for each child in the tree, so
		 * we must print out line numbers for each one */
		data_dict_get_type(child_element_ptr,type);
		if (strcmp(*type,"selection") == 0) {
			for (i=0;i<num_children(child_element_ptr);i++) {
				sprintf(temp_str," %d",current_line+1);
				strcat(output_str,temp_str);
			}
		}
		else {
			sprintf(temp_str," %d",current_line+1);
			strcat(output_str,temp_str);
		}
		new_line = current_line+1;
		while (child_list_ptr != NULL)
		{
			if (data_dict_get_next_child(child_list_ptr) != NULL)
			{
				child_element_ptr = data_dict_get_child(child_list_ptr);
				new_line += path_length(child_element_ptr) + 2;
				data_dict_get_type(data_dict_get_child(data_dict_get_next_child(child_list_ptr)),type);
				if (strcmp(*type,"selection") == 0) {
					for (i=0;i<num_children(data_dict_get_child(data_dict_get_next_child(child_list_ptr)));i++) {
						sprintf(temp_str," %d",new_line);
						strcat(output_str,temp_str);
					}
				}
				else {
					sprintf(temp_str," %d",new_line);
					strcat(output_str,temp_str);
				}
			}
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		write_cpml_data(current_line,output_str,filetype,output);
		current_line +=1;

		count = 1;
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			strcpy(output_str,"call set none ");
			free(*type);
			data_dict_get_type(child_element_ptr,type);
			if (strcmp(*type,"selection") == 0) {
				strcat(output_str,get_tokens(action_str,temp_str,count,num_children(child_element_ptr)));
				count += num_children(child_element_ptr);
			}
			else {	
				strcat(output_str,get_tokens(action_str,temp_str,count,1));
				count += 1;
			}
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			current_line = write_cpml_recursively(output,current_line,child_element_ptr,
								filetype);
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
			/* if we are not on the last task, print a goto to end of selection */
			if (child_list_ptr != NULL)
			{
				strcpy(output_str,"goto");
				sprintf(temp_str," %d",temp_line);
				strcat(output_str,temp_str);
				write_cpml_data(current_line,output_str,filetype,output);
				current_line += 1;
			}
		}
		free(*name);
		free(name);
		free(*type);
		free(type);
		return current_line;
	}
	else if (strcmp(*type,"branch") == 0)
	{
		strcpy(output_str,"push 0");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		child_list_ptr = data_dict_get_child_list(element_ptr);
		/* compute where first task will actually start (for fork call) */
		temp_line = current_line + num_children(element_ptr)*4 + 6;
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			sprintf(output_str,"call fork %d",temp_line);
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			sprintf(output_str,"jzero %d",error_line);
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			strcpy(output_str,"pop");
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			strcpy(output_str,"incr");
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			/* compute where next task starts */
			temp_line += path_length(child_element_ptr) + 1;
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}

		strcpy(output_str,"call join");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		sprintf(output_str,"jzero %d",error_line);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"pop");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"decr");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		/* subtracting 13 comes from the lines that were counted for the branch
		 * loop that we already passed */
		sprintf(output_str,"jzero %d",current_line+path_length(element_ptr)-13);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		sprintf(output_str,"goto %d",current_line-5);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;

		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			current_line = write_cpml_recursively(output,current_line,child_element_ptr,
								filetype);
			strcpy(output_str,"call exit");
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		free(*name);
		free(name);
		free(*type);
		free(type);
		return current_line;
	}
	else if (strcmp(*type,"action") == 0)
	{
		if (check_for_ready(*name) == -1) {
			strcpy(output_str,"call select ");
			strcpy(action_str,get_actions_query(temp_str,element_ptr,FALSE,"requires"));
			strcpy(var_name,strtok(action_str,". "));
			rc = check_for_var(var_name);
			query_str = strtok(NULL,"& ");
		 	if (query_str != NULL) {
		 		strtok(NULL,". ");
				temp_ptr = strtok(NULL,"& ");
				while (temp_ptr != NULL) {
					strcat(query_str,"&&");
					strcat(query_str,temp_ptr);
					strtok(NULL,". ");
					temp_ptr = strtok(NULL,"& ");
				}
				strcat(output_str,query_str);
			}
			else if (rc != -1)
				strcat(output_str,var_list[rc].var_desc);
			if (rc == -1)
				add_var(var_name, query_str);
			else {
				sprintf(temp_str,"&&id==$%s", var_name);
				strcat(output_str,temp_str);
			}
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			if (rc == -1) {
				sprintf(output_str,"pop %s",var_name);
			} else
				strcpy(output_str,"pop ");
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;

			sprintf(output_str,"call set ready %s",*name);
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			sprintf(output_str,"jzero %d",error_line);
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
			strcpy(output_str,"pop");
			write_cpml_data(current_line,output_str,filetype,output);
			current_line += 1;
		}
		sprintf(output_str,"call wait done %s",*name);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		sprintf(output_str,"jzero %d",error_line);
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"pop");
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		strcpy(output_str,"call select ");
		strcpy(action_str,get_actions_query(temp_str,element_ptr,FALSE,"provides"));
		strcpy(var_name,strtok(action_str,". "));
		rc = check_for_var(var_name);
		query_str = strtok(NULL,"& ");
		if (query_str != NULL) {
		 	strtok(NULL,". ");
			temp_ptr = strtok(NULL,"& ");
			while (temp_ptr != NULL) {
				strcat(query_str,"&&");
				strcat(query_str,temp_ptr);
				strtok(NULL,". ");
				temp_ptr = strtok(NULL,"& ");
			}
			strcat(output_str,query_str);
		}
		if (rc == -1)
			add_var(var_name, query_str);
		else {
			sprintf(temp_str,"&&id==$%s",var_name);
			strcat(output_str,temp_str);
		}
		write_cpml_data(current_line,output_str,filetype,output);
		current_line += 1;
		if (rc == -1) {
			sprintf(output_str,"pop %s",var_name);
		} else
			strcpy(output_str,"pop ");
		write_cpml_data(current_line,output_str,filetype,output);
		free(*name);
		free(name);
		free(*type);
		free(type);
		return current_line+1;
	}
	else if ((strcmp(*type,"sequence") == 0) ||
		(strcmp(*type,"task") == 0)) {
		/* we basically do nothing except continue down the tree */
		child_list_ptr = data_dict_get_child_list(element_ptr);
		while (child_list_ptr != NULL)
		{
			child_element_ptr = data_dict_get_child(child_list_ptr);
			current_line = write_cpml_recursively(output,current_line,child_element_ptr,
								filetype);
			child_list_ptr = data_dict_get_next_child(child_list_ptr);
		}
		free(*name);
		free(name);
		free(*type);
		free(type);
		return current_line;
	}
	else 
	{
		printf("Action %s currently not supported or incorrect\n",*type);
	}
	return 0;
}

/******************************************************************************
**
**  Function/Method Name:  path_length
**  Precondition:	   A valid pointer into the data dict.
**  Postcondition:	   A count of the length of the path in the data tree.
**  Description:	   Given a point in the data tree, the function estimates
**			   what the length in lines of code the rest of the path
**			   will be.
**
******************************************************************************/
int path_length(data_dict_element_struct* element_ptr)
{
	int sum = 0;
	char **type;
	data_dict_element_list_struct *child_list_ptr = NULL;
	data_dict_element_list_struct *first_child = NULL;
	data_dict_element_struct *child_element_ptr = NULL;

	if ((type = malloc(sizeof(char *))) == NULL) {
		printf("\nERROR[path_length]-memory allocation\n");
		return sum;
	}
	child_list_ptr = data_dict_get_child_list(element_ptr);
	first_child = child_list_ptr;
	while (child_list_ptr != NULL)
	{
		child_element_ptr = data_dict_get_child(child_list_ptr);
		data_dict_get_type(child_element_ptr, type);
		if (strcmp(*type,"action") == 0) {
			data_dict_get_type(element_ptr, type);
			if (strcmp(*type,"selection") == 0)
				sum += 5;
			else if (strcmp(*type,"iteration") == 0 && child_list_ptr == first_child)
				sum += 5;
			else
				sum += 10;
		} else
			sum += path_length(child_element_ptr);
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
	}
	data_dict_get_type(element_ptr, type);
	/* all of these numbers come from the op-code specification and need to be changed
	 * if the op-code changes at all! */
	if (strcmp(*type,"branch") == 0)
		sum += num_children(element_ptr) + 15;
	else if (strcmp(*type,"action") == 0) {
		 element_ptr = data_dict_get_parent(element_ptr);
		 data_dict_get_type(element_ptr, type);
         if (strcmp(*type,"selection") == 0)
            sum += 5;
         else if (strcmp(*type,"iteration") == 0 && child_list_ptr == first_child)
            sum += 5;
         else
            sum += 10;
	} else if (strcmp(*type,"iteration") == 0)
		sum += 10;
	else if (strcmp(*type,"selection") == 0)
		sum += num_children(element_ptr)*2 + 6;
	free(*type);
	free(type);
	return sum;
}

/******************************************************************************
**
**  Function/Method Name:  num_children
**  Precondition:	   A valid data dict. pointer
**  Postcondition:	   A count of the number of children of a node in dict.
**  Description:	   Given a point in the data dictionary, count and return
**			   the number of children it has.
**
******************************************************************************/
int num_children(data_dict_element_struct* element_ptr)
{
	int sum = 0;
	data_dict_element_list_struct *child_list_ptr = NULL;

	child_list_ptr = data_dict_get_child_list(element_ptr);
	while (child_list_ptr != NULL)
	{
		sum += 1;
		child_list_ptr = data_dict_get_next_child(child_list_ptr);
	}
	return sum;
}

/******************************************************************************
** 
**  Description:   Create output file name.  Replace trailing extension "pml" 
**                 with extension derived from the mode.  The input filename 
**		   should be checked for the existence a pml extension before
**		   this function is called.
**
******************************************************************************/
char * create_cpml_filename (char *pml, char *cpml, char* filetype)
{

    char *ptr = NULL;
    char *ext_ptr;

   	ptr = (char *)malloc(strlen(pml) + strlen(filetype) + 1);
    	strcpy(ptr, pml);
	ext_ptr = strrchr(ptr,'.');
        if (strcmp(ext_ptr,".pp") == 0) {
           *ext_ptr = '\0';
           ext_ptr = strrchr(ptr,'.');
	}
	ext_ptr++;
	*ext_ptr = '\0';
	if (strcmp(filetype,TEXT_MODE) == 0) {
	   strcat(ptr,"txt");
	} else {
	   strcat(ptr,filetype);
	}

#if DEBUG == 1
    printf ("\n\ncpml is %s\n", ptr);
#endif

    return ptr;
}

/******************************************************************************
**
**  Function/Method Name:  write_cpml_data
**  Precondition:	   Valid output information.
**  Postcondition:	   Output to the correct file.
**  Description:	   Write to either a text file or gdbm file, depending on mode.
**
******************************************************************************/
Boolean write_cpml_data(int line_num, char* data_str, char* mode, OUTPUT_STRUCT output)
{
	if (strcmp(mode,TEXT_MODE) == 0)
		/* print out line number for readability only */
		fprintf(output.fptr,"%d %s\n",line_num,data_str);
	else if (strcmp(mode,CPML_MODE) == 0)
		fprintf(output.fptr,"%d %s\n",line_num,data_str);
	else if (strcmp(mode,GDBM_MODE) == 0)
		store_gdbm_data(output.dbf, line_num, data_str);
	else {
		printf("Failed to write out to cpml (output).\n");
		return FALSE;
	}
	return TRUE;
}


/******************************************************************************
**
**  Function/Method Name: create_gdbm
**  Precondition:	  Output file name
**  Postcondition:	  handle to gdbm output file created.
**  Description:	  Create/Open output gdbm. If mode is "write" create/open
**			  gdbm with "write and create" mode.
**
******************************************************************************/
GDBM_FILE create_gdbm(char* filename, char* mode)
{
	GDBM_FILE dbf;

	if ( strcmp(mode, "write") == 0 )
		dbf = gdbm_open(filename, 0, GDBM_WRCREAT, 0644, 0);
	else
		dbf = gdbm_open(filename, 0, GDBM_READER, 0644, 0);

	if (dbf == (GDBM_FILE) NULL) {
		printf("FAILED to create/open GDBM.\n");
		return (GDBM_FILE) NULL;
	}

	return dbf;
}

/******************************************************************************
**
**  Function/Method Name: store_gdbm_data
**  Precondition:	  key and content data
**  Postcondition:	  key and content data saved to file.
**  Description:	  Store datainto gdbm.  Eack key and content is stored
**			  in to gdbm's datrum format. The size od each data is
**			  also into datum format.
**
******************************************************************************/
void store_gdbm_data(GDBM_FILE dbf, int key_data, char* content_data)
{
	datum key, content;
	char *content_ptr;
	int* key_ptr;

	key_ptr = (int*) malloc(sizeof(int));
	if (key_ptr == (int*) NULL)
		printf("Failed to allocate memory.\n");

	key_ptr = &key_data;
	key.dptr = (char*) key_ptr;
	key.dsize = sizeof(int);

	content_ptr = (char*) calloc ((unsigned)strlen (content_data)+1, sizeof(char));
	strcpy(content_ptr, content_data);
	content.dptr = content_ptr;
	content.dsize = strlen(content_ptr)+1;

	if(gdbm_store(dbf, key, content, GDBM_REPLACE) != 0)
		printf("failed to store data.\n");

	free (content_ptr);
}

/******************************************************************************
**
**  Function/Method Name: retrieve_gdbm_data
**  Precondition:  gdbm output data.
**  Postcondition: Single level content data retrieved and returned to
**                 the user.  If failed to retrieve data, it
**                 returns (char *) NULL.
**
******************************************************************************/
char *retrieve_gdbm_data (GDBM_FILE dbf, int key_data)
{
     datum   key, content;
     int    *key_ptr; 
     char *content_ptr;

     key_ptr = (int*) malloc (sizeof(int));
     if (key_ptr == (int *) NULL) {
        printf ("Failed to allocate memory.\n");
     }

     key_ptr = &key_data;

     key.dptr = (char*) key_ptr;
     key.dsize = sizeof(int);

     content = gdbm_fetch (dbf, key);
     if (content.dptr == NULL) {
        printf ("Failed to retrieve data.\n");
        return (char *) NULL;
     }
     content_ptr = (char *) calloc (content.dsize, sizeof (char));
     if (content_ptr == (char *) NULL) {
        printf ("Failed to allocate memory for retrieving data.\n");
     }
     strcpy (content_ptr, content.dptr);

/* bugbug 4/29/99.... free memory used by key and content???? */

     return content_ptr;

}

/******************************************************************************
**
**  Function/Method Name: test_retrieval
**  Precondtion:  gdbm output exist.
**  Postconditon: Data stored in the gdbm are tested.
**  Description:  Test function to test the data stored in gdbm file.
**
******************************************************************************/
void test_retrieval (OUTPUT_STRUCT output)
{
    char    *cpml_data=(char *) NULL;
    int i = 1;;

    if (output.dbf == (GDBM_FILE) NULL) {
        printf ("Can't test gdbm. GDBM is null.\n");
        return;
    }
    printf ("\n________________________________________________________\n");
    printf ("\nRetrieval test.\n");
    while((cpml_data = retrieve_gdbm_data (output.dbf, i)) != NULL) {
       printf ("retreived:\t%d %s\n", i,cpml_data);
       free (cpml_data);
       i++;
    }
    printf ("Failed at %d;either at end or error; compare with text\n", i);
    printf ("\n________________________________________________________\n");
}

/******************************************************************************
**
**    Function/Method Name: add_var
**    Precondition:  
**    Postcondition: 
**
**    Description:   This function adds a new variable name to var symbol table.
**
******************************************************************************/
void add_var(char* name, char* desc)
{
	VAR_STRUCT* new_var = (VAR_STRUCT*) malloc(sizeof(VAR_STRUCT));
	
	strcpy(new_var->var_name,name);
	strcpy(new_var->var_desc,desc);
	var_list[var_list_ptr] = *new_var;
	var_list_ptr += 1;
}

/******************************************************************************
**
**    Function/Method Name: check_for_var
**    Precondition:  
**    Postcondition: Return value is >= 0 if var is in table ,-1 otherwise
**
**    Description:   This function checks for a variable name in var symbol table.
**
******************************************************************************/
int check_for_var(char* name)
{
	int i = 0;
	while (i < var_list_ptr && strcmp(name,var_list[i].var_name) != 0 ) {
		i += 1;
	}
	if (i < var_list_ptr)
		return i;
	return -1;
}

/******************************************************************************
**
**    Function/Method Name: add_to_ready
**    Precondition:  
**    Postcondition: 
**
**    Description:   This function adds an action name to table.
**
******************************************************************************/
void add_to_ready(char* name)
{
	char* new_var = (char*) malloc(sizeof(char));
	
	strcpy(new_var,name);
	ready_list[ready_list_ptr] = new_var;
	ready_list_ptr += 1;
}

/******************************************************************************
**
**    Function/Method Name: check_for_ready
**    Precondition:  
**    Postcondition: Return value is >= 0 if var is in table ,-1 otherwise
**
**    Description:   This function checks for an action name in table.
**
******************************************************************************/
int check_for_ready(char* name)
{
	int i = 0;
	while (i < ready_list_ptr && strcmp(name,ready_list[i]) != 0 ) {
		i += 1;
	}
	if (i < ready_list_ptr)
		return i;
	return -1;
}
