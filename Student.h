#ifndef STUDENT_H_
#define STUDENT_H_

#include <stdbool.h>
#include <stdio.h>
#include "list.h"
#include "Grade.h"

//-----------------------------------------------------------------------------

typedef enum {
	STUD_SUCCESS,
	STUD_INVALID_ID,
	STUD_ALREADY_FRIEND,
	STUD_ALREADY_REQUESTED,
	STUD_NOT_REQUESTED,
	STUD_NOT_FRIEND,
	STUD_INVALID_PARAMETERS,
	STUD_OUT_OF_MEMORY,
	STUD_COURSE_DOES_NOT_EXIST
} studentResult;

typedef enum    {
    NOT_EFFECTIVE,
    EFFECTIVE_SEMESTER,
    EFFECTIVE_GRADESHEET,
    EFFECTIVE_FOREVER
} effectiveResult;

typedef struct student* Student;
typedef Element (*copyFunction)(Element);
typedef int(*compareFunction)(Element, Element);
typedef void (*freeFunction)(Element);

//-----------------------------------------------------------------------------

/*Return pointer to a new student (and STUD_SUCCESS to result enum).
If the ID is invalid. student=NULL, result=STUD_INVALID_ID
NOTE :
Create set for friends
Create list for grades
*/
Student createStudent(int id, char* first_name, char* last_name,
                     studentResult* result, copyFunction copyGrade,
                     freeFunction freeGrade, copyFunction copyFriend,
                     freeFunction destroyFriend, compareFunction compareFriend);

//-----------------------------------------------------------------------------

/*
*copy an element of student by all his parameters to a new element of student
* input: Student to copy
* return: NULL if allocation fail or if student1 == NULL
* else: return pointer to copied student
* NOTE: iterator's of graedSheet and friend are pointing to the same place
* that the original student iterator was pointing
*/
Element copyStudent(Element student1);

//-----------------------------------------------------------------------------


/**
* input: student1, student2 arestudent to compare
* the comparison is made by the id values of the Students
* return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
* 		A negative integer if the second element is greater.
*/
int compareStudents(Element student1, Element student2);

//-----------------------------------------------------------------------------
/*
* add a new friend request from the connected student to other student.
* input: connected student, other student.
* return: If student1 and other_student are equal, return STUD_ALREADY_FRIEND.
* if already friends, return STUD_ALREADY_FRIEND.
* if already requested return: STUD_ALREADY_REQUESTED
* else add the request to both students set of friend(with diferrent statuses)
* and return STUD_SUCCESS.
* in case of failed allocation return STUD_OUT_OF_MEMORY.
*/
studentResult addFriendRequest(Student student1, Student other_student);

//-----------------------------------------------------------------------------
/*
*input: connected student, other student, action (accept/reject).
* assert- action parameter was already checked and it's valid
* check other_student status in the connected student set of friends
* process:in cases of valid request from other student to the connected student,
* if action accept, create the friendship, if action reject delete the request.
* output:
* STUD_NOT_REQUESTED - if never requested
* STUD_ALREADY_FRIEND - if they were already friend
* STUD_INVALID_PARAMETER - if action is invalid
* STUD_SUCCESS -in case of vaid request.
* in case of failed allocation return STUD_OUT_OF_MEMORY.
*/

studentResult handleRequest(Student student1,
                            Student other_student, char* action);
//-----------------------------------------------------------------------------
/*
*input: connected student, student to remove
*return:
* STUD_NOT_FRIEND if their isn't friendship between them or if student and other
* student are equal.
* STUD_SUCCESS otherwise, in that case friendship will be removed.
* STUD_OUT_OF_MEMORY in case of allocation failure.
*/
studentResult unfriend(Student student1, Student other_student);

//-----------------------------------------------------------------------------
/*input: student, student to remove
* process: remove the student from current set of friend's
* output:
* STUD_OUT_OF_MEMORY in case of allocation failure.
* STUD_SUCCESS.
*/
studentResult removeFriend(Student current, Student student_to_remove);
//-----------------------------------------------------------------------------

//input: student
//output: the current student id
int getStudentId(Student student1);

//-----------------------------------------------------------------------------

