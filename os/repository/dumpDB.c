/***********************************
 * Author: Mark Mastroieni
 * Date: 8/16/2002
 * File: dumpDB.c
 * Purpose: dump the entire repository to the output, 
 * for examination by the user, in the format
 *  object id | attribute name | attribute value
 **********************************/

#include <stdio.h>
#include "repository.h"
#include "pml_state.h"

int main (int argc, char *argv[]) {

  /* local variable declarations */
  int i, j;
  int nobj = 0; 
  int nattr = 0;
  pml_obj_t *objptr, *tmpObj;
  char **attribs;
  char **vals;
  
  /* open (the default) repository in append mode */
  if ( (pml_open_repository(NULL, APPEND)) < 0 ) {
    printf("pml_open_repository failed!\n"); 
    exit(-1); 
  } 

  nobj = pml_list_open(&objptr);
  printf("%-10s | %-25s | %-25s\n",
	 "object id", "attribute name", "attrib. value");
  printf("%-10s | %-25s | %-25s\n",
	 "---------", "--------- ----", "------- -----");
  for(i=0, tmpObj = objptr; i<nobj; i++, tmpObj++) {
    nattr = pml_list_attributes(*tmpObj, &attribs, &vals);
    for(j=0; j<nattr; j++) {
      printf("%-10u | %-25s | %-25s\n", tmpObj, attribs[j], vals[j]);
    } /* done w/ this object... */
    pml_free_alist(attribs, nattr); 
    pml_free_alist(vals, nattr);
    printf("\n");
  } /* done w/ all objects in the repository */
  

  pml_query_close(&objptr);
  pml_close_repository();
}
