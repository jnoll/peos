/*****************************************************************/
/* File:        vm.c
/* Author:      Crystal Zhu 
/* Date:        11/17/1999 
/* Description: Virtual machine. 
/*****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <pml_state.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "pmlevent.h"
#include "vm.h"

/* static variables local to this file so as to make local functions
   simple */


int PC = 0, SP = INT_MAX, A = 0; /* A is a register */
struct context_t context;   /* context of current process */

char* VM_PREF = "ENG"; /* standard prefix for all processes created in Rep.*/

char* VM_ATTR_PROC = "PROCESS_NAME";
char* VM_ATTR_PCTXT = "PROCESS_CONTEXT";
char* VM_ATTR_SP = "PROCESS_STACK";
char* VM_ATTR_SP_DEPTH = "PROCESS_STACK_DEPTH";
char *  COMP_DIR = "COMPILER_DIR";

char Buffer[1024];
char* exitStatus = Buffer;

char * nextOPCode(pml_obj_t proc, char*);
void push(pml_obj_t proc, int arg);
int pop(pml_obj_t proc);
int setState(pml_obj_t proc, char * act_name, int state);
int setWait(pml_obj_t proc, char * act_name, int wait);
int assert(pml_obj_t proc, char * act_name);
int storeContext(pml_obj_t proc);
int fetchContext(pml_obj_t proc);
void add_var(pml_obj_t proc, char* name, char* value);
char* dereference(pml_obj_t proc, char* string);

void  errExit(void )
{
	perror("ERROR: can not read. ");
}

char * getModel(char * processName)
{
    char * model = NULL;
    char * file = NULL;
    char * filend = NULL;
    char * path = getenv(COMP_DIR);
    char begin = '/';
    char end = '.';
    FILE* fd; /* file descriptor */

    model = (char *) malloc(256);

    if(model == NULL)
    {
       perror("NO MORE SPACE");
       return model;
    }
    if(processName == NULL)
    {
       perror("NO Process Name passed in.");
       free(model);
       return model;
    }
    else
    {
	file = strrchr(processName, begin);
        file = (file == NULL) ? processName : ++file;
	filend = strchr(file, end);
        filend = (filend == NULL) ? processName+strlen(processName) : filend;
        if(file >= filend)
        {
            perror("Could not find the model name from processName");
	    free(model);
	    return model;
        } 
    }
    if(path && strlen(path))
    {
       strcpy(model, path);
       if(path[strlen(path) -1] != '/')
	   strcat(model, "/");
       strncat(model, file,  filend - file);
       strcat(model, ".txt");
    }
    else
    {
       strcpy(model, "./");
       strncat(model, file,  filend - file);
       strcat(model, ".txt");
    }

    if ((fd=(FILE*)fopen(model, "r")) == NULL) {
      return NULL;
    } else {
      fclose(fd);
    }
    return model;
}

