/******************************************************************************
** File Information: This is the PML file parser
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "par_dd.h"

/******************************************************************************
**    Function/Method Name:  next_token 
**    Precondition:
**    Postcondition:        
**    Description:           get next token 
******************************************************************************/

int next_token()
{
	return look_a_head=lexical_analyzer();
}

/******************************************************************************
**    Function/Method Name:  push_back 
**    Precondition:
**    Postcondition:        
**    Description:            push back last token 
******************************************************************************/

void push_back()
{
	fseek(in_fptr,-(long)strlen(token_buffer),SEEK_CUR);
}

/******************************************************************************
**    Function/Method Name:  clear_token_buffer 
**    Precondition:
**    Postcondition:        
**    Description:           clear token buffer 
******************************************************************************/

void clear_token_buffer()
{
	memset(token_buffer,0,TOKEN_LEN);
}

/******************************************************************************
**    Function/Method Name:  lexical_analyzer 
**    Precondition:
**    Postcondition:        
**    Description:           Reads the pml source code file character at 
**			     at time to pull out tokens, literals, variables,
**			     etc....Function for lexical analyzer 
******************************************************************************/

int lexical_analyzer()
{
	char *module = "lexical_analyzer";
	char msg[MSG_SIZE];
   int in_char,in_char2;
   
   clear_token_buffer();
   while((in_char=fgetc(in_fptr))!=EOF) {
      if(in_char==' '||in_char=='\t'||in_char=='\r')
         continue;      /* skip white space */
      
      /* Process comments */
      else if(in_char=='/') {
		if((in_char2=fgetc(in_fptr))=='*') 
		  do {
		    in_char2=fgetc(in_fptr);
                    if(in_char2=='*'&&(in_char2=fgetc(in_fptr))=='/')
                       break;
                  } while(in_char2!=EOF);

		else if (in_char2=='/')
		  do {
		    in_char2=fgetc(in_fptr);
		  } while(in_char2!='\n');

                else {
		   sprintf(token_buffer,"%c",in_char);	
		   sprintf(msg,
		       "Line %d: Unexpected character in comments: %c Ascii %d",
	               line_no, in_char,in_char);
		   pmlprint(ERROR,PMLSYN,module,msg);
                   clear_token_buffer();
                   ungetc(in_char2,in_fptr);
                }
		continue;	
      } 

      else if(isalpha(in_char)) {
           int bb=0;
           while(isalnum(in_char)||in_char=='_') {
               token_buffer[bb]=tolower(in_char);
               in_char=fgetc(in_fptr);   bb++;
		sprintf(msg,"Line %d: Exceeded max token length: %d",
		    line_no,IN_BUF_SIZE);
               if(bb>=IN_BUF_SIZE) pmlprint(ERROR,PMLSYN,module,msg); 
           }
           token_buffer[bb]=EOS;
           ungetc(in_char,in_fptr);
           return check_resword();

      } else if(in_char=='{') {
         sprintf(token_buffer,"%c",in_char);
         return OP_BRC;
      }
      else if(in_char=='}') {
         sprintf(token_buffer,"%c",in_char);
         return CL_BRC;
      } else if(in_char=='=') {
	  	 in_char2=fgetc(in_fptr);
         sprintf(token_buffer,"%c%c",in_char,in_char2);
		 if(in_char2=='=')
	 		return EQUAL;
      } else if(in_char=='&') {
	  	 in_char2=fgetc(in_fptr);
         sprintf(token_buffer,"%c%c",in_char,in_char2);
	 	 if(in_char2=='&')
	 		return AND;
      } else if(in_char=='.') {
         sprintf(token_buffer,"%c",in_char);
		 return PERIOD;
      } else if(in_char=='\"') {
              int bb=0;
              in_char=fgetc(in_fptr);

	      while(((in_char == '\t') || 
		    ((in_char >= 32) && (in_char <= 126))) &&
		     (in_char != '\"')) {
		 if(in_char == '\\') {
		    in_char2 = fgetc(in_fptr);
	            if(in_char2 != '\n') {
		       ungetc(in_char2,in_fptr);
		    } else {
		       in_char = fgetc(in_fptr);
		       continue;
	            }
	  	 }
                 token_buffer[bb]=in_char;
		 in_char = fgetc(in_fptr);
		 bb++;
	 	 sprintf(msg,"Line %d: Exceeded max token length: %d",
		    line_no,IN_BUF_SIZE);
                 if(bb>=IN_BUF_SIZE) pmlprint(ERROR,PMLSYN,module,msg); 
	      }

              token_buffer[bb]=EOS;
              if(in_char=='\"')
                  return STRING;
              else {
		 sprintf(msg,"Line %d: Unterminated constant, expect \"",
		     line_no);	
		 pmlprint(ERROR,PMLSYN,module,msg);
              }
      }
      else if(in_char=='\n')
         line_no=line_no+1;
      else {
	 sprintf(msg,"Line %d: Unexpected character-%c, Ascii %d",
	    line_no,in_char,in_char);
	 pmlprint(ERROR,PMLSYN,module,msg);
      }
   } /* end of 'while' cycle */
   return 0;
}

