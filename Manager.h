#ifndef courseManager_H_
#define courseManager_H_

#include <stdbool.h>
#include <stdio.h>
#include "Grade.h"
#include "Student.h"

//-----------------------------------------------------------------------------

typedef enum {
	MANAGER_SUCCESS,
	MANAGER_OUT_OF_MEMORY,
	MANAGER_INVALID_PARAMETER,
	MANAGER_NOT_LOGGED_IN,
	MANAGER_ALREADY_LOGGED_IN,
	MANAGER_STUDENT_DOES_NOT_EXIST,
	MANAGER_STUDENT_ALREADY_EXISTS,
	MANAGER_NOT_FRIEND,
	MANAGER_ALREADY_FRIEND,
	MANAGER_NOT_REQUESTED,
	MANAGER_ALREADY_REQUESTED,
	MANAGER_COURSE_DOES_NOT_EXIST
} managerResult;

typedef struct manager* Manager;

//-----------------------------------------------------------------------------
/*Return pointer to a new manager (and MANAGER_SUCCESS to result enum).
* in case of memory allocation fail return NULL.
* (and MANAGER_OUT_OF_MEMORY to result enum)
NOTE :
Create set for student's
*/
Manager createManager(managerResult *result, copyFunction copyStudent,
                   compareFunction compareStudent, freeFunction destroyStudent);
//-----------------------------------------------------------------------------
/*
*input: manager, details of new student.
* return:
*MANAGER_SUCCESS for SUCCESS.
*MANAGER_INVALID_PARAMETER for INVALID ID.
*MANAGER_STUDENT_ALREADY_EXISTS for a student that already exist 
*MANAGER_OUT_OF_MEMORY for memory allocation failure
*/
managerResult addStudent(Manager manager1, int student_id,
	char* first_name, char* last_name);
//-----------------------------------------------------------------------------
/* remove a certain student from the system, include all his grades
* and friendship connection(request or friendShip)
* input: manager, student to remove by id.
* return: 
* MANAGER_STUDENT_DOES_NOT_EXIST in case of non-existing student,
* or invalid id.
* MANAGER_OUT_OF_MEMORY in case of allocation failure.
* MANAGER_SUCCESS in case of success.
*/
managerResult removeStudent(Manager manager1, int student_id);
//-----------------------------------------------------------------------------
/*
* input: manager, student to login by id.
* return: 
*MANAGER_SUCCESS
*MANAGER_ALREADY_LOGGED_IN if someone already logged in.
*MANAGER_STUDENT_DOES_NOT_EXIST for id of a non-existing student
*/
managerResult studentLogIn(Manager manager1, int student_id);
//-----------------------------------------------------------------------------
/*
*input: manager.
*return:
*MANAGER_NOT_LOGGED_IN If no one is connected
*else logOut the connected student and return MANAGER_SUCCESS
*/
managerResult studentlogOut(Manager manager1);
//-----------------------------------------------------------------------------
/*
* input: manager, student that will get requested from connected student
* return:
* MANAGER_NOT_LOGGED_IN if no one is connected
* MANAGER_STUDENT_DOES_NOT_EXIST for non-existing student.
* if already friends, or connected student and other_student are equal
* return MANAGER_ALREADY_FRIEND.
* if already requested return: MANAGER_ALREADY_REQUESTED.
* make the friendShip request and return MANAGER_SUCCESS
* if memory allocation faildes return MANAGER_OUT_OF_MEMORY.
*/
managerResult managerAddFriendRequest(Manager manager1, int other_id);
//-----------------------------------------------------------------------------
/*
*input: manager, other student id, action(accept/reject)
return :
* if action isn't valid return: MANAGER_INVALID_PARAMETER
* if no one is logged in return MANAGER_NOT_LOGGED_IN 
* if id is invalid, or student does not exist in system 
retuen MANAGER_STUDENT_DOES_NOT_EXIST.
* if connected student and other student already friend: MANAGER_ALREADY_FRIEND 
* if connected student never got requested from other
 * student: MANAGER_NOT_REQUESTED
* MANAGER_SUCCESS for success.
* if memory allocation faildes return MANAGER_OUT_OF_MEMORY.
*/
managerResult managerHandleRequest(Manager manager1,int other_id,char* action);
//-----------------------------------------------------------------------------
/*
* input: manager, student to unfriend from connected student by his id.
*return :
*MANAGER_NOT_LOGGED_IN if no one is logged in.
*MANAGER_STUDENT_DOES_NOT_EXIST if studnet isn't exist or for invalid id.
*MANAGER_NOT_FRIEND if the friendship wasnt exist
 * (friendShip request is not a friendship).
*MANAGER_SUCCESS.
* if memory allocation failes return MANAGER_OUT_OF_MEMORY.
*/
managerResult managerUnfriend(Manager manager1, int other_id);
//-----------------------------------------------------------------------------
/*
 * Input : manager, semester, course ID, points and grade
 * Process : add grade to connected student with the following parameters
 * Output :
 * - MANAGER_OUT_OF_MEMORY if there was a memory error
 * - MANAGER_INVALID_PARAMETERS if one of the parameters is illegal
 * - MANAGER_NOT_LOGGED_IN if there is no connected student.
 * - MANAGER_SUCCESS if grade was added successfully
 */