int execute(pml_obj_t process)
{
    char op[256];
    char outsidechildName[256];            /* current child name */
    char outsideName[256];
    int ret = 0;
    int cond = 1; /* condition for the following while loop */
    int numOfChild = 0;
    int pExit = 0;

    fetchContext(process);
    
    while (cond) {
        /* fetch next opcode */
        nextOPCode(process,op);

        /* interpret opcode */
        if (strncmp(op, "start", 5) == 0) {
        }
        else if (strncmp(op, "end", 3) == 0) {
            PC--;
            cond = 0;
	    pExit = 1;
	    printf("e 0\n");
	    fflush(stdout);
        }
        else if (strncmp(op, "push", 4) == 0) {
            int arg;
            sscanf(op+4, "%d", &arg);
            push(process,arg);
        }
        else if (strncmp(op, "pop", 3) == 0) {
		char* token;

            	A = pop(process);
		token = strtok(op+3," \n");		
		if (token != NULL) {
			add_var(process,token,(char*)A);
		}
        }
        else if (strncmp(op, "goto", 4) == 0) {
            PC = atoi(op+4);
        } 
        else if (strncmp(op, "jump", 4) == 0) {
            int i;
            char * token;
            A = pop(process);
            token = strtok(op, " \n\t");
            for (i = 0; i <= A; i++) {
                token = strtok(0, " \n\t");
            }
            PC = atoi(token);
        } 
        else if (strncmp(op, "jzero", 5) == 0) {
            A = pop(process);
            if (A == 0) {
                char * token;
                token = strtok(op+5, " \n\t");
                PC = atoi(token);
            }
            push(process,A);
        }
        else if (strncmp(op, "incr", 4) == 0) {
            A = pop(process);
            A++;
            push(process,A);
        }
        else if (strncmp(op, "decr", 4) == 0) {
            A = pop(process);
            A--;
            push(process,A);
        }
        else if (strncmp(op, "call set ready", 14) == 0) {
            char * token;
            token = strtok(op+14, " \n\t");
            while (token) {
                ret = setState(process,token, READY);
                token = strtok(0, " \n\t");
            }
            push(process,-1);
        }
        else if (strncmp(op, "call set none", 13) == 0) {
            char * token;
            token = strtok(op+13, " \n\t");
            while (token) {
                ret = setState(process,token, NONE);
                token = strtok(0, " \n\t");
            }
            push(process,-1);
        }
        else if (strncmp(op, "call wait done", 14) == 0) {
            char * token;
            int order = 0;
	    char waitList [256];

	    strcpy(waitList, op+15);
	    waitList[strlen(waitList)] = '\0';
            ret = -1;
            token = strtok(op+14, " \n\t");
            while (token && ret == -1) {
                order++;
                ret = setWait(process,token, DONE);
                token = strtok(0, " \n\t");
            }
            if (ret == -1) {
                cond = 0;
                PC--;
		printf("a %d %s\n", DONE, waitList);
		fflush(stdout);
            } else {
                push(process,order);
            }
        }
        else if (strncmp(op, "call wait active", 16) == 0) {
            char * token;
            int order = 0;
	    char waitList [256];

	    strcpy(waitList, op+17);
	    waitList[strlen(waitList)] = '\0';
	    token = strtok(op+16, " \n\t");
            ret = -1;
            while (token && ret == -1) {
                order++;
                ret = setWait(process,token, RUN);
                
                token = strtok(0, " \n\t");
            }
            if (ret == -1) {
                cond = 0;
                PC--;
		printf("a %d %s\n", RUN, waitList);
		fflush(stdout);
            } else {
                push(process,order);
            }
        }
        else if (strncmp(op, "call select", 11) == 0) {
		int i,n;
		char* token;
		char* strptr;
	    	char query [256];
	    	char query_new [256];
		
		token = strtok(op+11, " \n&");
		strcpy(query,token);
		while (token) {
			token = strtok(NULL, " \n&");
			strcat(query," ");
			if (token)
				strcat(query,token);
		}
		// Look for variables to dereference
		i = 0;
		strptr = strchr(&query[i],'$');
		if (strptr == NULL)
			strcpy(query_new,query);
		while (strptr) {
			n = strcspn(&query[i],"$");
			strncpy(query_new,&query[i],n);
			if (n < strlen(&query[i]))
				strcat(query_new,dereference(process,&query[i+n+1]));
			i += n + 1 + strcspn(&query[i+n+1]," &");
			strptr = strchr(&query[i],'$');
		}
		printf("d %s\n",query_new);
		fflush(stdout);
		cond = 0;
	}
        else if (strncmp(op, "call assert", 11) == 0) {
            char * token;
            token = strtok(op+11, " \n\t");
            ret = 1;
            while (token && ret == 1) {
                ret = assert(process, token);
                token = strtok(0, " \n\t");
            }
//            if (ret == 0) {
//                cond = 0;
//                PC--;
//            }
//            else {
                push(process,ret);
//            }
        }
        else if (strncmp(op, "call fork", 9) == 0) {
            int start;
            char me[256];
	    char childName[256];

            push(process,-1);
            storeContext(process);
	    outsidechildName[0] = '\0';
	    ret = pml_read_attribute(process,
				     VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
				     me, sizeof(me));
	    if( ret == 0 )
		errExit();
	    strcpy(outsideName,me+strlen(VM_PREF));
            sscanf(op+9, "%d", &start);
            // create child process name
	    sprintf(outsidechildName, "%s.%03d", outsideName, ++numOfChild);
	    printf("b %s %d\n",  outsidechildName, start);
	    fflush(stdout);
            fetchContext(process);
        }
        else if (strncmp(op, "call join", 9) == 0) {
	    int i;
            push(process,-1);
            cond = 0;
	    if(numOfChild) // first time run, give all the children's name to OS
	    {
		for( i =0; i < numOfChild; i++)
		    printf("c %s.%03d\n",  outsideName, i+1);
	    }
	    else
		printf("c \n");
	    fflush(stdout);
        }
        else if (strncmp(op, "call exit", 9) == 0) {
            push(process,-1);
	    printf("e 0\n");
	    fflush(stdout);
	    strcpy(exitStatus, "Execute successful\n");
            cond = 0;
	    pExit = 1;
        }
        else if (strncmp(op, "call err", 8) == 0) {
	    printf("e 1\n");
	    fflush(stdout);
	    strcpy(exitStatus, "Execute exit on ERROR\n");
            cond = 0;
	    pExit = 1;
        }
        /* else if (strncmp(op, "call", 4) == 0) {
            push(process,-1);
            cond = 0;
        }*/
    };
    storeContext(process);
    if(pExit == 1)
	pml_delete_object(process);
    pml_pack_objects();
    return 0;
} /* execute */

