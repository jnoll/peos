/* XXX - unistd.h under RH9 fails to declare get_current_dir_name correctly */
extern char *get_current_dir_name(void);
#include <stdlib.h>	/* EXIT_SUCCESS EXIT_FAILURE */
#include <string.h>
#include <check.h>
#include "engine.h"
#include "glo.h"


START_TEST(trunPeos)
{
 
  int rtn_val = -1;
  size_t size = 0;
  char *command = NULL, *cmd_peos = NULL, *str_rtn = NULL;

  /* Construct peos command with peos exe in a known directory */
  size = strlen("/home/jshah1/bin/peos -c Test.pml") + 1;
  cmd_peos = (char *) malloc((size_t) size * sizeof(char));
  size = 0;

  strcpy(cmd_peos, "/home/jshah1/bin/peos -c Test.pml");
  
  /* Construct commnand argument for runPeos((char *) str) */
  size = strlen(cmd_peos) + strlen(" >message 2>&1") + 1;
  command = (char *) malloc((size_t) size * sizeof(char));
  
  strcpy(command, cmd_peos);
  strcat(command, " >message 2>&1");
  free(cmd_peos);
  cmd_peos = NULL;
  
  /* call runPeos((char *) str) with command */
  rtn_val = runPeos((char *) command);
  free(command);
  command = NULL;
   
  fail_unless (rtn_val == EXIT_SUCCESS,
              "The trunPeos test failed to return 0.");

  
}
END_TEST

START_TEST(tpeos_in_dir)
{

  #define INVALID_DIR ")"    /* invalid directory name */
  #define BIN "/bin"         /* contains peos executable */
  char *rtn_val = NULL, *testPeosdir = NULL, *str_rtn = NULL, *dirname = NULL;
  size_t size = 0, rtn = 0;
  signed cmp_val;
  
  /* check to see if pwd has a value */
  rtn_val = get_current_dir_name();
  if(rtn_val == NULL) {
  	fail("Error. Need to set PWD correctly. \n");	
	free(rtn_val);
	rtn_val = NULL;
  }	

  /* pass to peos_in_dir(char *) str) a NULL value */
  rtn_val = (char *) peos_in_dir((char *) NULL);
  if(rtn_val != NULL) {
	fail("Error. Return value must be NULL if parameter of peos_in_dir is NULL. \n");
  }
 
  /* construct a valid directory to pass to peos_in_dir(char * str) */
  size = strlen("/home/jshah1/bin") + 1;
  dirname = (char *) malloc((size_t) size * sizeof(char));
  size = 0;
  strcpy(dirname, "/home/jshah1/bin");
  
  /* pass a valid peos location dirname argument for peos_in_dir((char *) str) */
  rtn_val = (char *) peos_in_dir((char *) dirname);
  
  /* check to see if peos is in /home/jshah1/bin */
  rtn = strcmp(dirname, "/home/jshah1/bin/peos");
  if((rtn_val == NULL) && (rtn != 0)) {
	fail("Error. Peos executable is not in /home/jshah1/bin. \n");
  }
  
  free(rtn_val);
  rtn_val = NULL;
  free(dirname);
  dirname = NULL;
 
  /* Construct an invlaid dir */
  size = strlen(INVALID_DIR) + 1;
  dirname = (char *) malloc((size_t) size * sizeof(char));
  size = 0;
  str_rtn = strcpy(dirname, INVALID_DIR);
     
  /* test against an invalid dir */
  rtn_val = (char *) peos_in_dir((char *) dirname);
  free(dirname);
  dirname = NULL;

  if(rtn_val != NULL) {
  	fail("Error. With an invalid directory peos_in_dir((char *) str) should return NULL");
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
  
  if(rtn_val == NULL) {
  	fail("Error. Unable to locate peos executable.");
  }
  
  for(t = strtok(rtn_val, "/"); t != NULL; t = strtok(NULL, "/")) {
	tok_count++;
  	if(strcmp(t, "bin") == 0) i = tok_count;
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