managerResult managerAddGrade (Manager manager1, int semester,
                               int course_id, char* points, int certain_grade);
//-----------------------------------------------------------------------------
/*
 * Input : manager, semester, course ID
 * Process : should delete the requested grade from gradesheet.
 * if there's more than one - should delete the most updated one.
 * Output :
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_COURSE_DOES_NOT_EXIST : no such course+semester in gradesheet
 * - MANAGER_SUCCESS : removed successfully
 */
managerResult managerRemoveGrade (Manager manager1,int semester,int course_id);
//-----------------------------------------------------------------------------
/*
 * Input : manager, course ID, and new grade.
 * Process : should update course's grade to new grade.
 * If there's more than one, change last one (sorted by semester)
 * Output :
 * - MANAGER_INVALID_PARAMETER : grade is not valid
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_COURSE_DOES_NOT_EXIST : no such course in gradesheet
 * - MANAGER_SUCCESS : grade updated successfully
 */
managerResult managerUpdateGrade (Manager manager1,int course_id,int new_grade);
//-----------------------------------------------------------------------------
/*
 * Input : output channel.
 * Output :
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_SUCCESS : gradesheet printed successfully
 */
managerResult managerPrintFullReport (Manager manager1, FILE* output_channel);

//-----------------------------------------------------------------------------
/*
 * Input : output channel.
 * Output :
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_OUT_OF_MEMORY if there was a memory error.
 * - MANAGER_SUCCESS : clean gradesheet (effective only) printed successfully
 */
managerResult managerPrintCleanReport (Manager manager1, FILE* output_channel);
//-----------------------------------------------------------------------------
/*
 * Input : output channel, amounts (of grades) and isBest
 * is_best : TRUE if best grades requested, FALSE if worst grades requested.
 * Output :
 * - MANAGER_INVALID_PARAMETER : negative amount
 * - MANAGER_OUT_OF_MEMORY if there was a memory error
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_SUCCESS : grade sheet of best\worst grades
*  (effective only) printed successfully.
 */
managerResult managerPrintBestReport (Manager manager1, int amount,
                                      bool is_best, FILE* output_channel);
//-----------------------------------------------------------------------------
/*
* input: manager, output_channel, course for referenses, amount of friend
* process: print the connected student friend with the highest grade
* in the certain course sorted from highest to lowest(amount of friend's).
* return:
* MANAGER_INVALID_PARAMETER: for negative number in amount
* MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
* MANAGER_OUT_OF_MEMORY if there was a memory error.
* MANAGER_SUCCESS.
*/
managerResult managerAskReference(Manager manager1,FILE* output_channel,
                                  int course_id, int amount);
//-----------------------------------------------------------------------------

/*
 * Input : manager, course ID, request
 * NOTE : this is only preparation for feature feature
 * Output :
 * - MANAGER_INVALID_PARAMETER : request is not valid
 * - MANAGER_NOT_LOGGED_IN : no one is connected to manager system.
 * - MANAGER_COURSE_DOES_NOT_EXIST : request is to remove course
 *                                    and no such course in gradesheet
 * - MANAGER_SUCCESS : no problems. prints "your request was rejected"
 */
managerResult managerFacultyRequest(Manager manager1, int course_id,
									char* request, FILE* output_channel);

//-----------------------------------------------------------------------------
/*
* Input : a certain Manager
* Process : remove allocation memory for Manager
*/
void destroyManager(Manager manager);
//-----------------------------------------------------------------------------

void printManager(Manager manager1);
#endif /*courseManager_H_ */