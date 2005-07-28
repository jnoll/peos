
#define YYDEBUG

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef PALM
#include "tclinterp.h"
#include <unistd.h>
#else
#include <PalmOS.h>
#endif

#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"

/* begin koranin */
#include <tcl.h>
#define MAX_LINE 40
/* return a string with trimed leading and/or trailing white space characters */
char* trim(char* string) {
    int i;
    while (string != '\0' && isspace(*string)) {
        ++string;
    }

    if (string == NULL)
        return NULL;

    // trim right white spaces
    for (i = strlen(string) - 1; isspace(*(string + i)); i--)
        *(string + i) = '\0';

    //printf("trim = \"%s\"\n", string);
    return string;
}

char* subs__res_var(char* var_file, char* res_value)
{
    FILE* stream;
    char* var_name;
    char* var_value;
    char line[MAX_LINE];
    Tcl_Interp* interp;
    char* action;
    char* stmt;
    int first = 1;
    char* result;
    //int status;

    action=(char*)malloc(500);
    stmt=(char*)malloc(100);
    
    if ((stream=fopen(var_file, "r")) == NULL) {
        printf("%s file doesn't exists\n", var_file);
        exit(1);
    }

    printf("\"%s\" file opened\n", var_file);

    while (fgets(line, MAX_LINE, stream) != NULL) {

        var_name = trim(strtok(line, ":"));
        var_value = trim(strtok(NULL, ":"));
        sprintf(stmt, "set %s \"%s\"; ", var_name, var_value);

	printf("stmt = %s\n", stmt);

        if (first)
        {
            action = strcpy(action, stmt);
            first = 0;
        }
        else
            action = strcat(action, stmt);

    }
    fclose(stream);

    sprintf(stmt, "set result \"%s\"", res_value);
    action = strcat(action, stmt);
    printf("action = %s\n", action);
    interp = Tcl_CreateInterp();
    Tcl_Eval(interp, action);
    result = (char*)malloc(strlen(interp->result) + 1);
    strcpy(result, interp->result);
    Tcl_DeleteInterp(interp);
    printf("RESOURCE VALUE = <%s>\n", result);
    return result;
}

/* set resource values from the resource file (xxx.res) */
void bind_resources_value(int pid, char* res_file, char* var_file) {

    FILE* stream;
    char* res_name;
    char* res_value;
    //int i;
    char line[MAX_LINE];
    if ((stream = fopen(res_file,"r")) == NULL) {
        printf("%s file doesn't exists\n", res_file);
        exit(1);
    }
    
    if(update_state() < 0) {
        fprintf(stderr, "Could not update process state\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE, stream) != NULL) {
        res_name = trim(strtok(line, ":"));
        res_value = trim(strtok(NULL, ":"));

        if (var_file != NULL)
            res_value = subs__res_var(var_file, res_value);

        if (peos_set_resource_binding(pid, res_name, res_value) < 0) {    //see events.c
            fprintf(stderr, "Could not bind resources");
            exit(EXIT_FAILURE);
        }
    }
    fclose(stream);
}

/*int create_process_ret_pid(char *model)
{
    int pid;
    int i;
    int num_resources;
    peos_resource_t *resources;

    resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);    //see events.c

    if (resources == NULL) {
        printf("error getting resources\n");
	return -1;
    }
    
    for(i = 0; i < num_resources; i++) {
	strcpy(resources[i].value, "$$");
    }
    
    printf("Executing %s:\n", model);
    
    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	printf("couldn't create process\n");
	return -1;
    } 
    else {
	printf("Created pid = %d\n", pid);
	return pid;
    }
}*/

/* end koranin */

int create_process(char *model)
{
    int pid;
    int i;
    int num_resources;
    peos_resource_t *resources;

    resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);    //see events.c

    if (resources == NULL) {
        printf("error getting resources\n");
	return -1;
    }
    
    for(i = 0; i < num_resources; i++) {
	strcpy(resources[i].value, "$$");
    }
    
    printf("Executing %s:\n", model);
    
    if ((pid = peos_run(model,resources,num_resources)) < 0) {    //see events.c
	printf("couldn't create process\n");
	return -1;
    } 
    else {
	printf("Created pid = %d\n", pid);
	return 1;
    }
}


