/******************************************************************************
** File Information: This is the PML file parser
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/* include parser declarations */
#include "par_dd.h"


/******************************************************************************
**    Function/Method Name:   syntax_error
**    Precondition:
**    Postcondition:        
**    Description:            syntax_error 
******************************************************************************/
void syntax_error(char *msg)
{
   printf("\nLine %d: %s\n", line_no, msg);
   exit(-1);
  // error_flag=FALSE;
}


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
**    Description:           function for lexical analyzer 
******************************************************************************/
/* function for lexical analyzer */
int lexical_analyzer()
{
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
		   syntax_error(strcat(token_buffer,error_message[3]));
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
               if(bb>=IN_BUF_SIZE) syntax_error(error_message[1]);
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
      }
      else if(in_char=='\"') {
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
                 if(bb>=IN_BUF_SIZE) syntax_error(error_message[1]);
	      }

              token_buffer[bb]=EOS;
              if(in_char=='\"')
                  return STRING;
              else {
                 sprintf(token_buffer,"%c",in_char);
                 syntax_error(strcat(token_buffer,error_message[2]));
              }
      }
      else if(in_char=='\n')
         line_no=line_no+1;
      else {
         sprintf(token_buffer,"%c",in_char);
         syntax_error(strcat(token_buffer,error_message[2]));
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
   for(rw_ptr=rw; rw_ptr->str_ptr; ++rw_ptr) {
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
**    Description:           match_token 
******************************************************************************/
void match_token(int tt)
{
   look_a_head=lexical_analyzer();
   if(look_a_head!=tt)
      syntax_error(strcat(token_buffer,error_message[2]));
}


/******************************************************************************
**    Function/Method Name:  parse_pml
**    Precondition:
**    Postcondition:        
**    Description:           parse the given pml file and populate the data
**                           dictionary 
******************************************************************************/
int parse_pml(char* pml_filename, data_dictionary_struct* dictionary_ptr)
{
   char parent_name[TOKEN_LEN];
   line_no=1;
   error_flag=TRUE; 
   in_fptr = fopen(pml_filename, "r");
   match_token(PROCESS);
   match_token(ID);

   /* set the number of pml types we have found without names to 0 */
   num_unnamed_types = 0;

   /* add the root to the data dictionary -- this must always be done first */
   data_dict_add_root(dictionary_ptr, "", token_buffer, "process", "");
   strcpy(parent_name, token_buffer);
   match_token(OP_BRC);
   primlist_func(dictionary_ptr, parent_name);
   match_token(CL_BRC);
   fclose(in_fptr);
  
   if (error_flag == TRUE) {
      error_flag = add_level_info_to_data_dictionary(dictionary_ptr);
      if (error_flag == FALSE) {
         printf("\nERROR:[par_fun]-Unable to parse ");
         printf("the following PML file: %s\n", pml_filename);
      }
   }
   return error_flag;
}


/******************************************************************************
**    Function/Method Name:  primlist_func 
**    Precondition:
**    Postcondition:        
**    Description:           primlist function
******************************************************************************/
int primlist_func(data_dictionary_struct *dictionary_ptr, char *parent_name)
{
   token tok;
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
         if(tok==ID) look_a_head=ID;
         else { push_back(); clear_token_buffer(); }
        
         /* if a name is not specified then create a unique default one */
         if (strlen(token_buffer) == 0)
         {
            sprintf(token_buffer, "name_%d", num_unnamed_types);
            num_unnamed_types++;
         }

         data_dict_add_entry(dictionary_ptr, parent_name, "",
                                       token_buffer, type, "");
         strcpy(parent_name, token_buffer);
         match_token(OP_BRC);
         primlist_func(dictionary_ptr, parent_name);
         match_token(CL_BRC);
         
         /* reset the parent's name to previous value */   
         strcpy(parent_name, prev_parent_name);

         break;
      case ACTION:
         strcpy(type,token_buffer);
         match_token(ID);
         strcpy(name,token_buffer);
         tok=next_token();
         if(tok==MANUAL) look_a_head=MANUAL;
         else
            if(tok==EXECUTABLE) look_a_head=EXECUTABLE;

            else { push_back(); clear_token_buffer(); }

         /* if a mode is not specified for an action, it defaults to "manual */
         if (strlen(token_buffer) == 0)
         { 
            strcpy(token_buffer, "manual");
         }

         data_dict_add_entry(dictionary_ptr,parent_name,"",name,
                     type, token_buffer);
         strcpy(parent_name,name);
         match_token(OP_BRC);
         speclist_func(dictionary_ptr, parent_name);
         match_token(CL_BRC);
  
         /* reset the parent's name to previous value */
         strcpy(parent_name, prev_parent_name);

         break;
      default: push_back(); return;
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

            data_dict_action_add_desc(dictionary_ptr, action_name,
                                       desc_type, desc_name);
            match_token(CL_BRC);
            break;
         case REQUIRES:
         case PROVIDES:
         case INPUT:
         case OUTPUT:
         case CREATES:
            strcpy(desc_type,token_buffer);
            match_token(OP_BRC);
            match_token(STRING);
            strcpy(desc_name,token_buffer);

            data_dict_action_add_desc(dictionary_ptr, action_name,
                                       desc_type, desc_name);
            match_token(CL_BRC);
            break;
         default: push_back(); return;
      }
   }
}
