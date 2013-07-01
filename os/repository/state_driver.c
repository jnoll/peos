/* INCLUDES ************************************************/

#include "pml_state.h"
#include <stdio.h>

/***********************************************************/

/* Attribute names. */
#define ATTR_1 "attr1"
#define ATTR_2 "attr2"

/* Attribute values. */
#define V1_1 "value1.1"
#define V2_1 "value2.1"
#define V3_1 "value3.1"
#define V1_2 "value1.2"
#define V2_2 "value2.2"
#define V3_2 "value3.2"

/* Set attribute called 'name' to 'val'.  Assumes 'val' is a string. */
int set_attr(pml_obj_t obj, char *name, char *val)
{
  return  pml_write_attribute(obj, name, strlen(name) + 1,
			      val, strlen(val) + 1);
}


/* Retrieve attribute called 'name' and compare to 'expected_val'.  */
int get_attr(pml_obj_t obj, char *name, char *expected_val)
{
  char value[256];           /* value read buffer */
  memset(value, 0, sizeof(value));
  pml_read_attribute(obj, name, strlen(name) + 1, value, sizeof(value));
  if (memcmp(value, expected_val, strlen(expected_val) + 1) != 0) {
      printf("Object %u: retrieved value '%s' != expected value '%s'\n",
	     obj, value, expected_val);
  }

}

/* List attributes and compare to 'read' value. */
int list_attr(pml_obj_t obj) {
    int i, j, nattr;
    char **attrs;
    char **vals;

    if ((nattr = pml_list_attributes(obj, &attrs, &vals)) < 0) {
	printf("list_attr: error listing attributes of object %u\n", obj);
    } else if (nattr == 0) {
	printf("list_attr: object %u has %d attributes\n", obj, nattr);
    }
    
    for (i = 0; i < nattr; i++) {
	get_attr(obj, attrs[i], vals[i]);
    }

    pml_free_alist(attrs, nattr);
    pml_free_alist(vals, nattr);
}

/* MAIN ****************************************************/
int main(int argc, char *argv[])
{
    /* local variable declarations */
    int i = 0;                 /* loop counter */
    int nobjs = 0;             /* object counter */ 
    char value[256];           /* value read buffer */
    pml_obj_t objp1 = NULL;    /* object handle */
    pml_obj_t objp2 = NULL;    /* object handle */
    pml_obj_t objp3 = NULL;    /* object handle */
    pml_obj_t *resp = NULL;    /* query result pointer */ 
    pml_obj_t *atrp = NULL;    /* attribute pointer */ 


  /* open repository in append mode */
    if (pml_open_repository(NULL, APPEND) < 0) {
	perror("pml_open_repository failed");
	exit(1);
    }

    /* create some objects */
    objp1 = pmlstate_create_object();
    objp2 = pmlstate_create_object();
    objp3 = pmlstate_create_object();

    /* write some attributes */
    set_attr(objp1, ATTR_1, V1_1);
    set_attr(objp2, ATTR_1, V2_1);
    set_attr(objp3, ATTR_1, V3_1);

    /* read back attributes */
    get_attr(objp1, ATTR_1, V1_1);
    get_attr(objp2, ATTR_1, V2_1);
    get_attr(objp3, ATTR_1, V3_1);

    /* re-write some attributes */
    set_attr(objp1, ATTR_1, V1_2);
    set_attr(objp2, ATTR_1, V2_2);
    set_attr(objp3, ATTR_1, V3_2);

    /* read back new values */
    get_attr(objp1, ATTR_1, V1_2);
    get_attr(objp2, ATTR_1, V2_2);
    get_attr(objp3, ATTR_1, V3_2);

    /* test pml_list_attributes */
    list_attr(objp1);
    list_attr(objp2);
    list_attr(objp3);

    /* query for object which has attrib1 = value2 */
    nobjs = pml_query_open(&resp, ATTR_1, strlen(ATTR_1), EQ, V1_1, strlen(V1_1)); 
    printf("\nobjects with %s == %s\n", ATTR_1, V1_1);

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
	    memset(value, 0, sizeof(value));
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
	    memset(value, 0, sizeof(value));
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
	    memset(value, 0, sizeof(value));
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


