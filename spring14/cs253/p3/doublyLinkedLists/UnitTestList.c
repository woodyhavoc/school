/*
 * UnitTestList.c
 *
 *      Author: marissa, awoods
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "common.h"
#include "Job.h"
#include "Node.h"
#include "List.h"

/* 
 * macro to mimic the functionality of assert() from <assert.h>. The difference is that this version doesn't exit the program entirely.
 * It will just break out of the current function (or test case in this context).
 */
#define myassert(expr) if(!(expr)){ fprintf(stderr, "\t[assertion failed] %s: %s\n", __PRETTY_FUNCTION__, __STRING(expr)); return FALSE; }

ListPtr testlist;

int testCount = 0;
int passCount = 0;

/**
 * Prints the name and execution state of the test currently
 * being run.
 *
 * @param testName the name of the test currently running
 * @param info the execution state of the current test
 */
void printTestInfo(char* testName, char *info)
{
	fprintf(stdout, "%s - %s\n", testName, info);
}

/**
 * Prints the results of the test execution.
 *
 * @param testName the name of the test which just finished running
 * @param passed contains the value of the execution result, 1 if passed, 0 if failed
 */
void printTestResult(char* testName, Boolean passed)
{
	if(passed == TRUE)
		fprintf(stdout, "%s - %s\n\n", "[PASSED]", testName);
	else
		fprintf(stdout, "%s - %s\n\n", "[FAILED]", testName);
}

/**
 * Accepts a integer value as a job pointer key value,
 * creates a node using this value and a common string
 *
 * @param jobid value to be assigned to the <code>JobPtr</code>
 *
 * @return newly created <code>NodePtr</code>
 */
NodePtr createTestNode(int jobid)
{
	JobPtr job = createJob(jobid, "cmd args");
	NodePtr node = createNode(job);
	return node;
}

/**
 * Tests the list to ensure the <code>addAtFront()</code> function
 * works correctly when the list is currently empty.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean addAtFrontWithNoNodes()
{
	NodePtr node = createTestNode(1);

	addAtFront(testlist, node);

	assert(getSize(testlist) == 1);
	assert(testlist->head == node);
    assert(node->next == node);
	assert(node->prev == node);

	return TRUE;
}

/**
 * Tests the list to ensure the <code>addAtFront()</code> function
 * works correctly when the list has one node in it already.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean addAtFrontWithOneNode()
{
    NodePtr node0 = createTestNode(0);
    addAtFront(testlist, node0);
    
    assert(getSize(testlist) == 1);
    assert(testlist->head == node0);
    assert(testlist->head->next == testlist->head);
    assert(testlist->head->prev == testlist->head);

    NodePtr node1 = createTestNode(1);
    addAtFront(testlist, node1);
   
    assert(getSize(testlist) == 2);
    assert(testlist->head == node1);
    assert(testlist->head->next == node0);
    assert(testlist->head->prev == node0);
	
    return TRUE;
}

/**
 * Tests list to ensure the <code>addAtRear()</code> function
 * works correctly when the list is currently empty.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean addAtRearWithNoNodes()
{
	NodePtr node = createTestNode(1);
	addAtRear(testlist, node);

	assert(getSize(testlist) == 1);
	assert(testlist->head == node);
	assert(node->next == node);
	assert(node->prev == node);

	return TRUE;
}

/**
 * Tests list to ensure the <code>addAtRear()</code> function
 * works correctly when the list has one node already.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean addAtRearWithOneNode()
{
	NodePtr node0 = createTestNode(0);
    addAtRear(testlist, node0);
    
    assert(getSize(testlist) == 1);
    assert(testlist->head == node0);
    assert(node0->next == node0);
    assert(node0->prev == node0);

    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
   
    assert(getSize(testlist) == 2);
    assert(node1->next == node0);
    assert(node1->prev == node0);

	return TRUE;
}

/**
 * Tests the functions <code>removeFront(), removeRear(), removeNode()</code>
 * when the list has one node already.
 *
 * @return <code>TRUE</code> if tests are successful, <code>FALSE</code> otherwise
 */