/******************************************************************************
**    Function/Method Name:  check_resword 
**    Precondition:
**    Postcondition:        
**    Description:           check for whether reserved word or identifier 
******************************************************************************/
		
int check_resword()
{
	struct resword *rw_ptr;
	token cc=ID;
	for(rw_ptr=&rw[PROCESS]; rw_ptr->str_ptr; ++rw_ptr) {
		if(strcmp(token_buffer,rw_ptr->str_ptr)==0) {
			cc=rw_ptr->code;
			break;
		}
	}
	return cc;
}

/******************************************************************************
**    Function/Method Name:  match_token 
**    Precondition:
**    Postcondition:        
**    Description:           Expected next token is passed in.  The next
**			     token is extracted from the pml source code
**			     file via the lexical analyzer.  If the two
**			     do not match a syntax error is reported
**	 
******************************************************************************/

void match_token(int tt)
{
	char msg[MSG_SIZE];
	look_a_head=lexical_analyzer();
	if(look_a_head!=tt) {
		sprintf(msg,"Line %d: Expected %s,\n\tRetrieved %s",line_no,
		    rw[tt].str_ptr,token_buffer);
		pmlprint(ERROR,PMLSYN,"match_token",msg);
	}
}

/******************************************************************************
**    Function/Method Name:  parse_pml
**    Precondition:
**    Postcondition:        
**    Description:           parse the given pml file and populate the data
**                           dictionary 
******************************************************************************/

int parse_pml(char* pml_filename, data_dictionary_struct* dictionary_ptr,
    Boolean debug)
{
	char *module = "parse_pml";
	char parent_name[TOKEN_LEN];
	char msg[MSG_SIZE];
	line_no=1;
	error_flag=TRUE; 
	in_fptr = fopen(pml_filename, "r");
	match_token(PROCESS);
	match_token(ID);

	/* 
	 * Set the number of pml types we have found
	 * without names to 0.  The unnamed "types" will be assigned a
 	 * unique name with the current count
	 */
	num_unnamed_types = 0;

	/* 
	 * Add the root to the data dictionary -- 
	 * The root is initialized to NULL so to prevent segmentation 
	 * faults, this must always be done first.
	 */
	data_dict_add_root(dictionary_ptr, "", token_buffer, "process", "");
	strcpy(parent_name, token_buffer);
	match_token(OP_BRC);
	primlist_func(dictionary_ptr, parent_name);
	match_token(CL_BRC);
	fclose(in_fptr);
  
	if (error_flag == TRUE) {
		error_flag = add_level_info_to_data_dictionary(dictionary_ptr);
		if (error_flag == FALSE) {
			sprintf(msg,
			    "Unable to parse the following PML file: %s",
			    pml_filename);
			pmlprint(ERROR,PMLDD,module,msg);
		}
	}
	if (debug == TRUE) {
		if (error_flag == TRUE) {
			data_dict_print_to_screen(dictionary_ptr);
		}
	}
	return error_flag;
}

/******************************************************************************
**    Function/Method Name:  primlist_func 
**    Precondition:
**    Postcondition:        
**    Description:           A reserved word is passed in.  Parse the expected
**			     next tokens from the pml source code and add the
**			     reserved word and, if applicable, attributes to
**			     the data dictionary. 
******************************************************************************/

