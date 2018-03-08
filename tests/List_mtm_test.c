#include "../list_mtm/list_mtm.h"
#include "test_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CREATE_LIST1	List list1 = listCreate(copyInt, freeint);
#define COPY_LIST1	List list_copy = listCopy(list1);
#define ADD_VALUES_TO_LIST 		int arr[10] = { 9,8,7,6,5,4,3,2,1 };\
listInsertFirst(list1, arr);\
listInsertLast(list1, (arr + 1));\
listInsertLast(list1, (arr + 2));\
listInsertLast(list1, (arr + 3));\
listInsertLast(list1, (arr + 4));\
listInsertLast(list1, (arr + 5));\
listInsertLast(list1, (arr + 6));\
listInsertLast(list1, (arr + 7));\
listInsertLast(list1, (arr + 8));\
listInsertLast(list1, (arr + 9));
#define NEW_VALUES_TO_LIST int arr2[10] = { 0,4,8,7,5,3,1,2,9,6 };\
listInsertFirst(list1, arr2);\
listInsertLast(list1, (arr2 + 1));\
listInsertLast(list1, (arr2 + 2));\
listInsertLast(list1, (arr2 + 3));\
listInsertLast(list1, (arr2 + 4));\
listInsertLast(list1, (arr2 + 5));\
listInsertLast(list1, (arr2 + 6));\
listInsertLast(list1, (arr2 + 7));\
listInsertLast(list1, (arr2 + 8));\
listInsertLast(list1, (arr2 + 9));

void* copyInt(ListElement int1) {
	int* int2 = malloc(sizeof(int));
	*int2 = *((int*)int1);
	return int2;
}
void freeint(ListElement int1) {
	free(int1);
	return;
}

int compareInt(ListElement int1, ListElement int2, ListFilterKey key) {
	if (*(int*)key == 0) return -1;
	int x = ((*((int*)int1)) - (*((int*)int2)));
	return x;
}

bool filterInt(ListElement int1, ListFilterKey key) {
	if ((*(int*)int1) < (*(int*)key)) {
		return true;
	}
	return false;
}


static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL, freeint) == NULL);
	ASSERT_TEST(listCreate(copyInt, NULL) == NULL);
	CREATE_LIST1
	ASSERT_TEST(list1 != NULL);
	ASSERT_TEST(listGetCurrent(list1) == NULL);
	listDestroy(list1);
	return true;
}

static bool testListCopy() {
	CREATE_LIST1
	listGetNext(list1);
	COPY_LIST1
	ASSERT_TEST(list_copy != NULL);
	ASSERT_TEST(listGetCurrent(list1) == listGetCurrent(list_copy));
	ASSERT_TEST(listCopy(NULL) == NULL);
	listDestroy(list1);
	listDestroy(list_copy);
	return true;
}

static bool testListGetSize() {
	CREATE_LIST1
	ASSERT_TEST(listGetSize(list1) == 0);
	ADD_VALUES_TO_LIST
	ASSERT_TEST(listGetSize(list1) == 10);
	ASSERT_TEST(listGetSize(NULL) == -1);
	listDestroy(list1);
	return true;
}

static bool testListGetFirst() {
	CREATE_LIST1
	ASSERT_TEST(listGetFirst(list1) == NULL);
	ASSERT_TEST(listGetFirst(NULL) == NULL);
	ADD_VALUES_TO_LIST
	ASSERT_TEST((*(int*)listGetFirst(list1)) == 9);
	listDestroy(list1);
	return true;
}
static bool testlistGetNext() {
	CREATE_LIST1
	ASSERT_TEST(listGetNext(list1) == NULL);
	ASSERT_TEST(listGetNext(NULL) == NULL);
	ADD_VALUES_TO_LIST
	ASSERT_TEST((*(int*)listGetFirst(list1) == 9));
	ASSERT_TEST((*(int*)listGetNext(list1) == 8));
	ASSERT_TEST((*(int*)listGetNext(list1) == 7));
	ASSERT_TEST((*(int*)listGetNext(list1) == 6));
	ASSERT_TEST((*(int*)listGetNext(list1) == 5));
	ASSERT_TEST((*(int*)listGetNext(list1) == 4));
	ASSERT_TEST((*(int*)listGetNext(list1) == 3));
	ASSERT_TEST((*(int*)listGetNext(list1) == 2));
	ASSERT_TEST((*(int*)listGetNext(list1) == 1));
	ASSERT_TEST((*(int*)listGetNext(list1) == 0));
	ASSERT_TEST((listGetNext(list1) == NULL));
	listDestroy(list1);
	return true;
}
static bool testlistGetCurrent() {
	CREATE_LIST1
	ASSERT_TEST(listGetCurrent(list1) == NULL);
	ASSERT_TEST(listGetCurrent(NULL) == NULL);
	ADD_VALUES_TO_LIST
	ASSERT_TEST((*(int*)listGetFirst(list1)) == 9);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 9);
	listDestroy(list1);
	return true;
}

static bool testlistInsertFirst() {
	CREATE_LIST1
	ADD_VALUES_TO_LIST
	listInsertFirst(list1, arr + 9);
	ASSERT_TEST((*(int*)listGetFirst(list1)) == 0);
	listInsertFirst(list1, arr + 8);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 0);
	listDestroy(list1);
	return true;
}

