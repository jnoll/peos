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
#include "segments.h"

#include "TestDB.h"

extern void initialize_suite();
extern void terminate_suite();

char *j;

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

static void test_Int8() {
  Int8 i=0;
  Int8 j=-5;
  i--;

  ASSERT_INT8_EQUAL(i,j)
}
static void test_UInt8() {
  UInt8 i=0;
  UInt8 j=5;
  i--;

  ASSERT_UINT8_EQUAL(i,j)
}
static void test_Int16() {
  Int16 i=0;
  Int16 j=-5;
  i--;

  ASSERT_INT16_EQUAL(i,j)
}
static void test_UInt16() {
  UInt16 i=0;
  UInt16 j=5;
  i--;

  ASSERT_UINT16_EQUAL(i,j)
}
static void test_Int32() {
  Int32 i=0;
  Int32 j=-5;
  i--;

  ASSERT_INT32_EQUAL(i,j)
}
static void test_UInt32() {
  UInt32 i=0;
  UInt32 j=5;
  i--;

  ASSERT_UINT32_EQUAL(i,j)
}
static void test_String() {
  Char *i = "This";
  j = strdup(i);
  ASSERT_STR_EQUAL(i,j)
}

static void test_peos_list_models(){
	char ** ret;
	ret = peos_list_models(); 
	ASSERT_STR_EQUAL(ret[0],"yo");
}
static void test_List()
{
        List x = ListCreate();

        ASSERT_INT32_EQUAL(ListSize(x),0);
        ListDestroy(x);
}

static void test_Node()
{
	Node myNode;
	myNode = NodeCreate("Steve", 1,1);
	ASSERT_STR_EQUAL("Steve", myNode->name);
	ASSERT_INT16_EQUAL(1, myNode->type);
	ASSERT_INT32_EQUAL(1, myNode->line);	
	free(myNode->name);
	ListDestroy(myNode->predecessors);
	ListDestroy(myNode->successors);
	free(myNode);
}
/*-----------------------------*/
 
void initialize_suite() {
  TestGroup* group;

  group = add_test_group("CUnit", initializeCUnitGroup, terminateCUnitGroup, initializeCUnitTest, terminateCUnitTest);
  add_test_case(group, "Int8",   test_Int8);
  add_test_case(group, "UInt8",  test_UInt8);
  add_test_case(group, "Int16",  test_Int16);
  add_test_case(group, "UInt16", test_UInt16);
  add_test_case(group, "Int32",  test_Int32);
  add_test_case(group, "UInt32", test_UInt32);
  add_test_case(group, "String", test_String);
  add_test_case(group, "Node", test_Node);

}

void terminate_suite() {
}