Boolean removeFromListWithOneNode()
{
	NodePtr node0 = createTestNode(0);
	addAtFront(testlist, node0);
	
	removeFront(testlist);
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);

	NodePtr node1 = createTestNode(1);
	addAtFront(testlist, node1);

	removeRear(testlist);
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);

	NodePtr node2 = createTestNode(2);
	addAtFront(testlist, node2);

	removeNode(testlist, node2);
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	
	freeNode(node0);
	freeNode(node1);
	freeNode(node2);

	return TRUE;
}

/**
 * Tests adding a <code>NULL NodePtr</code> at the front
 * and rear of the list.  The list should not be modified.
 *
 * @return <code>TRUE</code> if tests are successful, <code>FALSE</code> otherwise
 */
Boolean nullNodeTest()
{
	NodePtr nullNode = NULL;
	
	addAtFront(testlist, nullNode);
	
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	
	addAtRear(testlist, nullNode);
	
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	
	NodePtr nullNode2 = removeNode(testlist, nullNode);
	assert(nullNode2 == NULL);
		
	return TRUE;
}

/**
 * Passes the parameter to <code>printTestInfo()</code>,
 * initializes <code>testlist</code>, and increments
 * the count of total tests by 1.
 *
 * @param testName the name of the test to be ran next (passed to <code>printTestInfo()</code>)
 */
void beforeTest(char* testName)
{
	printTestInfo(testName, "Running...");
	testlist = createList();
	testCount++;
}

/**
 * Passes parameters to <code>printTestResults()</code>,
 * frees the memory allocated to the list,
 * and increments the number of passed tests by the value
 * of the second parameter.
 *
 * @param testName name of the test which most recently finished running 
 * 		  (passed to <code>printTestResult()</code>)
 * @param result increments value of the count of passed tests by 1 if test was successful,
 *        0 if failure (passed to <code>printTestResult</code>)
 */
void afterTest(char* testName, Boolean result)
{
	printTestResult(testName, result);
	freeList(testlist);
	passCount += result;
}

/**
 * Tests list to ensure no changes are made when
 * a <code>NULL NodePtr</code> is added at the front of the list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean addAtFrontWithNullNode()
{
	addAtFront(testlist, NULL);

	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);

	return TRUE;
}

/**
 * Tests list to ensure <code>addAtFront()</code> function does nothing
 * when a null list is given.
 *
 * @return <code>TRUE</code> if test passes, <code>FALSE</code> otherwise
 */
