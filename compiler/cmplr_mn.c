/******************************************************************************
**  File Information: compiler main program 
******************************************************************************/

#include <errno.h>
/* bugbug
#include <mem.h>
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


#include "datadict.h"
#ifndef _DEFS_H_
#include "defs.h"
#endif

#include "io_cpml.h"
#include "par_dd.h"

#ifndef _PML_MSG_H_
#include "pml_msg.h"
#endif

/* function prototypes */
int validate_inputs(int argc, char *argv[], char **pml_filename,
    char *output_file_type,Boolean *ppout,Boolean *list,Boolean *debug);
void print_usage();

int main (int argc, char *argv[])
{
	char *pml_filename;
	char *module = "main";
	char output_file_type[5];
	data_dictionary_struct data_dictionary;
	int returnval;
	Boolean ppout = FALSE;
	Boolean list = FALSE;
	Boolean debug = FALSE;
	int old_format = FALSE;

	pmlprint(INFO,PMLVER,module,NULL);

	/* make sure the user typed in valid command line arguments */
	returnval = validate_inputs(argc, argv, &pml_filename, output_file_type, 
	    &ppout,&list,&debug);
	if (returnval == FALSE) {
		pmlprint(ERROR,PMLINP,module,NULL);
		return returnval;
	} else if (returnval == STOP) {
		return returnval;
	} else {
		/* parse the file to make sure it is a valid PML file */
		returnval = parse_pml(pml_filename, &data_dictionary,
		    debug);
	}
	if (returnval == TRUE) {
		returnval = write_cpml(pml_filename, &data_dictionary,
		    output_file_type,debug,list);
	}
	if (ppout == FALSE) {
		remove(pml_filename);
	}
	return returnval;
}

/******************************************************************************
**
**    Description:          validiate the user's inputs before running
**			    source through the compiler. 
**                          
******************************************************************************/