static bool testlistInsertLast() {
	CREATE_LIST1
	ADD_VALUES_TO_LIST
	int i = 9;
	LIST_FOREACH(int*, iterator, list1) {
		ASSERT_TEST(*iterator == i);
		i--;
	}
	listDestroy(list1);
	return true;
}

static bool testlistInsertBeforeCurrent() {
	CREATE_LIST1
	int x = 8;
	ListResult result = listInsertBeforeCurrent(list1,&x);
	ASSERT_TEST(result == LIST_INVALID_CURRENT);
	ADD_VALUES_TO_LIST
	listGetFirst(list1);
	listInsertBeforeCurrent(list1, arr + 9);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 9);
	ASSERT_TEST((*(int*)listGetFirst(list1)) == 0);
	listGetNext(list1);
	listInsertBeforeCurrent(list1,arr + 8);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 9);
	listGetFirst(list1);
	ASSERT_TEST((*(int*)listGetNext(list1)) == 1);
	listDestroy(list1);
	return true;
}

static bool testlistInsertAfterCurrent() {
	CREATE_LIST1
	int x = 8;
	ListResult result = listInsertAfterCurrent(list1, &x);
	ASSERT_TEST(result == LIST_INVALID_CURRENT);
	ADD_VALUES_TO_LIST
	listGetFirst(list1);
	listInsertAfterCurrent(list1, arr + 9);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 9);
	ASSERT_TEST((*(int*)listGetNext(list1)) == 0);
	listDestroy(list1);
	return true;
}
static bool testlistRemoveCurrent() {
	CREATE_LIST1
	ListResult result = listRemoveCurrent(list1);
	ASSERT_TEST(result == LIST_INVALID_CURRENT);
	ADD_VALUES_TO_LIST
	listGetFirst(list1);
	listRemoveCurrent(list1);
	ASSERT_TEST(listGetCurrent(list1) == NULL);
	ASSERT_TEST((*(int*)listGetFirst(list1)) == 8);
	listGetNext(list1);
	listRemoveCurrent(list1);
	listGetFirst(list1);
	ASSERT_TEST((*(int*)listGetNext(list1)) == 6);
	while (listGetFirst(list1)!=NULL)	{
		listRemoveCurrent(list1);
	}
	ASSERT_TEST(listGetSize(list1)==0);
	listDestroy(list1);
	return true;
}

static bool testlistSort() {
	CREATE_LIST1
	ListResult result = listSort(list1,NULL,NULL);
	ASSERT_TEST(result == LIST_NULL_ARGUMENT);
	ADD_VALUES_TO_LIST
	int key = 7, i=0;
	listGetFirst(list1);
	listSort(list1, compareInt, &key);
	//ASSERT_TEST((*(int*)listGetCurrent(list1)) == 0);
	LIST_FOREACH(int*, iterator, list1) {
		ASSERT_TEST(*iterator == i);
		i++;
	}
	listClear(list1);
	NEW_VALUES_TO_LIST
	listGetFirst(list1);
	listGetNext(list1);
	listSort(list1, compareInt, &key);
	i = 0;
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 1);
	LIST_FOREACH(int*, iterator, list1) {
		ASSERT_TEST(*iterator == i);
		i++;
	}
	
	listDestroy(list1);
	return true;
}

static bool testlistFilter() {
	CREATE_LIST1
	ADD_VALUES_TO_LIST
	int key = 4;
	List filtered_list = listFilter(NULL, filterInt, &key);
	ASSERT_TEST(filtered_list == NULL);
	filtered_list = listFilter(list1, NULL, &key);
	ASSERT_TEST(filtered_list == NULL);
	listGetFirst(list1);
	filtered_list= listFilter(list1, filterInt,&key);
	ASSERT_TEST((*(int*)listGetCurrent(list1)) == 9);
	ASSERT_TEST(listGetCurrent(filtered_list) == NULL);
	ASSERT_TEST((*(int*)listGetFirst(filtered_list)) == 3);
	key = -1;
	listDestroy(filtered_list);
	filtered_list = listFilter(list1, filterInt, &key);
	ASSERT_TEST(listGetSize(filtered_list) == 0);
	listDestroy(list1);
	listDestroy(filtered_list);
	return true;
}

static bool testlistClear() {
	CREATE_LIST1
	ADD_VALUES_TO_LIST
	listGetFirst(list1);
	ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listClear(list1) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list1) == 0);
	ASSERT_TEST(listGetCurrent(list1) == NULL);
	ASSERT_TEST(listGetFirst(list1) == NULL);
	listDestroy(list1);
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testlistGetNext);
	RUN_TEST(testlistGetCurrent);
	RUN_TEST(testlistInsertFirst);
	RUN_TEST(testlistInsertLast);
	RUN_TEST(testlistInsertBeforeCurrent);
	RUN_TEST(testlistInsertAfterCurrent);
	RUN_TEST(testlistRemoveCurrent);
	RUN_TEST(testlistSort);
	RUN_TEST(testlistFilter);
	RUN_TEST(testlistClear);
	return 0;
}