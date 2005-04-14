/*
 * TestSuite.c
 *
 *   counter_XXX, rcounter_XXX
 */

#include "TestDB.h"
#include <stdio.h>

#include <events.h>
#include <pml/list.h>
#include <pml/graph.h>
#include <pml/tree.h>
#include <pml/features.h>
#include "segments.h"
#include <action.h>
#include <resources.h>


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
	//ASSERT_INT16_EQUAL(5, dbc);
	ret=peos_list_models(); 
	if (ret == NULL)
		ASSERT_STR_EQUAL("yo", "YOU");
	ASSERT_STR_EQUAL("AddressDB", ret[0]);
}

static void test_load()
{
	UInt32 i;
	int num_resources=-1;
	peos_resource_t *resources;
	char *model;
	model = strdup("simple");
	resources = get_resource_list(model, &num_resources);
	ASSERT_STR_EQUAL(model, find_model_file(model));
	free(model);
	ASSERT_INT32_EQUAL(0, num_resources);
/*	for(i=0; i< num_resources; i++)
	{
		strcpy(resources[i].value, "$$");
	}*/
//	ASSERT_INT32_EQUAL(0, peos_run(model, resources, num_resources));
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
	Graph *g=makegraph("simple");
	ASSERT_MSG("G is null", g != NULL);

}
static void test_ListCreate()
{
        x = ListCreate();
        ASSERT_INT32_EQUAL(0,ListSize(x));
}

static void test_TreeCreate()
{
	//printf("Testing Tree Create-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
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
//	ListInsert(x, 0, "Steve");
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
/*-----------------------------*/
 
void initialize_suite() {
  TestGroup* group;

  group = add_test_group("CUnit", initializeCUnitGroup, terminateCUnitGroup, initializeCUnitTest, terminateCUnitTest);
//  add_test_case(group, "MakeGraph", test_makegraph);
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
}

void terminate_suite() {
}
