#include "list_mtm.h"
#include <stdlib.h>
#include <assert.h>

#define EMPTY_LIST (-1)
#define POINT_TO_NULL (-1)

typedef struct node* Node;

struct node {
    ListElement data;
    Node next;
};

struct List_t {
    Node header;
    Node iterator;
    int size;
    CopyListElement copyElement;
    FreeListElement freeElement;
};

static bool isListEmpty(List list) {
    assert(list != NULL);
    return (list->size == 0);
}

static Node getLastElement(List list) {
    assert(list != NULL);
    if (isListEmpty(list)) {
        return NULL;
    }
    Node ptr = list->header;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    return ptr;
}

static Node getBeforeCurrentElement(List list) {
    assert(list != NULL);
    if (isListEmpty(list)) {
        return NULL;
    }
    Node ptr = list->header;
    while (ptr->next != list->iterator) {
        ptr = ptr->next;
    }
    return ptr;
}


static void swap(ListElement* element1, ListElement* element2) {
	ListElement temp = *element1;
	*element1 = *element2;
	*element2 = temp;
	}

static void bubbleSort(List list, CompareListElements compareElement,
                       ListSortKey key) {
	int length = listGetSize(list);
	for (int j = length; j>1; j--){
		listGetFirst(list);
		for (int i = 0; i < (j-1); i++) {
			if (compareElement(list->iterator->data ,
                               list->iterator->next->data,key)>0){
				swap(&(list->iterator->data),	&(list->iterator->next->data));
			}
			listGetNext(list);
		}
	}
}

static void freeNode(Node node1) {
    free(node1);
}

//Input : list
//Output : index of iterator in the list
static int findPlaceOfIterator (List list)   {
	if (list->iterator == NULL){
		return POINT_TO_NULL;
	}
	int counter = 1;
	ListElement temp = list->iterator->data;
    LIST_FOREACH(ListElement, ptr, list) {
        if (temp == ptr) {
            break;
        }
        counter++;
    }
    return counter;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if ((!copyElement) || (!freeElement)) {
        return NULL;
    }

    List newList = malloc(sizeof(*newList));
    if (newList == NULL) {
        return NULL;
    }

    newList->header = NULL;
    newList->iterator = NULL;
    newList->size = 0;
    newList->copyElement = copyElement;
    newList->freeElement = freeElement;

    return newList;
}

List listCopy(List list) {
    if (list == NULL) {
        return NULL;
    }
    Node temp_iterator = list->iterator;
    List copiedList = listCreate(list->copyElement, list->freeElement);
	if (copiedList ==NULL)	{
		return NULL;
	}
    ListResult insert_result;
    LIST_FOREACH(ListElement, ptr, list) {
		ListElement copiedElement = list->copyElement(ptr);
        insert_result = listInsertLast(copiedList, copiedElement);
        if (insert_result == LIST_OUT_OF_MEMORY) {
            listDestroy(copiedList);
            return NULL;
        }
        assert(insert_result == LIST_SUCCESS);
		list->freeElement(copiedElement);
    }
    list->iterator = temp_iterator;
    int iterator_place = findPlaceOfIterator(list);
    //iterator of copied list should point to the same element
    copiedList->iterator = copiedList->header;
    while(iterator_place > 1)   {//iterator might be point to null
        copiedList->iterator = copiedList->iterator->next;
        iterator_place--;
    }
	if (iterator_place == POINT_TO_NULL) {
		copiedList->iterator= NULL;
	}
    return copiedList;
}

int listGetSize(List list) {
    if (list == NULL) {
        return EMPTY_LIST;
    }
    return list->size;
}

ListElement listGetFirst(List list) {
    if ((list == NULL) || (isListEmpty(list))) {
        return NULL;
    }
    list->iterator = list->header;
    return list->iterator->data;
}

ListElement listGetNext(List list) {
    if ((list == NULL) || (list->iterator == NULL)) {
        return NULL;
    }
    list->iterator = list->iterator->next;
	if (list->iterator == NULL)
	{
		return NULL;
	}
    return list->iterator->data;
}