int validate_inputs(int argc, char *argv[], char **pml_filename,
    char *output_file_type,Boolean *ppout,Boolean *list,Boolean *debug)
{
	int fp;
	int returnval = TRUE;
	int i,j;
  
	char *filename = NULL;
	char *output = NULL;
	char *ptr = NULL;
	char *module = "validate_inputs";
	char *cppenv = NULL;
	char *cpppath = NULL;

	char *cppcmd = "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/cpp";
	pid_t pid;

	/*
	 * Check command line arguments for flags
	 *   argc must be >=2.
	 *
	 *   if -format flag is found use next argument
	 *   as format field, if format flag in form of
	 *   -f use next argument if format flag is two long
	 *   or remainder of current argument. 
	 *   i.e. -fgdbm or -f gdbm or -format gdbm are all
	 *   legal.
	 *
	 *   Memory allocation failures set returnval to false
	 *   and no further checking is allowed.  If there's not
	 *   enough memory to validate inputs, there's definitely
	 *   not enough to parse the pml input source code file.
	 *
	 *   See print_usage() for input flags.
	 */
	if (argc <= 1) {
		pmlprint(ERROR,PMLINP,module,"PML input file required");
		returnval = FALSE;
	}
	for(i = 1; i < argc; i++) {
		if (returnval == FALSE) break;
		if (argv[i][0] == '-') {
		if (strcmp(argv[i],"-d") == 0) {
			*debug = TRUE;
			  continue;
		}
		if (strncmp(argv[i],"-h",2) == 0) {
			returnval = STOP;
			break;
		} 
		if (strcmp(argv[i],"-P") == 0) {
			*ppout = TRUE;
			continue;
		}
		if (strcmp(argv[i], "-l") == 0) {
			*list = TRUE;
			continue;
		}
		if (strncmp(argv[i],"-f",2) == 0) {
			if ((strcmp(argv[i],"-format") == 0) ||
			    (strcmp(argv[i],"-f") == 0)) {
				i++;
				if ( i >= argc) {
					pmlprint(ERROR,PMLINP,module,
					    "Incomplete argument list");
					returnval = FALSE;
					break;
				}
				if((output = (char *)calloc(strlen(argv[i])+1,
				    sizeof(char))) == NULL) {
					pmlprint(ERROR,PMLMEM,module,NULL);
					returnval = FALSE;
					break;
				} else {
					strcpy(output,argv[i]);
				}
			} else {
  				if ((output = (char *)calloc(strlen (argv[i]) 
				    - 1, sizeof (char))) == NULL) {
					pmlprint(ERROR,PMLMEM,module,NULL);
					returnval = FALSE;
					break;
				} else {
					strcpy(output,argv[i]+2);
				}
			}
			for (j = 0; j < strlen(output); j++) {
				output[j] = tolower(output[j]);
			}
			if ((strcmp(output,TEXT_MODE) !=0) &&
			    (strcmp(output,GDBM_MODE) !=0) &&
			    (strcmp(output,CPML_MODE) !=0)) {
				pmlprint(ERROR,PMLINP,module,
       				    "Invalid output format");
				returnval = FALSE;
				break;
			} 
			continue;
		}
		pmlprint(ERROR,PMLINP,module,"Invalid flags");
		returnval = FALSE;
		break;

	} else {	
		ptr = (strrchr(argv[i],'.'));
		if (ptr == NULL) {
			if((filename = (char *)calloc(strlen(argv[i])+5,
			    sizeof(char))) == NULL) {  	
				pmlprint(ERROR,PMLMEM,module,NULL);
				returnval = FALSE;
				break;
			} else {
				strcpy(filename,argv[i]);
				strcat(filename,".pml");
			}
		} else {
			if (strcmp(ptr,".pml") != 0) {
				pmlprint(ERROR,PMLINP,module,
				    "pml extension expected");
				returnval = FALSE;
				break;
			} else {
				if((filename = (char *)calloc(strlen(argv[i])
				    +1,sizeof(char))) == NULL) {
					pmlprint(ERROR,PMLMEM,module,NULL);
					returnval = FALSE;
					break;
				} else {  	
					strcpy(filename,argv[i]);
				}
			}
		}
 
		if ((fp = open(filename, O_RDONLY)) < 0) {
			pmlprint(ERROR,PMLFOPN,module,filename);
			returnval = FALSE;
		} else {
			close(fp);
			} 
		}
	}
	/*
	 *  All command line arguments have passed whatever tests
	 *  were defined in the previous while loop.  Time to check
	 *  existence of pml source code file and if it exists run
	 *  it through the C preprocessor.  A preset path to the 
	 *  C preprocessor is specified if no environment variable is
	 *  set.  If the C preprocessor step fails, further compilation
	 *  will continue, but #include, macro defines, etc. will
	 *  cause the compilation to fail.  
	 */
	if (returnval == TRUE) { 
		if (output != NULL) {
			strcpy(output_file_type, output);
			free(output);
		} else {
			strcpy(output_file_type, GDBM_MODE);
		}
		if (filename == NULL) {
			pmlprint(ERROR,PMLINP,module,
			    "PML source code file required");
			print_usage();
			returnval = FALSE;	
		} else {
			if((*pml_filename = (char *)calloc(strlen(filename)+4,
			    sizeof(char))) == NULL) {
				pmlprint(ERROR,PMLMEM,module,NULL);
				returnval = FALSE;
			} else {
				strcpy(*pml_filename, filename);
				strcat(*pml_filename,".pp");
				if ((cppenv = getenv("CPP")) != NULL) {
					if((cpppath = (char *)calloc(
					    strlen(cppenv)+1,sizeof(char)))
					    == NULL) {
						pmlprint(WARN,PMLMEM,
						    module, NULL);
						returnval = FALSE;
					} else {
						strcpy(cpppath,cppenv);
					}
				} else {
					if((cpppath = (char *)calloc(
					    strlen(cppcmd)+1,sizeof(char)))
					    == NULL) {
						pmlprint(WARN,PMLMEM,
						    module, NULL);
						returnval = FALSE;
					} else {
						strcpy(cpppath,cppcmd);
					}
				} 
				pid = fork();
				if (pid == 0) {
					if (execlp(cpppath,
					    "cpp",filename,*pml_filename,
					    "-P",(char *) 0) < 0) {
						exit(1);
					}
				}
				waitpid(pid, NULL, 0); 
				if ((fp = open(*pml_filename, O_RDONLY)) < 0) {
					pmlprint(WARN,PMLCPP,module,NULL);
					memset(*pml_filename,0,
					    sizeof(filename)+4); 
					*pml_filename = (char *)calloc(
					    strlen(filename)+1,sizeof(char));
					strcpy(*pml_filename,filename);
					*ppout = TRUE;
				} else {
					close(fp);
				}
			}
			free(filename);    
		}

	} else {
		print_usage();	
	}
	free(cpppath);
	return returnval;
}

/******************************************************************************
**
**    Description:         Prints unix style usage 
**                          
******************************************************************************/
void print_usage()
{
	printf("\nUsage - \n\tpmlc [-d] [-h] [-f] [-l] [-P] ");
	printf("<pml source file>\n");
	printf("\nNote: The command \"pmlc\" performs the same function");
	printf("as \"compiler\".\n  The \"compiler\" command remains for ");
	printf("backward compatibility.\n\n");
	printf("\t-d : debug information.  Displays data dictionary and\n");
	printf("\t\tif gdbm, reads gdbm file.\n");
	printf("\t-h[elp] : usage\n");
	printf("\n\t-f[ormat]  CPML|TEXT|GDBM : ");
	printf("output format CPML,TEXT,GDBM\n");
	printf("\t\tCPML output is currently equivalent to TEXT output,\n");
	printf("\t\tthe extensions will differ.  ");
	printf("The format request can be\n");
	printf("\t\tall upper case or all lower case.\n");
	printf("\n\t-l : retains list file when output is gdbm\n");
	printf("\t-P : retains output from C pre-processor ");
	printf("filename.pml.pp)\n");
	printf("\n\tThe pml source file extension is \"pml\". ");
	printf("If no extension ");
	printf("is specified\n\t\".pml\" is assumed \n\n");
	printf("\tExamples:\n");
	printf("\t\t compiler -fgdbm filename.pml\n");
	printf("\t\t compiler filename -f GDBM\n");
	printf("\t\t compiler filename -format text\n");
}