char * nextOPCode(pml_obj_t proc, char * op)
{
    FILE * fp = 0;
    char * fileName = 0;
    char * p = 0;
    char   path[256];
    int    pc = 0;
    int    ret;
 
    ret = pml_read_attribute(proc, 
		       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
		       path, sizeof(path));
       if( ret == 0 )
	        errExit();
    fileName = getModel(path+strlen(VM_PREF));
    /* fetch next opcode */
    fp = fopen(fileName, "r");
    while(fgets(path, 256, fp))
    {
       sscanf(path, "%d", &pc);
       if( pc == PC)
       {
	  p = strchr(path, ' ');
	  while(isspace(*p)) 
   	    ++p;
	  strcpy(op, p);
          if(op[strlen(op)-1] == '\n')
	      op[strlen(op)-1] = '\0';
	  break;
       }
    }

    fclose(fp);
    sprintf(path ,"%d %s\n", PC, op);
    fprintf(stderr,path); fflush(stderr);
    PC++;
    free(fileName);
    return op;
} /* nextOPCode */

void push(pml_obj_t proc, int arg)
{
    int stack[MAX_STACK_SIZE];
    int depth;
    char path[256];
    int ret;
    ret = pml_read_attribute(proc, 
		       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
		       path, sizeof(path));
       if( ret == 0 )
	        errExit();
    ret = pml_read_attribute(proc,
		       VM_ATTR_SP, strlen(VM_ATTR_SP)+1,
		       stack, sizeof(stack));
       if( ret == 0 )
	        errExit();
    ret = pml_read_attribute(proc,
		       VM_ATTR_SP_DEPTH, strlen(VM_ATTR_SP_DEPTH)+1,
		       &depth, sizeof(depth));
       if( ret == 0 )
	        errExit();
    stack[depth] = arg;
    depth++;
    pml_write_attribute(proc,
		       VM_ATTR_SP, strlen(VM_ATTR_SP)+1,
		       stack, sizeof(stack));
    ret = pml_write_attribute(proc,
		       VM_ATTR_SP_DEPTH, strlen(VM_ATTR_SP_DEPTH)+1,
		       &depth, sizeof(int));
    SP--;
} /* push */

int pop(pml_obj_t proc)
{
    int stack[MAX_STACK_SIZE];
    int depth;
    int ret;
    char path[256];  // For Debugging
    ret = pml_read_attribute(proc, 
		       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
		       path, sizeof(path));
    if( ret == 0 )
	       errExit();

    if (SP == INT_MAX) {
        perror("stack underflow");
    }

    ret = pml_read_attribute(proc,
		       VM_ATTR_SP, strlen(VM_ATTR_SP)+1,
		       stack, sizeof(stack));
    if( ret == 0 )
	     errExit();
    ret = pml_read_attribute(proc,
		       VM_ATTR_SP_DEPTH, strlen(VM_ATTR_SP_DEPTH)+1,
		       &depth, sizeof(depth));
    if( ret == 0 )
	      errExit();

    if (depth <= 0) {
        perror("stack underflow");
    }

    depth--;
    ret = pml_write_attribute(proc,
		       VM_ATTR_SP_DEPTH, strlen(VM_ATTR_SP_DEPTH)+1,
			&depth, sizeof(depth));
    SP++;
    return stack[depth];
} /* pop */

