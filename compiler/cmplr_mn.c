/******************************************************************************
**  File Information: compiler main program 
******************************************************************************/

#include <errno.h>
/* bugbug
#include <mem.h>
*/
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "datadict.h"
#include "defs.h"
#include "io_cpml.h"
#include "par_dd.h"

/* function prototypes */
int validate_inputs(int argc, char *argv[], char **pml_filename,
                    char *output_file_type,int *ppout,int *list);
void print_usage();

int main (int argc, char *argv[])
{
   char **pml_filename;
   char output_file_type[5];
   data_dictionary_struct data_dictionary;
   int returnval;
   int ppout = FALSE;
   int list = FALSE;

   printf("\nINFO:[main]-PML Compiler Version 2.0\n");

   /* make sure the user typed in valid command line arguments */
   returnval = validate_inputs(argc, argv, pml_filename, output_file_type, 
     &ppout,&list);
   if (returnval == FALSE)
   {
      printf("\nERROR:[main]-Invalid inputs detected\n");
      return returnval;
   } else if (returnval == STOP) {
      return returnval;
   } else {


   /* parse the file to make sure it is a valid PML file */
      returnval = parse_pml(*pml_filename, &data_dictionary);
	
      if (returnval == FALSE) {
         printf("\nERROR:[main]-Unable to parse the following PML file: %s\n",
             *pml_filename);
         return FALSE;
      }
   }

   if (returnval == TRUE) {
      returnval = write_cpml(*pml_filename, &data_dictionary, output_file_type);
      if (returnval == FALSE) {
         printf("\nERROR:[main]-Unable to create CPML file for: %s\n",
             *pml_filename);
         return FALSE;
      }
   }
   printf("\n");
   if (ppout == FALSE) {
      remove(*pml_filename);
   }
   if ((list == TRUE) && (strcmp(output_file_type,GDBM_MODE) == 0)) {
      printf("GOING TO KEEP THE LISTING file\n");
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
                    char *output_file_type,int *ppout,int *list)
{
   int fp;
   int returnval = TRUE;
   int i,j;
  
   char *filename = NULL;
   char *output = NULL;
   char *ptr = NULL;

   pid_t pid;

   /*
    * Check command line arguments for flags
    *   if -format flag is found use next argument
    *   as format field, if format flag in form of
    *   -f use next argument if format flag is two long
    *   or remainder of current argument. 
    *   i.e. -fgdbm or -f gdbm or -format gdbm are all
    *   legal.
    *
    */
   if (argc <= 1) {
      printf("\nERROR:[validate_inputs]-");
      printf("PML input file required");
      returnval = FALSE;
   }
   for ( i = 1; i < argc; i++) {
      if (returnval == FALSE) break;
      if (argv[i][0] == '-') {
	 if (strncmp(argv[i],"-h",2) == 0) {
    	    returnval = STOP;
	    break;
	 } 
	/* if (argc <= 2) {
	    printf("\nERROR: incomplete argument list -");
            returnval = FALSE;
	    break;
	 }*/
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
	          printf("\nERROR: incomplete argument list -");
                  returnval = FALSE;
	          break;
	       }
	       output = (char *)calloc(strlen(argv[i])+1,sizeof(char));
	       strcpy(output,argv[i]);
  	    } else {
	       output = (char *)calloc(strlen(argv[i])-1,sizeof(char));
	       strcpy(output,argv[i]+2);
	    }
	    for (j = 0; j < strlen(output); j++) {
	       output[j] = tolower(output[j]);
	    }
	    if ((strcmp(output,TEXT_MODE) !=0) &&
	        (strcmp(output,GDBM_MODE) !=0) &&
	        (strcmp(output,CPML_MODE) !=0)) {
	       printf("\nERROR:[validate_inputs]-");
               printf("\'%s\' invalid output format\n");
	       returnval = FALSE;
	       break;
  	    } 
	    continue;
         }
	
	 printf("\nERROR: invalid flags -");
	 returnval = FALSE;
	 break;

      } else {	
	 ptr = (strrchr(argv[i],'.'));
	 if (ptr == NULL) {
	    filename = (char *)calloc(strlen(argv[i])+5,sizeof(char));  	
    	    strcpy(filename,argv[i]);
 	    strcat(filename,".pml");
         } else {
            if (strcmp(ptr,".pml") != 0) {
	       printf("\nERROR:[validate_inputs]-");
	       printf("Expect pml extension: %s",argv[i]);
	       returnval = FALSE;
	       break;
            } else {
	       filename = (char *)calloc(strlen(argv[i])+1,sizeof(char));  	
	       strcpy(filename,argv[i]);
            }
	 }
	 
         if ((fp = open(filename, O_RDONLY)) < 0) {
       	    printf("\nERROR:[validate_inputs]: ");
	    printf("opening file %s failed: %s\n",filename,strerror(errno));
	    returnval = FALSE;
         } else {
            close(fp);
	 }
      }
   }
   if (returnval == TRUE) { 
      if (output != NULL) {
  	 strcpy(output_file_type, output);
	 free(output);
      } else {
	 strcpy(output_file_type, GDBM_MODE);
      }
      if (filename == NULL) {
 	 printf("\nERROR[validate_inputs]: pml source code file required\n");
         print_usage();
         returnval = FALSE;	
      } else {
         *pml_filename = (char *)calloc(strlen(filename)+4,sizeof(char));
         strcpy(*pml_filename, filename);
         strcat(*pml_filename,".pp");
         
         pid = fork();
         if (pid == 0) {
            //execlp("/usr/bin/cpp","cpp",filename,*pml_filename,"-P",(char *) 0);
            execlp("/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/cpp",
              "cpp",filename,*pml_filename,"-P",(char *) 0);
         }
         waitpid(pid, NULL, 0); 
         if ((fp = open(*pml_filename, O_RDONLY)) < 0) {
       	    printf("\nWARNING:[validate_inputs]: ");
	    printf("C preprocessor failed\n");
	    memset(*pml_filename,0,sizeof(filename)+4); 
	    *pml_filename = (char *)calloc(strlen(filename)+1,sizeof(char));
	    strcpy(*pml_filename,filename);
	
            *ppout = TRUE;
         } else {
            close(fp);
         }
         free(filename);    
      }

   } else {
 	 print_usage();	
   }
   return returnval;
}

/******************************************************************************
**
**    Description:         Prints unix style usage 
**                          
******************************************************************************/
void print_usage()
{
   printf("\nUsage - \n\tcompiler [-h] [-f] [-l] [-P] <pml source file>\n");
   printf("\t-h[elp] : usage\n");
   printf("\t-f[ormat]  CPML|TEXT|GDBM : output format CPML,TEXT,GDBM\n");
   printf("\t\tCPML output is currently equivalent to TEXT output,\n");
   printf("\t\tthe extensions will differ.  The format request can be\n");
   printf("\t\tall upper case or all lower case.\n");
   printf("\n\t-l : retains list file when output is gdbm\n");
   printf("\t-P : retains output from C pre-processor (filename.pml.pp)\n");
   printf("\n\tThe pml source file extension is \"pml\". If no extension ");
   printf("is specified\n\t.pml is assumed \n\n");
   printf("\tExamples:\n");
   printf("\t\t compiler -fgdbm filename.pml\n");
   printf("\t\t compiler filename -f GDBM\n");
   printf("\t\t compiler filename -format text\n");
}