Boolean addAtFrontWithNullList()
{
	NodePtr node = createTestNode(1);

	addAtFront(NULL, node);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code>
 * is returned when <code>removeFront()</code> is called
 * with a <code>NULL</code> list.
 *
 * @return <code>TRUE</code> if test passes, <code>FALSE</code> otherwise
 */
Boolean removeFromFrontWithNullList()
{
	NodePtr node = removeFront(NULL);

	assert(node == NULL);

	return TRUE;
}

/**
 * Tests to ensure a <code>NULL NodePtr</code> is returned
 * when <code>removeFront()</code> is called with an empty list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeFromFrontWithEmptyList()
{
	NodePtr node = removeFront(testlist);

	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	assert(node == NULL);

	return TRUE;
}

/**
 * Tests to ensure <code>removeFront()</code> works when
 * the list has one node.  The function should return the
 * only node inside the list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeFromFrontWithOneNode()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr nodeTest = removeFront(testlist);
	
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	assert(nodeTest == node);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure a <code>NULL NodePtr</code> is returned
 * when <code>removeRear()</code> is called with a
 * <code>NULL</code> list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeFromRearWithNullList()
{
	NodePtr node = removeRear(NULL);

	assert(node == NULL);

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> is returned
 * when <code>removeRear()</code> is called with an empty list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeFromRearWithEmptyList()
{
	NodePtr node = removeRear(testlist);

	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	assert(node == NULL);

	return TRUE;
}

/**
 * Tests to ensure <code>removeRear()</code> returns the only
 * node when <code>list->size == 1</code>.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeFromRearWithOneNode()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr nodeTest = removeRear(testlist);

	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	assert(nodeTest == node);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure a <code>NULL NodePtr</code> is returned
 * when <code>removeNode()</code> is called with a
 * <code>NULL</code> list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeNodeWithNullList()
{
	NodePtr node = createTestNode(1);

	NodePtr nodeTest = removeNode(NULL, node);

	assert(nodeTest == NULL);

	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure a <code>NULL NodePtr</code> is returned
 * when <code>removeNode()</code> is called with an empty list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeNodeWithEmptyList()
{
	NodePtr node = createTestNode(1);

	NodePtr nodeTest = removeNode(testlist, node);

	assert(nodeTest == NULL);

	freeNode(node);
	
	return TRUE;
}

/**
 * Tests to ensure <code>removeNode()</code> returns a
 * valid <code>NodePtr</code> when <code>list->size == 1</code>.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeNodeWithOneNode()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr nodeTest = removeNode(testlist, node);
	
	assert(getSize(testlist) == 0);
	assert(testlist->head == NULL);
	assert(nodeTest == node);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure a <code>NULL NodePtr</code> is returned
 * when <code>removeNode()</code> is called with a
 * <code>NULL</code> node.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean removeNodeWithNullNode()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr nodeTest = removeNode(testlist, NULL);
	
	assert(nodeTest == NULL);
	
	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> will be returned
 * when <code>removeNode()</code> is called with a node that is
 * not in the list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise 
 */
Boolean removeNodeNotInList()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr node1 = createTestNode(2);

	NodePtr nodeTest = removeNode(testlist, node1);

	assert(nodeTest == NULL);

	freeNode(node1);

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> is returned
 * when a <code>NULL</code> list is searched.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchWithNullList()
{
	NodePtr node = createTestNode(1);

	NodePtr nodeTest = search(NULL, node->data->jobid);

	assert(nodeTest == NULL);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> is returned
 * when an empty list is searched.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchWithEmptyList()
{
	NodePtr node = createTestNode(1);

	NodePtr nodeTest = search(testlist, node->data->jobid);
	
	assert(nodeTest == NULL);
	
	freeNode(node);

	return TRUE;
}

/**
 * Tests to ensure that a valid <code>NodePtr</code> is returned
 * when a list of size 1 is searched, and it contains the node
 * being searched for.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchOneNodeInList()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr nodeTest = search(testlist, node->data->jobid);

	assert(nodeTest == node);

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> is returned
 * when a list of size 1 is searched, but does not contain the
 * node being searched for.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchOneNodeNotInList()
{
	NodePtr node = createTestNode(1);
	addAtFront(testlist, node);

	NodePtr node1 = createTestNode(2);

	NodePtr nodeTest = search(testlist, node1->data->jobid);

	assert(nodeTest == NULL);

	freeNode(node1);

	return TRUE;
}

/**
 * Tests to ensure that a valid <code>NodePtr</code> is returned
 * when a list with more than 1 element is searched, and contains
 * the node being searched for.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchMulNodesInList()
{
	int i;
	for(i = 0; i < 10; i++)
	{
		NodePtr node = createTestNode(i);
		addAtFront(testlist, node);
		NodePtr nodeTest = search(testlist, node->data->jobid);
		assert(nodeTest == node);
	}

	return TRUE;
}

/**
 * Tests to ensure that a <code>NULL NodePtr</code> is returned
 * when a list with more than 1 element is searched, and the node
 * being searched for is not contained in the list.
 *
 * @return <code>TRUE</code> if test is successful, <code>FALSE</code> otherwise
 */
Boolean searchMulNodesNotInList()
{
	int i;
	for(i = 0; i < 10; i++)
	{
		NodePtr node = createTestNode(i);
		addAtFront(testlist, node);
	}

	for(i = 10; i < 20; i++)
	{
		NodePtr node = createTestNode(i);
		NodePtr nodeTest = search(testlist, node->data->jobid);
		assert(nodeTest == NULL);
		freeNode(node);
	}

	return TRUE;
}

/**
 * Tests to ensure no errors are generated when
 * <code>reverseList()</code> is called with a
 * <code>NULL</code> list.
 *
 * @return <code>TRUE</code> if no errors generated
 */
Boolean reverseListNullList()
{
	reverseList(NULL);
	return TRUE;
}

/**
 * Tests to ensure no errors are generated when
 * <code>printList()</code> is called with a
 * <code>NULL</code> list.
 *
 * @return <code>TRUE</code> if no errors are generated
 */
Boolean printListNullList()
{
	printList(NULL);
	return TRUE;
}

/**
 * Runs all of the predefined Unit Tests, and prints the statistics.
 */
void runUnitTests()
{
	Boolean result;
	char *testName;

	testName = "addAtFrontWithNoNodes";
	beforeTest(testName);
	result = addAtFrontWithNoNodes();
	afterTest(testName, result);

	testName = "addAtFrontWithOneNode";
	beforeTest(testName);
	result = addAtFrontWithOneNode();
	afterTest(testName, result);

	testName = "addAtRearWithNoNodes";
	beforeTest(testName);
	result = addAtRearWithNoNodes();
	afterTest(testName, result);

	testName = "addAtRearWithOneNode";
	beforeTest(testName);
	result = addAtRearWithOneNode();
	afterTest(testName, result);

	testName = "removeFromListWithOneNode";
	beforeTest(testName);
	result = removeFromListWithOneNode();
	afterTest(testName, result);

	testName = "nullNodeTest";
	beforeTest(testName);
	result = nullNodeTest();
	afterTest(testName, result);
	
	testName = "addAtFrontWithNullNode";
	beforeTest(testName);
	result = addAtFrontWithNullNode();
	afterTest(testName, result);

	testName = "addAtFrontWithNullList";
	beforeTest(testName);
	result = addAtFrontWithNullList();
	afterTest(testName, result);
	
	testName = "removeFromFrontWithNullList";
	beforeTest(testName);
	result = removeFromFrontWithNullList();
	afterTest(testName, result);

	testName = "removeFromFrontWithEmptyList";
	beforeTest(testName);
	result = removeFromFrontWithEmptyList();
	afterTest(testName, result);

	testName = "removeFromFrontWithOneNode";
	beforeTest(testName);
	result = removeFromFrontWithOneNode();
	afterTest(testName, result);

	testName = "removeFromRearWithNullList";
	beforeTest(testName);
	result = removeFromRearWithNullList();
	afterTest(testName, result);

	testName = "removeFromRearWithEmptyList";
	beforeTest(testName);
	result = removeFromRearWithEmptyList();
	afterTest(testName, result);

	testName = "removeFromRearWithOneNode";
	beforeTest(testName);
	result = removeFromRearWithOneNode();
	afterTest(testName, result);

	testName = "removeNodeWithNullList";
	beforeTest(testName);
	result = removeNodeWithNullList();
	afterTest(testName, result);
	
	testName = "removeNodeWithEmptyList";
	beforeTest(testName);
	result = removeNodeWithEmptyList();
	afterTest(testName, result);
		
	testName = "removeNodeWithOneNode";
	beforeTest(testName);
	result = removeNodeWithOneNode();
	afterTest(testName, result);
		
	testName = "removeNodeWithNullNode";
	beforeTest(testName);
	result = removeNodeWithNullNode();
	afterTest(testName, result);
		
	testName = "removeNodeNotInList";
	beforeTest(testName);
	result = removeNodeNotInList();
	afterTest(testName, result);
		
	testName = "searchWithNullList";
	beforeTest(testName);
	result = searchWithNullList();
	afterTest(testName, result);
		
	testName = "searchWithEmptyList";
	beforeTest(testName);
	result = searchWithEmptyList();
	afterTest(testName, result);
		
	testName = "searchOneNodeInList";
	beforeTest(testName);
	result = searchOneNodeInList();
	afterTest(testName, result);
		
	testName = "searchOneNodeNotInList";
	beforeTest(testName);
	result = searchOneNodeNotInList();
	afterTest(testName, result);
		
	testName = "searchMulNodesInList";
	beforeTest(testName);
	result = searchMulNodesInList();
	afterTest(testName, result);
		
	testName = "searchMulNodesNotInList";
	beforeTest(testName);
	result = searchMulNodesNotInList();
	afterTest(testName, result);
		
	testName = "reverseListNullList";
	beforeTest(testName);
	result = reverseListNullList();
	afterTest(testName, result);
		
	testName = "printListNullList";
	beforeTest(testName);
	result = printListNullList();
	afterTest(testName, result);
	
	fprintf(stdout, "Test Cases: %d\n",  testCount);
	fprintf(stdout, "Passed: %d\n", passCount);
	fprintf(stdout, "Failed: %d\n", testCount - passCount);
}

/**
 * Main function, calls <code>runUnitTests()</code>
 *
 * @param argc number of command line arguments, not used
 * @param argv command line argument vector, not used
 */
int main(int argc, char *argv[])
{
	runUnitTests();
	exit(0);
}
