#include <stdlib.h>	/* EXIT_SUCCESS EXIT_FAILURE */
#include <string.h>
#include <ctype.h>
#include <gconf/gconf.h>
#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "/home/jmontana/include/check.h"
#include "glo.h"
#include "test_util.h"
#include "set_globals.h"
#include "set_test.h"

/* prototypes */
void set_CUR_PID(int);
char *case1_setup(char *);
int case2_setup(char *);
int case3_setup();
int case4_setup();

START_TEST(tSet_href)
{ 
 
  #define TEST_SIZE 256 
  int size = 0;
  char *buf = NULL, *temp = NULL;
  int rtn_val;

  /* set current process id to 0 */
  set_CUR_PID((int) 0);
  
  /* temp test buffer to pass to set_href() */
  size = TEST_SIZE + 1;
  buf = (char *) malloc(size);
  strcpy(buf, "");        /* copy NULL byte */

  set_href(buf, NULL, NULL);
  /* test while glist is NULL */
  if(strcmp(buf, "") != 0) {
  	fail("The call to set_href should not set buf as long as glist is NULL \n");
  }

  /* setup global values outside of this address space */
  buf = case1_setup((char *) buf);

#ifdef DEBUG
  /* test while glist is not NULL */
  if(strcmp(buf, "") == 0) {
  	fail("The call to set_href should set buf as long as glist is not NULL \n");
  }

  /* test to make sure the buffer is set right */
  rtn_val = case2_setup((char *) buf);

  if(rtn_val == 1) {
  	fail("The buffer was not setup correctly. \n");
  } 

  /* test to make sure test reports failure if buffer is not set correclty */
  strcpy(buf, "fail");
  rtn_val = case2_setup((char *) buf);
  
  if(rtn_val == 0) {
  	fail("The test must fail if buffer is set incorrrectly. \n");
  }

  /* set CUR_PID to be an invalid number */
  set_CUR_PID((int) -1);
  rtn_val = set_href(buf, NULL, NULL);
  if(rtn_val == 0) {
  	fail("The function should exit with invalid id. \n");
  }

  /* Set cwd to NULL and make sure it fails. */
  temp = (char *) set_CWD((char *) NULL);
  set_href(buf, NULL, NULL);
  if(rtn_val == 0) {
  	fail("set_href should return 1 under these conditions. \n");
  }

  free(cwd);
  cwd = NULL;
  free(temp);
  temp = NULL;
  free(buf);
  buf = NULL;
#endif  

}
END_TEST

START_TEST(tLookup_rsc_name)
{
  
  int rtn_val = -1;

  set_CUR_PID((int) 0);
  rtn_val = case3_setup();

#ifdef DEBUG
  if(rtn_val == 1) {
  	fail("Call to lookup_rsc_name in htmllinkage.c failed. \n");
  }
#endif
  /* test the case where we pass NULL arguments to lookup_src_name in htmllinkage.c */
  rtn_val = case4_setup();

  if(rtn_val != -1) {
	fail("Function should return -1 if arguments are set to NULL \n");
  }
  
}
END_TEST

Suite *htmllinkage_suite (void) 
{ 
  Suite *s = suite_create ("htmllinkage_suite"); 

  /* Create test harnest for functions in module */
  TCase *tc = tcase_create ("test_Set_href");
  TCase *tc1 = tcase_create ("tLookup_rsc_name");

  suite_add_tcase (s, tc);
  suite_add_tcase (s, tc1);

  tcase_add_test (tc, tSet_href); 
  tcase_add_test (tc1, tLookup_rsc_name); 

  return s; 
}

int main(int argc, char** argv)
{
  
  int nf;

  Suite *s = (Suite *) htmllinkage_suite ();
  SRunner *sr = srunner_create (s);

  /* optional parameters */
  parse_args(argc, argv);

  /* Run program in separate address space than PEOS.
   * srunner_set_fork_status(sr, fork_status);
   */
   

  /* Run suite tests. */
  srunner_run_all (sr, CK_NORMAL);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);
  sr = NULL;

  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 

}

