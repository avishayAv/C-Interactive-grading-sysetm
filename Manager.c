#include "Manager.h"
#include "set.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Friend.h"
#include "mtm_ex3.h"
#define BEST_GRADES 1
#define WORST_GRADES (-1)

//-----------------------------------------------------------------------------

struct manager {
	Set Students;
	Student connected_student;
};
//-----------------------------------------------------------------------------
//input: set of student and a student id
//output: pointer to the student, if not exist return NULL
static Student findStudentById(Set Students, int student_id) {
	Student student_location = NULL;
	SET_FOREACH(Student, ptr, Students) {
		if (student_id == getStudentId(ptr)) {
            student_location = ptr;
		}
	}
	return student_location;
}

//-----------------------------------------------------------------------------

/*
 * Input : manager
 * Output : TRUE is someone connected, FALSE otherwise.
 */
static bool isSomeoneConnected(Manager manager1)    {
    if(manager1->connected_student == NULL) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
Manager createManager(managerResult *result, copyFunction copyStudent,
                 compareFunction compareStudent, freeFunction destroyStudent){
	Manager new_manager = malloc(sizeof(*new_manager));
	if (new_manager == NULL)	{
		*result = MANAGER_OUT_OF_MEMORY;
		return NULL;
	}
	new_manager->connected_student = NULL;
	new_manager->Students=setCreate(copyStudent,destroyStudent,compareStudent);
	if (new_manager->Students == NULL)	{
		destroyManager(new_manager);
		*result = MANAGER_OUT_OF_MEMORY;
		return NULL;
	}
	*result = MANAGER_SUCCESS;
	return new_manager;
}
//-----------------------------------------------------------------------------

managerResult addStudent(Manager manager1, int student_id,
	char* first_name, char* last_name) {
	studentResult student_result;
	SetResult set_result;
	Student new_student = createStudent(student_id, first_name, last_name,
                          &student_result, copyGrade, DestroyGrade, copyFriend,
                                        destroyFriend, compareFriend);
	if (student_result == STUD_OUT_OF_MEMORY) {
		return MANAGER_OUT_OF_MEMORY;
	}
	if (student_result == STUD_INVALID_ID){
		return MANAGER_INVALID_PARAMETER;
	}
	set_result = setAdd(manager1->Students, new_student);
	destroyStudent(new_student);//because set holds a copy by definition
	if (set_result == SET_OUT_OF_MEMORY){
		return MANAGER_OUT_OF_MEMORY;
	}
	if (set_result == SET_ITEM_ALREADY_EXISTS){
		return MANAGER_STUDENT_ALREADY_EXISTS;
	}
	assert(set_result == SET_SUCCESS);
	return MANAGER_SUCCESS;
}
//-----------------------------------------------------------------------------

managerResult removeStudent(Manager manager1, int student_id) {
	studentResult result;
	Student student_to_remove = findStudentById(manager1->Students, student_id);
	if (student_to_remove == NULL){
		return MANAGER_STUDENT_DOES_NOT_EXIST;
	}
    //Handle : if the connected student to be removed - log out
	if ((manager1->connected_student != NULL)
		&&(compareStudents(manager1->connected_student,student_to_remove)==0)){
		manager1->connected_student = NULL;
	}
	SET_FOREACH(Student, ptr, manager1->Students) {
		result = removeFriend(ptr, student_to_remove);
		if (result == STUD_OUT_OF_MEMORY){
			return MANAGER_OUT_OF_MEMORY;
		}
	}
	setRemove(manager1->Students,student_to_remove);
	return MANAGER_SUCCESS;
}
//-----------------------------------------------------------------------------

managerResult studentLogIn(Manager manager1, int student_id) {
    if (isSomeoneConnected(manager1))  {
        return MANAGER_ALREADY_LOGGED_IN;
    }
	Student student1 = findStudentById(manager1->Students, student_id);
	if (student1 == NULL){
		return MANAGER_STUDENT_DOES_NOT_EXIST;
	}
	manager1->connected_student = student1;
	return MANAGER_SUCCESS;
}
//-----------------------------------------------------------------------------
managerResult studentlogOut(Manager manager1) {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
	manager1->connected_student = NULL;
	return MANAGER_SUCCESS;
}
//-----------------------------------------------------------------------------

managerResult managerAddFriendRequest(Manager manager1, int other_id) {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
	Student other_student= findStudentById(manager1->Students, other_id);
	if (other_student == NULL)	{
		return MANAGER_STUDENT_DOES_NOT_EXIST;
	}
    studentResult result = addFriendRequest(manager1->connected_student,
                                            other_student);
	switch (result){
	case STUD_OUT_OF_MEMORY:
		return MANAGER_OUT_OF_MEMORY;
	case STUD_ALREADY_FRIEND:
		return MANAGER_ALREADY_FRIEND;
	case STUD_ALREADY_REQUESTED:
		return MANAGER_ALREADY_REQUESTED;
	case STUD_SUCCESS:
    default:
		return MANAGER_SUCCESS;
	}
}
//-----------------------------------------------------------------------------
managerResult managerHandleRequest(Manager manager1,int other_id,char* action) {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
	Student other_student = findStudentById(manager1->Students,other_id);
	if (other_student== NULL){
		return MANAGER_STUDENT_DOES_NOT_EXIST;
	}
	studentResult result = handleRequest(manager1->connected_student,
                                         other_student, action);
	switch (result) {
	case STUD_OUT_OF_MEMORY:
		return MANAGER_OUT_OF_MEMORY;
	case STUD_ALREADY_FRIEND:
		return MANAGER_ALREADY_FRIEND;
	case STUD_NOT_REQUESTED:
		return MANAGER_NOT_REQUESTED;
    case STUD_INVALID_PARAMETERS:
            return MANAGER_INVALID_PARAMETER;
	case STUD_SUCCESS:
    default:
		return MANAGER_SUCCESS;
	}
}

//-----------------------------------------------------------------------------

managerResult managerUnfriend(Manager manager1, int other_id){
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
	Student other_student = findStudentById(manager1->Students, other_id);
	if (other_student == NULL) {
		return MANAGER_STUDENT_DOES_NOT_EXIST;
	}
	studentResult result = unfriend(manager1->connected_student, other_student);
	switch (result) {
	case STUD_OUT_OF_MEMORY:
		return MANAGER_OUT_OF_MEMORY;
	case STUD_NOT_FRIEND:
		return MANAGER_NOT_FRIEND;
	case STUD_SUCCESS:
    default:
		return MANAGER_SUCCESS;
	}
}

//-----------------------------------------------------------------------------


managerResult managerAddGrade (Manager manager1, int semester, int course_id,
                               char* points, int certain_grade) {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = studentAddGrade(manager1->connected_student,semester,
                                           course_id, points, certain_grade);
    switch (result) {
        case STUD_OUT_OF_MEMORY:
            return MANAGER_OUT_OF_MEMORY;
        case STUD_INVALID_PARAMETERS:
            return MANAGER_INVALID_PARAMETER;
        case STUD_SUCCESS:
        default:
            return MANAGER_SUCCESS;
    }
}
//-----------------------------------------------------------------------------

managerResult managerRemoveGrade (Manager manager1,int semester,int course_id){
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = studentRemoveGrade(manager1->connected_student,
                                              semester, course_id);
    switch (result) {
        case STUD_COURSE_DOES_NOT_EXIST:
            return MANAGER_COURSE_DOES_NOT_EXIST;
        case STUD_SUCCESS:
        default:
            return MANAGER_SUCCESS;
    }
}
//-----------------------------------------------------------------------------

managerResult managerUpdateGrade(Manager manager1,int course_id,int new_grade) {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = studentUpdateGrade(manager1->connected_student,
                                              course_id, new_grade);
    switch (result) {
        case STUD_COURSE_DOES_NOT_EXIST:
            return MANAGER_COURSE_DOES_NOT_EXIST;
        case STUD_INVALID_PARAMETERS:
            return MANAGER_INVALID_PARAMETER;
        case STUD_SUCCESS:
        default:
            return MANAGER_SUCCESS;
    }
}
//-----------------------------------------------------------------------------

managerResult managerPrintFullReport (Manager manager1,FILE* output_channel)   {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = printFullReport(manager1->connected_student,
		output_channel);
	if (result == STUD_OUT_OF_MEMORY) {
		return MANAGER_OUT_OF_MEMORY;
	}
    return MANAGER_SUCCESS;
}

//-----------------------------------------------------------------------------

managerResult managerPrintCleanReport(Manager manager1,FILE* output_channel)   {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = printCleanReport(manager1->connected_student,
											output_channel);
	if (result == STUD_OUT_OF_MEMORY)	{
		return MANAGER_OUT_OF_MEMORY;
	}
    return MANAGER_SUCCESS;
}

//-----------------------------------------------------------------------------

/*
 * Input : output channel, amounts (of grades) and isBest
 * is_best : TRUE if best grades requested, FALSE if worst grades requested.
 * Output :
 * - MANAGER_INVALID_PARAMETER : negative amount
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_SUCCESS : clean grade sheet (effective only) printed successfully
 */
managerResult managerPrintBestReport (Manager manager1, int amount,bool is_best,
                                      FILE* output_channel)   {

    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
	if (amount <= 0) {
		return MANAGER_INVALID_PARAMETER;
	}
    //printBestReport function handles 1 for best and -1 for worst grades
    int value_best = WORST_GRADES;
    if (is_best)    {
        value_best = BEST_GRADES;
    }
    studentResult result=printBestReport(manager1->connected_student,value_best,
                                         output_channel, amount);
    switch (result) {
        case STUD_OUT_OF_MEMORY:
            return MANAGER_OUT_OF_MEMORY;
        case STUD_SUCCESS:
        default:
            return MANAGER_SUCCESS;
    }
}

//-----------------------------------------------------------------------------
managerResult managerAskReference(Manager manager1,FILE* output_channel,
                                  int course_id, int amount) {

    if (manager1->connected_student ==NULL)	{
        return MANAGER_NOT_LOGGED_IN;
    }
	if (amount<=0) {
		return MANAGER_INVALID_PARAMETER;
	}
    studentResult result = askReference(output_channel,
                               manager1->connected_student, course_id, amount);
    if (result == STUD_OUT_OF_MEMORY)	{
        return MANAGER_OUT_OF_MEMORY;
    }
    return MANAGER_SUCCESS;
}
//-----------------------------------------------------------------------------

managerResult managerFacultyRequest (Manager manager1, int course_id,
                                     char* request,FILE* output_channel)    {
    if (!isSomeoneConnected(manager1))  {
        return MANAGER_NOT_LOGGED_IN;
    }
    studentResult result = addFacultyRequest(manager1->connected_student,
                                             course_id, request);
    switch (result) {
	    case STUD_COURSE_DOES_NOT_EXIST:
            return MANAGER_COURSE_DOES_NOT_EXIST;
        case STUD_INVALID_PARAMETERS:
            return MANAGER_INVALID_PARAMETER;
        case STUD_SUCCESS:
        default:
			mtmFacultyResponse(output_channel, "your request was rejected");
            return MANAGER_SUCCESS;
    }
}

//-----------------------------------------------------------------------------
void destroyManager(Manager manager1) {
	manager1->connected_student = NULL;
	setDestroy(manager1->Students);
	free(manager1);
	return;
}

//-----------------------------------------------------------------------------

void printManager(Manager manager1) {
	Set set1 = manager1->Students;
	SET_FOREACH(Student, ptr, set1) {
		printStudent(ptr);
	}
	return;
}