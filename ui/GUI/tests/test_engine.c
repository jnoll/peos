/* XXX - unistd.h under RH9 fails to declare get_current_dir_name correctly */
extern char *get_current_dir_name(void);
#include <stdlib.h>	/* EXIT_SUCCESS EXIT_FAILURE */
#include <string.h>
#include "/home/jmontana/include/check.h"
#include "engine.h"
#include "glo.h"

START_TEST(trunPeos)
{ 
#ifdef DEBUG
  int rtn_val = -1;
  size_t size = 0;
  char *command = NULL, *peosPath = NULL, *cmd_peos = NULL, *str_rtn = NULL;

  /* Locate the bin directory that contains peos executable. */ 
  peosPath = getPath();
  fail_unless (peosPath != NULL,
              "Unable to locate peos executable. ");

  /* Construct peos command */
  size = strlen((char *) "peosPath") + strlen((char *) "./peos -c Test.pml") + 1;
  cmd_peos = (char *) malloc((size_t) size);

  /* Add NULL character */
  str_rtn = strcpy((char *) cmd_peos, (char *) peosPath);
  free(peosPath);
  peosPath = NULL;
  str_rtn = strcat((char *) cmd_peos, (char *) "./peos -c Test.pml");
  
  /* Construct commnand argument for runPeos((char *) str) */
  size = strlen((char *) cmd_peos) + sizeof((char *) " >message 2>&1") + 1;
  command = (char *) malloc((size_t) size);
  str_rtn = strcpy((char *) command, (char *) cmd_peos);
  str_rtn = strcat((char *) command, (char *) " >message 2>&1");
  free(cmd_peos);
  cmd_peos = NULL;

  rtn_val = runPeos((char *) command);
  free(command);
  command = NULL;
   
  fail_unless (rtn_val == EXIT_SUCCESS,
              "The trunPeos test failed to return 0.");
  
  /* A different method similar to above accept more extensive use for
   * conditional statements.
   *   if () {
   *           fail  ("");
   *   }
   */
#endif   
}
END_TEST

START_TEST(tpeos_in_dir)
{

  #define INVALID_DIR ")"    /* invalid directory name */
  #define BIN "/bin"         /* contains peos executable */
  char *rtn_val = NULL, *testPeosdir = NULL, *str_rtn = NULL, *dirname = NULL;
  size_t size = 0;
  signed cmp_val;

  rtn_val = get_current_dir_name();
  if(rtn_val == NULL) {
  	fail("Error. Need to set PWD correctly.");	
  }
  size = strlen((char *) rtn_val) + strlen((char *) BIN) + 1;
  dirname = (char *) malloc((size_t) size);
  strcpy((char *) dirname, (char *) rtn_val);
  free(rtn_val);
  rtn_val = NULL;
  strcat((char *) dirname, (char *) BIN);
  
  /* pass to peos_in_dir(char *) str) a NULL value */
  rtn_val = (char *) peos_in_dir((char *) NULL);
  size = 1;
  if(rtn_val != NULL) {
	fail("Error. Return value must be NULL if testPeosdir is NULL.");
  }

  /* pass dirname argument for peos_in_dir((char *) str) */
  rtn_val = (char *) peos_in_dir((char *) dirname);
  free(dirname);
  dirname = NULL;
#ifdef DEBUG  
  if(rtn_val == NULL) {
	fail("Error. Return value must be NULL if testPeosdir is NULL.");
  }
#endif  
  free(rtn_val);
  rtn_val = NULL;
 
  /* Construct an invlaid dir */
  size = strlen((char *) INVALID_DIR) + 1;
  testPeosdir = (char *) malloc((size_t) size);
  size = 0;
  str_rtn = strcpy((char *) testPeosdir, (char *) INVALID_DIR);
     
  /* test against an invalid dir */
  rtn_val = (char *) peos_in_dir((char *) testPeosdir);
  free(testPeosdir);
  testPeosdir = NULL;
  if(rtn_val != NULL) {
  	fail("Error. With an invalid dir peos_in_dir((char *) str) should return NULL");
} 
  
  free(rtn_val);
  rtn_val = NULL;
  
}
END_TEST

START_TEST(tgetPath)
{
  
  char *rtn_val = NULL, *t = NULL;	/* t is current token */
  unsigned i = 0, tok_count = 0;      

  rtn_val = getPath();
  printf(" rtn_val = %s \n", rtn_val);
  if(rtn_val == NULL) {
  	fail("Error. Unable to locate peos executable.");
  }
  
  for(t = strtok((char *) rtn_val, (char *) "/"); t != NULL; t = strtok((char *) NULL, (char *) "/")) {
	tok_count++;
  	if(strcmp((char *) t, (char *) "bin") == 0) i = tok_count;
  }
  free(rtn_val);
  rtn_val = NULL;
  
  /* If tok_count-1 is greater than i that means bin is not the basename of the peos executable path.
   * I used tok_count-1 to exclude "peos" token.
   */ 
  if((i > 0) && (tok_count-1 > i)) {
  	fail("Error. Unable to locate peos in any bin directory.");
  }
  
}
END_TEST

Suite *
engine_suite (void) 
{ 
  Suite *s = suite_create ("engine_suite"); 

  /* Create test harnest for functions in module */
  TCase *tc = tcase_create ("test_RunPeos");
  TCase *tc1 = tcase_create ("test_Peos_in_dir");
  TCase *tc2 = tcase_create ("test_GetPath");

  suite_add_tcase (s, tc);
  suite_add_tcase (s, tc1);
  suite_add_tcase (s, tc2);

  tcase_add_test (tc, trunPeos); 
  tcase_add_test (tc1, tpeos_in_dir); 
  tcase_add_test (tc2, tgetPath); 

  return s; 
}

int 
main(int argc, char** argv)
{

  int nf; 
  Suite *s = engine_suite (); 
  SRunner *sr = srunner_create (s); 

  /* Run program in separate address space than PEOS. */
  //srunner_run_all(sr, fork_status);

  /* Run suite tests. */
  srunner_run_all (sr, CK_NORMAL); 
  nf = srunner_ntests_failed (sr); 
  srunner_free (sr); 
  //suite_free (s);
  sr = NULL;
  //s  = NULL;
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 

return 0;
}