/*
* Input : student, semester, course id, points and grade
* NOTE : parameters are valid (tested in manager)
* Output :
* - if a memory issue appeared during creation - return STUD_OUT_OF_MEMORY
* - if one of the parameters is invalid - return STUD_INVALID_PARAMETERS
* - if succeeded to create grade, add it to student's list
* -- if couldn't add it to the list, return return STUD_OUT_OF_MEMORY
* -- if succeeded to add it to the list, return STUD_SUCCESS
*/
studentResult studentAddGrade(Student student1, int semester, int course_id,
	char* points, int certain_grade);
//-----------------------------------------------------------------------------
/*
* Input : student, course ID, a grade
* NOTE : grade parameter should be valid (tested in manager)
* Output :
* STUD_COURSE_DOES_NOT_EXIST if course doesn't exist in the student's grades.
* STUD_INVALID_PARAMETER if new grade is invalid
* STUD_SUCCESS if the new grade was updated in the course successfully.
*/
studentResult studentUpdateGrade(Student student1,int course_id,int new_grade);
//-----------------------------------------------------------------------------
/*
 * Input : student, semester, course ID
 * finds the last grade for this course in this semester.
 * Output :
 * STUD_COURSE_DOES_NOT_EXIST if there's no such.
 * STUD_SUCCESS if removed.
*/
studentResult studentRemoveGrade(Student student1,int semester,int course_id);
//-----------------------------------------------------------------------------

/*
 * Input : student
 * Output :
 * - student's details
 * - all student's grades. Sorted by : #1 semester, #2 order of insertion
 * - summary for each semester
 * - summary for all grades
 * STUD_SUCCESS : for success.
 * STUD_OUT_OF_MEMORY : for memory allocation error.
 */
studentResult printFullReport(Student student1, FILE* output_channel);

//-----------------------------------------------------------------------------

/*
 * helper function for printFullReport
 * responsible for summarizing achievements.
 * Input : student, array of all semesters that appears, number of semesters
 * Output : as described in printFullReport and mtm_ex3 file.
 */
void handlePrintFormat(Student student1, int* all_semesters,
                       int count_semesters, FILE* output_channel);
//-----------------------------------------------------------------------------

/*
 * Input : student
 * Output :
 * - student's details
 * - all student valid grades. Sorted by : #1 course_id, #2 semester
 * - summary for all grades
 * STUD_SUCCESS : for success.
 * STUD_OUT_OF_MEMORY : for memory allocation error
 */
studentResult printCleanReport(Student student1, FILE* output_channel);

//-----------------------------------------------------------------------------

/*NOTE : isBest=(-1) means printWorstGrades
* input: connected student,isBest (if isBest = 1 then print best grades,
* if isBest = -1 print worst grades), output channel,amount of grades to print. 
* assuming: amount is legal parameter
* return:
* if memory allocation failed return STUD_OUT_OF_MEMORY.
* else print amount of highest/lowest grades and return STUD_SUCCESS.
*/
studentResult printBestReport(Student student1, int isBest,
                              FILE* output_channel, int amount);

//-----------------------------------------------------------------------------
/*
* input: output_channel,connected student ,course for referenses, amount.
* process: print the connected student friend's with the highest grade
* in the certain course. sorted from highest to lowest(amount of friend's).
* return:
* STUD_OUT_OF_MEMORY if there was a memory error.
* MANAGER_SUCCESS.
*/
studentResult askReference(FILE* output, Student student1,
						   int course_id, int amount);

//-----------------------------------------------------------------------------

/*
 * Input : student, course ID, request
 * NOTE : this is only preparation for feature feature
 * Output :
 * - STUD_COURSE_DOES_NOT_EXIST : request is to remove course
 *                                    and no such course in gradesheet
 * - STUD_INVALID_PARAMETERS : request is invalid
 * - STUD_SUCCESS : no problems. prints "your request was rejected"
 */
studentResult addFacultyRequest (Student student1,int course_id,char* request);

//-----------------------------------------------------------------------------

/*
 * ----FOR INTERNAL TESTS ONLY----
 * Input : student
 * Output : print all his details
 */
void printStudent (Student student1);

//-----------------------------------------------------------------------------

/*
 * Input : a certain student
 * Process : remove allocation memory for student
 */
void destroyStudent (Element student1);

#endif /*STUDENT_H_ */