ListElement listGetCurrent(List list) {
    if ((list == NULL) || (list->iterator == NULL)) {
        return NULL;
    }
    return list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element) {
    if ((list == NULL) || (element == NULL)) {
        return LIST_NULL_ARGUMENT;
    }
    ListElement newElement = list->copyElement(element);
    Node newNode = malloc(sizeof(*newNode));
    if ((newElement == NULL) || (newNode == NULL)) {
        return LIST_OUT_OF_MEMORY;
    }
    newNode->data = newElement;
    newNode->next = list->header;
    list->header = newNode;
    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
	if ((list == NULL) || (element == NULL)) {
		return LIST_NULL_ARGUMENT;
	}
	ListElement newElement = list->copyElement(element);
	Node newNode = malloc(sizeof(*newNode));
	if ((newElement == NULL) || (newNode == NULL)) {
		return LIST_OUT_OF_MEMORY;
	}
	newNode->data = newElement;
	newNode->next = NULL;
	Node temp_iterator = list->iterator;
	Node ptr = getLastElement(list);
	if (ptr == NULL) {   //means list is empty
		list->header = newNode;
	}
	else {
		ptr->next = newNode;
	}
	list->size++;
	list->iterator = temp_iterator;

	return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if ((list == NULL) || (element == NULL)) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    ListElement newElement = list->copyElement(element);
	if (newElement == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
    Node newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
		list->freeElement(newElement);
        return LIST_OUT_OF_MEMORY;
    }
    newNode->data = newElement;
    newNode->next = list->iterator;
	if (list->iterator==list->header){
		list->header = newNode;
	}
	else{
		Node ptr = getBeforeCurrentElement(list);
		ptr->next = newNode;
	}
    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
    if ((list == NULL) || (element == NULL)) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    ListElement newElement = list->copyElement(element);
	if (newElement == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
    Node newNode = malloc(sizeof(*newNode));
	if (newNode == NULL) {
		list->freeElement(newElement);
		return LIST_OUT_OF_MEMORY;
	}
    newNode->data = newElement;
    newNode->next = list->iterator->next;
    list->iterator->next = newNode;
    list->size++;
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
	if (list->iterator== list->header)	{
		list->header = list->iterator->next;
	}
	else{
		Node ptr = getBeforeCurrentElement(list);
		ptr->next = list->iterator->next;
	}
    Node ptr = list->iterator;
    list->iterator = NULL;
    list->freeElement(ptr->data);
    freeNode(ptr);
	list->size--;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement,
                    ListSortKey key) {
    if ((list == NULL) || (compareElement == NULL)) {
        return LIST_NULL_ARGUMENT;
    }
	
	if (listGetSize(list)==0)	{
		return LIST_SUCCESS;
	}
	int iterator_place = findPlaceOfIterator(list);
    bubbleSort(list, compareElement, key);
    listGetFirst(list);
    for (; iterator_place > 1; iterator_place--) {
        listGetNext(list);
    }
	if (iterator_place == POINT_TO_NULL)	{
		list->iterator = NULL;
	}
	return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if ((list == NULL) || (filterElement == NULL)) {
        return NULL;
    }
    List filteredList = listCreate(list->copyElement, list->freeElement);
	if (filteredList == NULL)	{
		return NULL;
	}
    ListResult insert_result;
    Node temp_iterator = list->iterator;
    LIST_FOREACH(ListElement, ptr, list) {
        if (filterElement(ptr, key)) {
            insert_result = listInsertLast(filteredList, ptr);
            if (insert_result == LIST_OUT_OF_MEMORY) {
                listDestroy(filteredList);
                return NULL;
            }
            assert(insert_result == LIST_SUCCESS);
        }
    }
    list->iterator = temp_iterator;
    filteredList->iterator = NULL;
    return filteredList;
}

ListResult listClear(List list) {
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
	Node temp = NULL;
    LIST_FOREACH(ListElement, ptr, list) {
		freeNode(temp);
        list->freeElement(ptr);
		temp = list->iterator;
    }
	freeNode(temp);
    list->header = NULL;
    list->iterator = NULL;
	list->size = 0;
    return LIST_SUCCESS;
}

void listDestroy(List list) {
    if (list != NULL) {
        listClear(list);
        free(list);
    }
}
