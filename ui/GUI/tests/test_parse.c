#include <libxml/parser.h>
#include <libxml/tree.h>
#include <check.h>
#include "../parse.h"
#include "../glo.h"


START_TEST(tParse)
{

  int rtn_val = -1, i = -1, count = 0, j = 0, k = 0;
  xmlNode *root_element = NULL, *cur = NULL;
  
  /* if DOM structure exists then free it for parse test */
  if(doc != NULL) {
  	xmlFreeDoc(doc);
	doc = NULL;
  }	
  
  for(i = 0; i < MAX_PID; i++) {
  	if(table[i].process == NULL) count = i;
  }

  rtn_val = parse();
  if(rtn_val == -1) {
  	fail("Abnormal call to peos() or peos() exited abnormally. \n");
  }

  /* test to see if DOM structure exists */
  if(doc == NULL) {
  	fail("DOM structure not created. \n");
  }

  /* at this point the root element of the DOM tree exists 
   * so did the call to walk(root_element) respond correctly
   */
  if(rtn_val == 1) {
  	fail("The call to walk(xmlNod *root) exited abnormally. \n");
  }
  
  /* check if the process' are NULL terminated */
  for(i = 0; i < MAX_PID; i++) {
  	if(table[i].process == NULL) count = i;
  }
  
  /* i-1 because i is incremented one more time to test condition in loop */
  if(count != (i - 1)) {
  	fail("All the process' must be set to NULL. \n");
  }

  /* since our Test.pml specifies process elements, then did the 
   * process data structure get set properly by the call to 
   * walk(xmlNode *root)
   */ 
  i = -1; 
  root_element = xmlDocGetRootElement(doc);
  xmlNode *curAction = NULL;
  action *procAction = NULL;
  
  /* traversing through the treee one time to see if the conditions pass */
  for(cur = root_element->children; cur != NULL; cur = cur->next) {
  	if((cur->type == XML_ELEMENT_NODE) && (strcmp(cur->name, (const xmlChar *) "process") == 0)) {
		i = atoi(xmlGetProp(cur, "pid"));
#ifdef DEBUG		
printf("table[i].process = %u \n", table[i].process);		
#endif
	
		if(table[i].process == NULL) {
			j = -1;
		}	
		
		/* check to see that walkaction set data structure correctly 
		 * for every cur element
		 */
		procAction = &(table[i].list);
		for(curAction = cur->children; curAction != NULL; curAction = curAction->next){
			if((curAction->type = XML_ELEMENT_NODE) && (xmlStrcmp(curAction->name, (const xmlChar *) "action") == 0)) {
#ifdef DEBUG			
printf("in action branch j = %d \n", j);			
printf("procAction = %u \n", procAction);
printf("curAction = %u \n", curAction);
printf("procAction->action[%d] = %u \n", k, procAction->action[k]);
#endif
				/* must stop at k == procAction->count b/c that 
				 * is how long the walkaction iterated it's 
				 * index
				 */
				if((procAction->action[k] == NULL) && (k < procAction->count)){
					j = -1;
				}
				k++;
			} else if((curAction->type == XML_ELEMENT_NODE) && (xmlStrcmp(curAction->name, (const xmlChar *) "iteration") == 0)) {
				if((procAction->action[k] == NULL) && (k < procAction->count)){
					j = -1;
				}
			
				if(curAction->children != NULL) {
					if((procAction->iterptr == NULL) && (k < procAction->count)){
						j = -1;
					}
				}
				k++;
			} else if ((curAction->type == XML_ELEMENT_NODE) && (xmlStrcmp(curAction->name, (const xmlChar *) "selection") == 0)) {
				if((procAction->action[k] == NULL) && (k < procAction->count)) {
					j = -1;
				}
				if(curAction->children != NULL) {

					if((procAction->iterptr == NULL) && (k < procAction->count)) {
						j = -1;
					}
				}
				k++;
			} else if ((curAction->type == XML_ELEMENT_NODE) && (xmlStrcmp(curAction->name, (const xmlChar *) "branch") == 0)) {
				if((procAction->action[k] == NULL) && (k < procAction->count)) {
					j = -1;
				}
				if(curAction->children != NULL) {
					if((procAction->iterptr == NULL) && (k < procAction->count)) {
						j = -1;
					}
				}
				k++;
			} else if((curAction->type == XML_ELEMENT_NODE) && (xmlStrcmp(curAction->name, (const xmlChar *) "sequence") == 0)) {
				if(curAction->children != NULL) {

					if((procAction->action[k] == NULL) && (procAction->iterptr == NULL) && (k < procAction->count)) {
						j = -1;
					}
					if((curAction->type == XML_ELEMENT_NODE)&& (xmlStrcmp(curAction->name, (const xmlChar *) "action") == 0)) {
						if((procAction->action[k] == NULL) && (k < procAction->count)) {
							j = -1;
						}	
						k++;	
					}
				}
			}
		}
	}
  }

  /* make sure that the xml file does not display pid's that are out of the
   * range of 0-10 or else walk(xmlNode *root) will construct an invalid 
   * data structure 
   */
  if((i < 0) || (i > 10)) {
  	fail("A process must exist and the amount of processes cannot exceed 11.  \n");
  }
  
  /* the Test.pml file contains process' so i should be greater than -1 */
  if(j == -1) {
  	fail("Either xml file not set properly or call to walk(xmlNode *root) is not correct. \n");
  }
  
}
END_TEST

Suite *
parse_suite (void) 
{ 
  Suite *s = suite_create ("parse_suite"); 

  /* Create test harnest for functions in module */
  TCase *tc = tcase_create ("tParse");

  suite_add_tcase (s, tc);

  tcase_add_test (tc, tParse); 

  return s; 
}

int 
main(int argc, char** argv)
{

  int nf; 
  Suite *s = parse_suite (); 
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