int notify(int pid, char *action, char *event)
{
    vm_exit_code status;

    if(strcmp(event, "start") == 0) {
        printf("Performing action %s\n", action);
        if ((status = peos_notify(pid, action, PEOS_EVENT_START)) == VM_ERROR 
	|| (status == VM_INTERNAL_ERROR)) {
	    printf("process executed an illegal instruction and has been terminated\n");
	    return -1;
        }
	else return 1;
    }	

    if(strcmp(event, "finish") == 0) {
        printf("Finishing action %s\n", action);
        if ((status = peos_notify(pid, action,PEOS_EVENT_FINISH)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	    printf("process executed an illegal instruction and has been terminated\n");
	    return -1;
        }
	else return 1;
    }
    
    if(strcmp(event, "suspend") == 0) {
	printf("Suspending action %s\n",action);    
        if ((status = peos_notify(pid, action,PEOS_EVENT_SUSPEND)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	    printf("process encountered an illegal event and has been terminated\n");
	    return -1;
        }
	else return 1;
    }
    
    if(strcmp(event, "abort") == 0) {
	printf("Aborting action %s\n",action);    
        if ((status = peos_notify(pid, action,PEOS_EVENT_ABORT)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	    printf("process encountered an illegal event and has been terminated\n");
	    return -1;
        }
	else return 1;
    }
#ifndef PALM
    fprintf(stderr,"Unknown event\n");
#else
    printf("Unknown event\n");
#endif
    return -1;
}

int update_state() {

    int i=0;
    peos_action_t *tempalist;
    char **result = peos_list_instances();	
	
    while((i <= PEOS_MAX_PID)) {
	if (result[i] != NULL) {
	    int temp_num_actions;	
	    tempalist = peos_list_actions(i,&temp_num_actions);	
	    if(tempalist) {
                if(peos_notify(i, "dummy_action", PEOS_EVENT_RESOURCE_CHANGE) == VM_INTERNAL_ERROR) {
	            printf("Error in notifying resource change event\n");
	            return -1;
	        }
	    }
	}
	i++;
    }
    return 1;
}

void set_login_name(char *loginname)
{
    char *process_filename;
    
    process_filename = (char *) malloc((strlen(loginname) + strlen(".dat") +1) * sizeof(char));
    strcpy(process_filename, loginname);
    strcat(process_filename, ".dat"); 

    peos_set_process_table_file(process_filename);
    peos_set_loginname(loginname);
}



#ifndef PALM
/* stub out main because PalmPilot will not use command line interpreter */

int
main (int argc, char **argv)
{
    int pid;
    char *act_name;
    char *event;
    char c;
    char *res_name;
    char *res_val;
    char *model;
    int l = 0; /* l == 1 iff login option is passed */
    char *login = "proc_table"; /* default login name */
    //int b = 0; /* b == 1 iff binding option exists */
    char *res_file;
    char *var_file;
    opterr = 0;
    system ("echo '#######################################################################' >  pelog");
    system ("echo '###################   PREDICATE EVALUATOR DEBUG LOG    ################' >> pelog");
    system ("echo '#######################################################################' >> pelog");

    while ((c = getopt (argc, argv, "+c:n:ihrb:d:ul:")) != -1) {
        switch (c) {
            case 'l': {
	                  l = 1;
                          login = strdup(optarg);
                          break;
		      }
            case 'c': {
                          if (l == 0) {
                              if (argc != 3) {
                                  fprintf(stderr, "Usage: peos -c process_file\n");
                                  exit(EXIT_FAILURE);
                              }
                              model = argv[2];
                          }
                          else {
                              if (argc != 5) {
                                  fprintf(stderr, "Usage: peos -c process_file\n");
                                  exit(EXIT_FAILURE);
                              }
                              model = argv[4];
                          }
                          
                          set_login_name(login);
                          if (create_process(model) != 1) {
                              fprintf(stderr, "Could not Create Process\n");
			      exit(EXIT_FAILURE);
			  }
                          /*if (getopt(argc, argv, "b") == 'b')
                              b = 1;

                          if (l == 0) {
                              if (argc == 3) {
                                  model = argv[2];
                              }
                              else if (argc == 5 && b) {
                                  model = argv[2];
                                  res_file = argv[4];
                              }
                              else {
                                  if (b)
                                      fprintf(stderr, "Usage: peos -c process_file -b resource_file\n");
                                  else
                                      fprintf(stderr, "Usage: peos -c process_file\n");
                                  exit(EXIT_FAILURE);
                              }
                          }
                          else {
                              if (argc == 5) {
                                  model = argv[4];
                              }
                              else if (argc == 7 && b) {
                                  model = argv[4];
                                  res_file = argv[6];
                              }
                              else {
                                  if (b)
                                      fprintf(stderr, "Usage: peos -l login_name -c process_file -b resource_file\n");
                                  else
                                      fprintf(stderr, "Usage: peos -l login_name -c process_file\n");
	                          exit(EXIT_FAILURE);
                              }
                          }
                          set_login_name(login);
                          if((pid = create_process_ret_pid(model)) < 0) {
                              fprintf(stderr, "Could not Create Process\n");
			      exit(EXIT_FAILURE);
			  }
			  if (b)
			  {
			      bind_resources_value(pid, res_file);
			  }*/
			  return 1;
		      }
           case 'n': {
			 if(l == 0) {    
		             if(argc != 5) {
		                 fprintf(stderr, "Usage: peos -n pid act_name start|finish|suspend|abort\n");
	                         exit(EXIT_FAILURE);
	                     }
			     else {
                                 pid = atoi(argv[2]);
                                 act_name = argv[3];
                                 event = argv[4];
			     }
			 }
			 else {
			     if(argc != 7) {	 
		                 fprintf(stderr, "Usage: peos -l login_name -n pid act_name start|finish|suspend|abort\n");
	                         exit(EXIT_FAILURE);
	                     }
			     else {
                                 pid = atoi(argv[4]);
                                 act_name = argv[5];
                                 event = argv[6];
			     }
			 }
			 set_login_name(login);
			 if(notify(pid, act_name, event) < 0) {
			     fprintf(stderr, "Could not %s action %s\n",event, act_name);
		             exit(EXIT_FAILURE);
			 }
	                 else return 1;
			 break;
		     }
		     
	 case 'i': {
		       if(l == 0) {
                           if(argc != 2) {
		               fprintf(stderr, "Usage: peos -i \n");
			       exit(EXIT_FAILURE);
		           }
		       }
		       else {
                           if(argc != 5) {
		               fprintf(stderr, "Usage: peos -l login_name -i \n");
			       exit(EXIT_FAILURE);
		           }
		       }
		       {
		           int i;
		           char ** result;
		           set_login_name(login);
		           result = peos_list_instances(result);
		           if (result != NULL) {
		               for (i = 0; i <= PEOS_MAX_PID; i++) {
		                   printf("%d %s\n", i, result[i]);
		               }
			       return 1;
		           }
		           else {
		               fprintf(stderr, "error getting instances\n");
	                       exit(EXIT_FAILURE);
	                   }
		       }
                       break;
                   }
		   
	 case 'r': {
		       if(l == 0) {
                           if(argc != 5) {
		               fprintf(stderr, "Usage: peos -r pid resource_name resource_value\n");
			       exit(EXIT_FAILURE);
		           }
			   else {
                               pid = atoi(argv[2]);
			       res_name = argv[3];
			       res_val = argv[4];
			   }
		       }
		       else {
		           if(argc != 7) {
		               fprintf(stderr, "Usage: peos -l login_name -r pid resource_name resource_value\n");
			       exit(EXIT_FAILURE);
		           }
			   else {
                               pid = atoi(argv[4]);
			       res_name = argv[5];
			       res_val = argv[6];
			   }
		       }
		       set_login_name(login);
		       if(peos_set_resource_binding(pid, res_name, res_val) < 0) {    //see events.c
		           fprintf(stderr, "Could not bind resources");
		           exit(EXIT_FAILURE);
		       }
	               else return 1;
		       		   
                       break;
                   }
         case 'b': {
                       if(l == 0) {
                           if (argc != 4 && argc != 5) {
		               fprintf(stderr, "Usage: peos -b pid resource_file [variable_file]\n");
			       exit(EXIT_FAILURE);
		           }
			   else {
			       pid = atoi(argv[2]);
                               res_file = argv[3];
                               var_file = (argc == 5) ? argv[4] : NULL;
			   }
		       }
		       else {
		           if(argc != 6 && argc != 7) {
		               fprintf(stderr, "Usage: peos -l login_name -b pid resource_file [variable_file]\n");
			       exit(EXIT_FAILURE);
		           }
			   else {
                               pid = atoi(argv[4]);
			       res_file = argv[5];
			       var_file = (argc == 7) ? argv[6] : NULL;
			   }
		       }
		       set_login_name(login);
		       bind_resources_value(pid, res_file, var_file);
	               return 1;
                       break;
                   }
	 case 'd': {
		       if(l == 0) {	   
                           if(argc != 3) {
		               fprintf(stderr, "Usage: peos -d pid \n");
			       exit(EXIT_FAILURE);
		           }
			   else {
                               pid = atoi(argv[2]);
			   }
		       }
		       else {
                           if(argc != 5) {
		               fprintf(stderr, "Usage: peos -l login_name -d pid \n");
			       exit(EXIT_FAILURE);
		           }
		           else {
                               pid = atoi(argv[4]);
			   }
		       }
		       set_login_name(login);
		       if(peos_delete_process_instance(pid) < 0) {
			   fprintf(stderr, "Could not delete process instance\n");
		           exit(EXIT_FAILURE);
		       }
	               else return 1;
		       		   
                       break;
		   }

	 case 'u': {
		       if(l == 0) {	   
                           if(argc != 2) {
		               fprintf(stderr, "Usage: peos -u\n");
			       exit(EXIT_FAILURE);
		           }
		       }
		       else {
                           if(argc != 4) {
		               fprintf(stderr, "Usage: peos -l login_name -u\n");
			       exit(EXIT_FAILURE);
		           }
		       }
		       set_login_name(login);
		       if(update_state() < 0) {
		           fprintf(stderr, "Could not update process state\n");
		           exit(EXIT_FAILURE);
		       }
	               else return 1;
		       		   
                       break;
		   }

		   
	 case 'h': {
		       printf("To create a process: peos [-l login_name] -c name_of_model_file\n");
	               printf("To start an action: peos [-l login_name] -n process_id action_name event\n");
	               printf("Event can be: start or finish or abort or suspend\n");
	               printf("To update states of all processes (daemon simulation): peos [-l login_name] -u \n");
	               printf("To get a list of instances: peos [-l login_name] -i\n");
		       printf("To bind resources: peos [-l login_name] -r pid resource_name resource_value\n");
		       printf("To bind resources for resource file: peos [-l login_name] -b resource_file\n");
		       printf("To delete a process: peos [-l login_name] -d pid\n");
	               printf("To get help: peos -h\n");
		       break;
		   }	       
        

	 case '?': {
                       if (isprint (optopt))
                           fprintf (stderr, "Unknown option `-%c'.Please use peos -h for help.\n", optopt);
                       else
                           fprintf (stderr, "Unknown option character `\\x%x'.Please use peos -h for help.\n", optopt);
                       return 1;
		       break;
		   }
		
         default:
              abort ();
	}
    }
    return 0;
}
		   
#endif

		   
