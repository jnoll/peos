/*
 * TestSuite.c
 *
 *   counter_XXX, rcounter_XXX
 */

//#include "counter.h"
#include <stdio.h>
#include <events.h>
#include <pml/list.h>
#include <pml/graph.h>
#include <pml/tree.h>
#include "segments.h"

#include "TestDB.h"

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
	//ret = peos_list_models(); 
	ASSERT_STR_EQUAL(ret[0],"yo");
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
  add_test_case(group, "strdup", test_Strdup);
  add_test_case(group, "ListCreate", test_ListCreate);
  add_test_case(group, "ListAdd", test_ListAdd);
  add_test_case(group, "Node", test_Node);
  add_test_case(group, "TreeCreate", test_TreeCreate);
  add_test_case(group, "TreeDelete", test_TreeDelete);
}

void terminate_suite() {
}
