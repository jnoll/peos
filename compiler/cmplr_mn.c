/******************************************************************************
**  File Information: compiler main program 
******************************************************************************/

#include <errno.h>
/* bugbug
#include <mem.h>
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "datadict.h"
#include "defs.h"
#include "io_cpml.h"
#include "par_dd.h"

/* function prototypes */
int validate_inputs(int argc, char *argv[], char *pml_filename,
                    char *output_file_type);
void print_usage();

int main (int argc, char *argv[])
{
   char pml_filename[80];
   char output_file_type[80];
   data_dictionary_struct data_dictionary;
   int returnval;

   printf("\nINFO:[main]-PML Compiler Version 2.0\n");

   /* reset strings */
   memset(pml_filename, 0, 80);
   memset(output_file_type, 0, 80);

   /* make sure the user typed in valid command line arguments */
   returnval = validate_inputs(argc, argv, pml_filename, output_file_type);
   if (returnval == FALSE)
   {
      printf("\nERROR:[main]-Invalid inputs detected\n");
      return returnval;
   } else if (returnval == STOP) {
      return returnval;
   }

   /* parse the file to make sure it is a valid PML file */
   if (parse_pml(pml_filename, &data_dictionary) == FALSE)
   {
      printf("\nERROR:[main]-Unable to parse the following PML file: %s\n",
             pml_filename);
      return FALSE;
   }

   /*if (data_dictionary.root == NULL)
   {
      printf("\nERROR:[main]-Data dictionary is empty!\n");
      return FALSE;
   }

   if (add_level_info_to_data_dictionary(&data_dictionary) == FALSE)
   {
      printf("\nERROR:[main]-Unable to parse the following PML file: %s\n",
             pml_filename);
      return FALSE;
   }*/

   /* generate the appropriate CPML output file */
   if (write_cpml(pml_filename, &data_dictionary, output_file_type) == FALSE)
   {
      printf("\nERROR:[main]-Unable to create CPML file for: %s\n",
             pml_filename);
      return FALSE;

   }
   printf("\n");
   return TRUE;
}


/******************************************************************************
**
**    Description:          validiate the user's inputs before running
**			    source through the compiler. 
**                          
******************************************************************************/
int validate_inputs(int argc, char *argv[], char *pml_filename,
                    char *output_file_type)
{
   int fp;
   int returnval = TRUE;
   int i,j;
  
   char *filename = NULL;
   char *output = NULL;
   char *ptr = NULL;

   if (argc <= 1) {
      printf("\nERROR:[validate_inputs]-");
      printf("PML input file required");
      returnval = FALSE;
   }
   /*
    * Check command line arguments for flags
    *   if -format flag is found use next argument
    *   as format field, if format flag in form of
    *   -f use next argument if format flag is two long
    *   or remainder of current argument. 
    *   i.e. -fgdbm or -f gdbm or -format gdbm are all
    *   legal.
    */
   for ( i = 1; i < argc; i++) {
      if (returnval <= FALSE) break;
      if (argv[i][0] == '-') {
	 if (strncmp(argv[i],"-h",3) == 0) {
    	    returnval = STOP;
	    break;
	 } else if (strncmp(argv[i],"-f",2) != 0) {
	    printf("\nERROR: invalid flag -");
	    returnval = FALSE;
	    break;
	 }
 	 if ((strcmp(argv[i],"-format") == 0) ||
	    (strcmp(argv[i],"-f") == 0)) {
	    i++;
	    output = (char *)malloc(strlen(argv[i])+1);
	    strcpy(output,argv[i]);
	 } else {
	    output = (char *)malloc(strlen(argv[i])-1);
	    strcpy(output,argv[i]+2);
	 }
	 for (j = 0; j < strlen(output); j++) {
	    output[j] = tolower(output[j]);
	 }
	 if ((strcmp(output,TEXT_MODE) !=0) &&
	     (strcmp(output,GDBM_MODE) !=0) &&
	     (strcmp(output,CPML_MODE) !=0)) {
	    printf("\nERROR:[validate_inputs]-");
	    printf("\'%s\' not a valid output format",output);
	    returnval = FALSE;
	} 
      } else {	
	 ptr = (strrchr(argv[i],'.'));
	 if (ptr == NULL) {
	    
	    printf("\nERROR:[validate_inputs]-");
	    printf("Expect pml extenstion: %s",argv[i]);
	    returnval = FALSE;
	    break;
         }
	 filename = (char *)malloc(strlen(argv[i]+1));  	
	 strcpy(filename,argv[i]);
	 
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
      strcpy(pml_filename, filename);

   } else {
 	 print_usage();	
   }

   if (filename != NULL)
      free(filename);    

   return returnval;
}

/******************************************************************************
**
**    Description:         Prints unix style usage 
**                          
******************************************************************************/
void print_usage()
{
   printf("\nUsage - \n\tcompiler [-f] [-h] <pml source file>\n");
   printf("\t-h[elp] : usage\n");
   printf("\t-f[ormat]  CPML|TEXT|GDBM : output format CPML,TEXT,GDBM\n");
   printf("\t\tCPML output is currently equivalent to TEXT output,\n");
   printf("\t\tthe extensions will differ.  The format request can be\n");
   printf("\t\tall upper case or all lower case.\n");
   printf("\n\tThe pml source file extension is \"pml\".  \n\n");
   printf("\tExamples:\n");
   printf("\t\t compiler -fgdbm filename.pml\n");
   printf("\t\t compiler filename -f GDBM\n");
   printf("\t\t compiler filename -format text\n");
}
