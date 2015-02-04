/*
 * TestSuite.c
 *
 *   counter_XXX, rcounter_XXX
 */

#include "TestDB.h"
#include <stdio.h>

#include <events.h>
#include <pml/features.h>
#include <pml/list.h>
#include <pml/graph.h>
#include <pml/tree.h>
#include "segments.h"

#include <action.h>
#include <resources.h>
#include <graph.h>

extern void initialize_suite();
extern void terminate_suite();

char *j;
char *ark, *steve;
Tree peos;
List x; //global needed for List tests

/*-----------------------------*/

//struct counter* c;
 int c;

static int initializeCUnitGroup() {
  return 0;
}

static int terminateCUnitGroup() {
  free(j);
  return 0;
}

static int initializeCUnitTest() {
  return 0;
}

static int terminateCUnitTest() {
  return 0;
}

/*
  ASSERT_INT8_EQUAL(i,j)
  ASSERT_UINT8_EQUAL(i,j)
  ASSERT_INT16_EQUAL(i,j)
  ASSERT_UINT16_EQUAL(i,j)
  ASSERT_INT32_EQUAL(i,j)
  ASSERT_UINT32_EQUAL(i,j)
*/

static void test_Strdup() {
  Char *i = "This";
  j = strdup(i);
  ASSERT_STR_EQUAL(i,j)
}

static void test_peos_list_models(){
	char ** ret;
	Int16 dbc;
	dbc = DmNumDatabases(0);
	ret=peos_list_models(); 
	ASSERT_MSG("peos_list_models() returns null", ret!=NULL);
	
	ASSERT_STR_EQUAL("simple", ret[0]);
}

static void test_load()
{
	char **processes;
	char *model;
	peos_resource_t *resource;
	int x;
	int num_resources=-1;
	model = strdup("timesheet");
	ASSERT_STR_EQUAL(model, find_model_file(model));
	processes=peos_list_instances();
	ASSERT_STR_EQUAL("", processes[0]);

	resource=get_resource_list(model, &num_resources); 
	ASSERT_INT32_EQUAL(1, num_resources);

	load_proc_table();

	x = peos_create_instance(model, resource, num_resources);
	ASSERT_INT32_EQUAL(0, x);
	x = peos_create_instance(model, resource, num_resources);
	ASSERT_INT32_EQUAL(1, x);
	processes=peos_list_instances();
	ASSERT_STR_EQUAL("timesheet", processes[0]);
	ASSERT_STR_EQUAL("timesheet", processes[1]);
	free(model);

}

static void test_Delete()
{
	int i;
	char **processes;

	processes=peos_list_instances();
	ASSERT_STR_EQUAL("timesheet", processes[1]);
	peos_delete_process_instance(1);
	processes=peos_list_instances();
	ASSERT_STR_EQUAL("", processes[1]);
}


static void test_fread()
{
	char *output=malloc(10);
	FILE *in;
	int i;
	in=fopen("timesheet", "r");
	fread(output, 1, 7, in);
	output[7]='\0';
	ASSERT_STR_EQUAL("process", output);
}
static void test_makegraph()
{
	Graph g=makegraph("simple");
	ASSERT_MSG("G is null", g != NULL);

}
static void test_ListCreate()
{
        x = ListCreate();
        ASSERT_INT32_EQUAL(0,ListSize(x));
}

static void test_TreeCreate()
{
	peos = TreeCreate(TreeCreate(NULL,NULL,"Steve",1),
			TreeCreate(NULL, NULL, "Ark",2),
				"John",0);
	ASSERT_STR_EQUAL("John", peos->sval);
	ASSERT_STR_EQUAL("Steve", peos->left->sval);
	ASSERT_STR_EQUAL("Ark", peos->right->sval);
}

static void test_TreeDelete()
{
	TreeDestroy(peos);
}
static void test_ListAdd()
{
	steve = "Steve";
	ASSERT_STR_EQUAL("Steve",ListPush(x,steve));
	ASSERT_STR_EQUAL("Steve", (char *) ListGet(x));
}

static void test_ListConcat()
{
	List z;
        List y = ListCreate();
	ark = "Ark";
	ListPush(y,ark);
        ASSERT_INT32_EQUAL(1,ListSize(y));
	z=ListConcat(x,y);
        ASSERT_INT32_EQUAL(2,ListSize(x));
        ListDestroy(x);
	ListDestroy(y);
	ListDestroy(z);
}

static void test_Node()
{
	Node myNode;
	myNode = NodeCreate("Steve", 1,1);
	ASSERT_STR_EQUAL("Steve", myNode->name);
	ASSERT_INT16_EQUAL(1, myNode->type);
	ASSERT_INT32_EQUAL(1, myNode->line);	
}

static void test_ListActions()
{
	int num_actions;
	peos_action_t *actions;

	actions = peos_list_actions(0, &num_actions);
	ASSERT_INT32_EQUAL(5, num_actions);
	ASSERT_STR_EQUAL("Fill_name", actions[0].name);
	ASSERT_STR_EQUAL("Fill_hours", actions[1].name);
	ASSERT_STR_EQUAL("Fill_total_hours", actions[2].name);
	ASSERT_STR_EQUAL("Sign_and_date", actions[3].name);
	ASSERT_STR_EQUAL("Turn_it_in", actions[4].name);
	ASSERT_STR_EQUAL("\"Fill in total hours\"", actions[2].script);
	
}

static void test_PeosNotify()
{
	int num_actions;
	peos_action_t *actions;

	actions = peos_list_actions(0, &num_actions);
	ASSERT_INT32_EQUAL(5, num_actions);
	ASSERT_MSG("State not currently in ready state.",get_act_state("Fill_name", actions, num_actions) == ACT_READY);
	peos_notify(0, "Fill_name", PEOS_EVENT_FINISH);
	actions = peos_list_actions(0, &num_actions);
	ASSERT_MSG("State not currently in done state.",get_act_state("Fill_name", actions, num_actions) == ACT_DONE);
	
}
/*-----------------------------*/
 
void initialize_suite() {
  TestGroup* group;

  group = add_test_group("CUnit", initializeCUnitGroup, terminateCUnitGroup, initializeCUnitTest, terminateCUnitTest);
  add_test_case(group, "strdup", test_Strdup);
  add_test_case(group, "ListCreate", test_ListCreate);
  add_test_case(group, "ListAdd", test_ListAdd);
  add_test_case(group, "Node", test_Node);
  add_test_case(group, "TreeCreate", test_TreeCreate);
  add_test_case(group, "TreeDelete", test_TreeDelete);
  add_test_case(group, "ListModels", test_peos_list_models);
  add_test_case(group, "FileRead", test_fread);
  add_test_case(group, "GraphCreate", test_makegraph);
  add_test_case(group, "LoadProcess", test_load);
  add_test_case(group, "LoadProcess", test_load);
  add_test_case(group, "ListActions", test_ListActions);
  add_test_case(group, "DeleteInstance", test_Delete);
  add_test_case(group, "PeosNotify", test_PeosNotify);
}

void terminate_suite() {
}