int primlist_func(data_dictionary_struct *dictionary_ptr, char *parent_name)
{
	token tok;
	char *module = "primlist_func";
	char msg[MSG_SIZE];
	char type[TOKEN_LEN],mode[TOKEN_LEN],name[TOKEN_LEN];
	char prev_parent_name[TOKEN_LEN];

	/* store a copy of the parents name */
	strcpy(prev_parent_name, parent_name);

	while(TRUE) {
		switch(next_token()) {
			case SEQUENCE:
			case TASK:
			case BRANCH:
			case SELECTION:
			case ITERATION:
				strcpy(type,token_buffer);
				tok=next_token();
				if(tok==ID) 
					look_a_head=ID;
				else {
					 push_back();
					 clear_token_buffer();
				 }
     
				/* 
			 	 * If a name is not specified then create a
				 * unique default one.
				 */
				if (strlen(token_buffer) == 0) {
					sprintf(token_buffer, "name_%d",
					     num_unnamed_types);
					num_unnamed_types++;
				}
		
				data_dict_add_entry(dictionary_ptr,
				    parent_name, "",token_buffer,type,"");
				strcpy(parent_name, token_buffer);
				match_token(OP_BRC);
				primlist_func(dictionary_ptr, parent_name);
				match_token(CL_BRC);
       	  
				/* reset the parent's name to previous value */   
				strcpy(parent_name, prev_parent_name);
	
				break;
			case ID:
				push_back(); 
				clear_token_buffer();
				strcpy(token_buffer,"action");
			case ACTION:
				strcpy(type,token_buffer);
				match_token(ID);
				strcpy(name,token_buffer);
				tok=next_token();
				if(tok==MANUAL) 
					look_a_head=MANUAL;
				else if(tok==EXECUTABLE) 
					look_a_head=EXECUTABLE;
				else { 
					push_back();
				 	clear_token_buffer(); }
	
				/* 
				 * If a mode is not specified for an action, 
				 * it defaults to "manual
				 */
				if (strlen(token_buffer) == 0) { 
					strcpy(token_buffer, "manual");
				}
	
				data_dict_add_entry(dictionary_ptr,parent_name,
				    "",name,type,token_buffer);
				strcpy(parent_name,name);
				match_token(OP_BRC);
				speclist_func(dictionary_ptr, parent_name);
				match_token(CL_BRC);
 	 
				/* reset the parent's name to previous value */
				strcpy(parent_name, prev_parent_name);
	
				break;
			case PROCESS:
				sprintf(msg,
				    "Line %d: PROCESS definition repeated",
				    line_no);
				pmlprint(ERROR,PMLSYN,module,msg);
			default:
				push_back(); 
				return;
		}
	}
}

/******************************************************************************
**    Function/Method Name:  speclist_func 
**    Precondition:
**    Postcondition:        
**    Description:           speclist function 
******************************************************************************/

int speclist_func(data_dictionary_struct* dictionary_ptr, char *action_name)
{
	int tok;
	char *module = "speclist_func";
	char msg[MSG_SIZE];
	char desc_type[TOKEN_LEN],desc_name[TOKEN_LEN];
	while(TRUE) {
		switch(next_token()) {
			case SCRIPT: 
			case TOOL:
			case AGENT:
				strcpy(desc_type,token_buffer);
				match_token(OP_BRC);
				match_token(STRING);
				strcpy(desc_name,token_buffer);

				data_dict_action_add_desc(dictionary_ptr,
				    action_name,
				desc_type, desc_name);
				match_token(CL_BRC);
				break;
			case REQUIRES:
			case PROVIDES:
				strcpy(desc_type,token_buffer);
				strcpy(desc_name,"\0");
				match_token(OP_BRC);
				tok = next_token();
				while (tok!=CL_BRC) {
					if(tok==ID) {
						strcat(desc_name,token_buffer);
						tok=next_token();
					if(tok==EQUAL)
						strcat(desc_name,token_buffer);
					if(tok==PERIOD) {
						strcat(desc_name,token_buffer);
						match_token(ID);
						strcat(desc_name,token_buffer);
						tok=next_token();
						strcat(desc_name,token_buffer);
					}
					if(tok!=EQUAL)
						continue;
					tok=next_token();
					strcat(desc_name,token_buffer);
					if(tok==PERIOD) {
						match_token(ID);
						strcat(desc_name,token_buffer);
					}
					tok = next_token();
					} else if (tok==AND) {
						strcat(desc_name,token_buffer);
						tok = next_token();
					} else if (tok=STRING) {
						strcat(desc_name,token_buffer);
						tok = next_token();
						old_format = TRUE;
					} else {
						sprintf(msg,
						    "Line %d: Invalid require",
						    line_no);
						sprintf(msg,
						    "%s or provide clause",msg);
						pmlprint(ERROR,PMLSYN,
						module,msg);
					}
				}	
				data_dict_action_add_desc(dictionary_ptr,
				    action_name,desc_type,desc_name);
				break;
			case INPUT:
			case OUTPUT:
			case CREATES:
				strcpy(desc_type,token_buffer);
				match_token(OP_BRC);
				match_token(STRING);
				strcpy(desc_name,token_buffer);
		
				data_dict_action_add_desc(dictionary_ptr,
				    action_name,
				desc_type, desc_name);
				match_token(CL_BRC);
				break;
			default: push_back(); return;
		}
	}
}
