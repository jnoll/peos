/* INCLUDES ************************************************/

#include "pml_state.h"
#include <stdio.h>

/***********************************************************/

/* MAIN ****************************************************/
int main(int argc, char *argv[])
{
  /* local variable declarations */
  int i = 0;                 /* loop counter */
  int rcode = 0;             /* return code */
  int nobjs = 0;             /* object counter */ 
  char value[256];           /* value read buffer */
  pml_obj_t objp1 = NULL;    /* object handle */
  pml_obj_t objp2 = NULL;    /* object handle */
  pml_obj_t objp3 = NULL;    /* object handle */
  pml_obj_t *resp = NULL;    /* query result pointer */ 
  pml_obj_t *atrp = NULL;    /* attribute pointer */ 


  /* open repository in append mode */
  if ((rcode = pml_open_repository(NULL, APPEND)) < 0)
  {
    perror("pml_open_repository failed");
    exit(1);
  }

  /* create some objects */
  objp1 = pml_create_object();
  objp2 = pml_create_object();
  objp3 = pml_create_object();

  /* write some attributes */
  pml_write_attribute(objp1, "attrib1", 8, "value1", 7);
  pml_write_attribute(objp2, "attrib1", 8, "value2", 7);
  pml_write_attribute(objp3, "attrib1", 8, "value3", 7);
	/* NOTE:  can write any data type or binary structure */

  /* read back attributes */
  pml_read_attribute(objp1, "attrib1", 8, value, sizeof(value));
  pml_read_attribute(objp2, "attrib1", 8, value, sizeof(value));
  pml_read_attribute(objp3, "attrib1", 8, value, sizeof(value));
	/* NOTE:  can read any data type or binary structure */

  /* query for object which has attrib1 = value2 */
  nobjs = pml_query_open(&resp, "attrib1", 8, EQ, "value2", 7); 
  printf("\nobjects with attrib1 == value2\n");

  /* get attributes for query results */
  for (i = 0, atrp = resp; i < nobjs; i++, atrp++)
  {
    pml_read_attribute(*atrp, "attrib1", 8, value, sizeof(value));
    printf("object %d value = %s\n", i+1, value);
  }

  /* close query */
  pml_query_close(&resp);

  /* query for object which has attrib1 != value2 */
  nobjs = pml_query_open(&resp, "attrib1", 8, NE, "value2", 7); 
  printf("\nobjects with attrib1 != value2\n");

  /* get attributes for query results */
  for (i = 0, atrp = resp; i < nobjs; i++, atrp++)
  {
    pml_read_attribute(*atrp, "attrib1", 8, value, sizeof(value));
    printf("object %d value = %s\n", i+1, value);
  }

  /* close query */
  pml_query_close(&resp);

  /* query for object which has attrib1 > value2 */
  nobjs = pml_query_open(&resp, "attrib1", 8, GT, "value2", 7); 
  printf("\nobjects with attrib1 > value2\n");

  /* get attributes for query results */
  for (i = 0, atrp = resp; i < nobjs; i++, atrp++)
  {
    pml_read_attribute(*atrp, "attrib1", 8, value, sizeof(value));
    printf("object %d value = %s\n", i+1, value);
  }

  /* close query */
  pml_query_close(&resp);

  /* query for object which has attrib1 < value3 */
  nobjs = pml_query_open(&resp, "attrib1", 8, LT, "value3", 7); 
  printf("\nobjects with attrib1 < value3\n");

  /* get attributes for query results */
  for (i = 0, atrp = resp; i < nobjs; i++, atrp++)
  {
    pml_read_attribute(*atrp, "attrib1", 8, value, sizeof(value));
    printf("object %d value = %s\n", i+1, value);
  }

  /* close query */
  pml_query_close(&resp);

  /* flag created objects for deletion */
  pml_delete_object(objp1);
  pml_delete_object(objp2);
  pml_delete_object(objp3);

  /* remove objects flagged for deletion from repository */
  pml_pack_objects();

  /* close repository */
  pml_close_repository();

  return 0;
}
/***********************************************************/