int setState(pml_obj_t proc,char * act_name, int state)
{
    int ret = -1;
    struct act_t act;

    ret = pml_read_attribute(proc,
		       act_name, strlen(act_name)+1,
		       &act, sizeof(struct act_t));
    if(ret < 0)
    {
        act.PC = 139;
        act.state = state;
        act.wait = NEW;
    }

    if (state == READY && act.state == ABORTED ) {
        return 1;
    }
    if (state == DONE && act.state != RUN) {
        return 1;
    }

    act.state = state;
    if( ret >=0 )     // wait is unset by call set
	act.wait = NONE;

    ret = pml_write_attribute(proc,
		       act_name, strlen(act_name)+1,
		       &act, sizeof(struct act_t));
    if (ret < 0) {
        fprintf(stderr,"gdbm_store");
    }

    return ret;
} /* setState */

int setWait(pml_obj_t proc,char * act_name, int wait)
{
    int ret = -1;
    int dbentry = -1;
    struct act_t act;

    dbentry = pml_read_attribute(proc,
				 act_name, strlen(act_name)+1,
				 &act, sizeof(struct act_t));
    if( dbentry == 0 )
	errExit();
    if (dbentry < 0) {
        fprintf(stderr, "create a dummy action: %s   \n", act_name);
        act.PC = 139;
        act.state = NEW;
        act.wait = wait;
    }

    if (act.state == wait)
    {
        ret = 0;
    }
    else
    {
	act.wait = wait;
    }

    if (ret != 0 && pml_write_attribute(proc, act_name, strlen(act_name)+1,
		            &act, sizeof(struct act_t)) < 0)
        fprintf(stderr,"pml_store failed.");

    return ret;
} /* setWait */


int assert(pml_obj_t proc, char * act_name)
{
    int ret = 0;
    struct act_t act;
    ret = pml_read_attribute(proc,
			     act_name, strlen(act_name)+1,
			     &act, sizeof(act));
    if( ret == 0 )  // fail to read
	errExit();
    if(ret > 0)
	ret = ( act.state == DONE );  // state set to 1 when DONE, otherwise 0
    ret =  ( ret <=0 ) ? 0 : 1;
    if(!ret)
	fprintf(stderr, "Assert on action %s failed.\n", act_name);
    return ret;
} /* assert */

int fetchContext(pml_obj_t proc)
{
    /* fetch context*/
    int ret;
    ret = pml_read_attribute(proc, 
		       VM_ATTR_PCTXT, strlen(VM_ATTR_PCTXT)+1,
		       &context, sizeof(struct context_t));
       if( ret == 0 )
	        errExit();
    /* store context */
    PC = context.PC;
    SP = context.SP;
    return 0;
} /* fetchContext */

int storeContext(pml_obj_t proc)
{

    /* store context */
    context.PC = PC;
    context.SP = SP;
    pml_write_attribute(proc, 
		       VM_ATTR_PCTXT, strlen(VM_ATTR_PCTXT)+1,
		       &context, sizeof(struct context_t));
    return 0;
} /* storeContext */

char* dereference(pml_obj_t proc, char* string)
{
	int ret;
	char* value_ptr = NULL;
	char value[64];
	char* temp;
	temp = strtok(string," \t\n");
	ret = pml_read_attribute(proc,
                       temp, strlen(temp)+1,
                       value, sizeof(value));
        if( ret == 0 )
                errExit();
	value_ptr = value;
	return value_ptr;
} /* dereference */

void add_var(pml_obj_t proc, char* name, char* value)
{
  int ret;
  
  ret = pml_write_attribute(proc,
			    name, strlen(name)+1,
			    value, strlen(value)+1);
  if( ret == 0 )
    errExit();
} /* add_var */
