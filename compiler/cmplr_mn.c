/******************************************************************************
**  File Information: compiler main program 
******************************************************************************/

#include <stdio.h>
/* bugbug
#include <mem.h>
*/
#include <string.h>
#include "datadict.h"
#include "defs.h"
#include "io_cpml.h"
#include "par_dd.h"



/* function prototypes */
int validate_inputs(int argc, char *argv[], char *pml_filename,
                    char *output_file_type);

int main (int argc, char *argv[])
{
   char pml_filename[80];
   char output_file_type[80];
   data_dictionary_struct data_dictionary;

   printf("\nINFO:[main]-PML Compiler Version 2.0");

   /* this must be done before using the dictionary */
   data_dictionary.root = NULL;

   /* reset strings */
   memset(pml_filename, 0, 80);
   memset(output_file_type, 0, 80);

   /* make sure the user typed in valid command line arguments */
   if (validate_inputs(argc, argv, pml_filename, output_file_type) == FALSE)
   {
      printf("\nERROR:[main]-Invalid inputs detected");
      return FALSE;
   }

   /* parse the file to make sure it is a valid PML file */
   if (parse_pml(pml_filename, &data_dictionary) == FALSE)
   {
      printf("\nERROR:[main]-Unable to parse the following PML file: %s",
             pml_filename);
      return FALSE;
   }

   /* check to be sure the data dictionary is not empty */
   if (data_dictionary.root == NULL)
   {
      printf("\nERROR:[main]-Data dictionary is empty!");
      return FALSE;
   }

   if (add_level_info_to_data_dictionary(&data_dictionary) == FALSE)
   {
      printf("\nERROR:[main]-Unable to parse the following PML file: %s",
             pml_filename);
      return FALSE;
   }

   /* generate the appropriate CPML output file */
   if (write_cpml(pml_filename, &data_dictionary, output_file_type) == FALSE)
   {
      printf("\nERROR:[main]-Unable to create CPML file for: %s",
             pml_filename);
      return FALSE;

   }


   return TRUE;
}


/******************************************************************************
**    Function/Method Name: validate_inputs
**    Precondition:
**    Postcondition:       
**    Description:          validiate the users inputs 
**                          
** compiler accepts the following command line arguments:
**
** example :: "compiler -input martini2.pml -format text"  (plain text)
**            "compiler -input martini2.pml -format gdbm"  (gdbm version)
******************************************************************************/
int validate_inputs(int argc, char *argv[], char *pml_filename,
                    char *output_file_type)
{
   FILE *fp;

   switch (argc) {
	case 2:
   		/* make sure the specified pml file exists */
   		if ((fp = fopen(argv[1], "r")) == NULL)
		{
      			printf("\nERROR:[validate_inputs]-");
      			printf("Unable to open pml file: %s", argv[1]);
			return FALSE;
		}
   		/* everything is valid so copy the information into the return variables */
   		strcpy(pml_filename, argv[1]);
   		strcpy(output_file_type, GDBMSTR);
		break;
	case 4:
   		/* make sure the specified pml file exists */
   		if ((fp = fopen(argv[3], "r")) == NULL)
		{
      			printf("\nERROR:[validate_inputs]-");
      			printf("Unable to open pml file: %s", argv[3]);
			return FALSE;
		}
   		/* make sure we have a valid format type */
   		if ((strcmp(argv[2], TEXTSTR) != 0) && (strcmp(argv[2], GDBMSTR) != 0))
		{
      			printf("\nERROR:[validate_inputs]-");
      			printf("The format type must be either \"text\" or \"gdbm\"");
			return FALSE;
		}
   		/* everything is valid so copy the information into the return variables */
   		strcpy(pml_filename, argv[3]);
   		strcpy(output_file_type, argv[2]);
		break;
	default:
      		printf("\nERROR:[validate_inputs]-");
   		printf("The inputs to the compiler must be in the following format:\n");
      		printf("\tusage: compiler [-f \"%s\"|\"%s\"] \"pml_filename\" \n",TEXTSTR,GDBMSTR);
      		return FALSE;
   }

   /* dont forget to close the file */
   fclose(fp);

   return TRUE;
}
