#include "Student.h"
#include "Friend.h"
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define BILLION 1000000000
#define COURSE_NOT_EXIST (-1)
#define SECOND_BIGGER (-1)
#define EQUAL 0
#define FIRST_BIGGER 1

#define DESTORY_STUDENT destroyStudent(new_student);    \
                        *result = STUD_OUT_OF_MEMORY;   \
                        return NULL;

#define SET_TO_NULL     new_student->first_name = NULL; \
                        new_student->last_name = NULL;  \
                        new_student->grade_sheet = NULL;    \
                        new_student->friends = NULL;

#define SET_COPIED_TO_NULL      copied_student->first_name = NULL; \
                                copied_student->last_name = NULL;  \
                                copied_student->grade_sheet = NULL;    \
                                copied_student->friends = NULL;

//-----------------------------------------------------------------------------

struct student {
	int id;
	char* first_name;
	char* last_name;
    List grade_sheet;
	Set friends;
};

//-----------------------------------------------------------------------------
/*Check status between the connected students
* (which is brought by set of friends) and other_student.
* input: set of friend, other Student, iterator to the set.
* @return:
*status friendship.
* a pointer to other_student in the friend of set using iterator
* If there is no, return NOT_FRIENDS, and pointer will point to NULL
*/
static friendshipStatus checkFriendshipStatus
		(Set friends1, Friend other_student1, Friend* iterator) {
	assert((friends1 != NULL) && (other_student1 != NULL));
	friendshipStatus status = NOT_FRIENDS;
	SET_FOREACH(Friend, ptr, friends1) {
		if (compareFriend(ptr, other_student1) == 0) {
			status = getCurrentFriendStatus(ptr);
			*iterator = ptr;
			return status;
		}
	}
	*iterator = NULL; 
	return status;
}
//-----------------------------------------------------------------------------
/*
*input: connected_student, student which his request should be handle
*(other student should come in student and in friend structs).
* friendshipStatus, iterator(pointing to other_friend).
*Case #0 : status WAITING_FOR_YOU.
 * change status to FRIENDS in both student1 and other_student.
Return STUD_SUCCESS
*Case #1 : status BOTH_REQUSTED change status to FRIENDS
* in both student1 and other_student.
*Return STUD_SUCCESS.
*STUD_OUT_OF_MEMORY in case of allocation failure.
* NOTE: change iterator.
*/
static studentResult acceptFriend(Student student1, Friend other_friend,
		Student other_student, friendshipStatus status, Friend iterator) {
	assert((status == BOTH_REQUESTED) || (status == WAITING_FOR_YOU));
	Friend friend1 = createFriend(student1, FRIENDS);
	if (friend1 == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	changeFriendshipStatus(iterator, FRIENDS);
	//make iterator to point to friend1 in other_student set of friends
	checkFriendshipStatus(other_student->friends, friend1, &iterator);
	assert((getCurrentFriendStatus(iterator) == FRIENDSHIP_REQUESTED)||
		(getCurrentFriendStatus(iterator)== BOTH_REQUESTED));
	changeFriendshipStatus(iterator, FRIENDS);
	destroyFriend(friend1);
	destroyFriend(other_friend);
	return STUD_SUCCESS;
}
//-----------------------------------------------------------------------------
/*
*input: connected_student, student which his request should be handle
*(other student should come in student and in friend structs).
* friendshipStatus, iterator(pointing to other_friend).
*Case #0 : status WAITING_FOR_YOU.
 * remove friend (from both student1 and other_student)
Return STUD_SUCCESS
*Case #1 : status BOTH_REQUESTED. change status
 * (student1 to FRIEND_REQUESTED, other_student to WAITING_FOR_YOU)
Return STUD_SUCCESS
*	or STUD_OUT_OF_MEMORY in case of allocation failure.
* NOTE: change iterator.
*/
static studentResult rejectFriend(Student student1, Friend other_friend,
			Student other_student, friendshipStatus status, Friend iterator) {
	Friend friend1 = createFriend(student1, FRIENDS);
	if (friend1 == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	if (status == WAITING_FOR_YOU) {
		setRemove(student1->friends, iterator);
		checkFriendshipStatus(other_student->friends, friend1, &iterator);
		setRemove(other_student->friends, iterator);
	}
	if (status == BOTH_REQUESTED) {
		changeFriendshipStatus(iterator, FRIENDSHIP_REQUESTED);
		//make iterator to point to friend1 in other_student set of friends
		checkFriendshipStatus(other_student->friends, friend1, &iterator);
		assert(getCurrentFriendStatus(iterator) == BOTH_REQUESTED);
		changeFriendshipStatus(iterator, WAITING_FOR_YOU);
	}
	destroyFriend(friend1);
	destroyFriend(other_friend);
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------
/*
*input: connected_student, student which his request should be handle
*(other student should come in student and in friend structs).
* friendshipStatus, iterator(pointing to other_friend).
* adding the new friend to both set of connected student and other friend.
* if friend was not exist adding him to both lists.
* if he was exist change the status value to WAITING_FOR_YOU in both lists.
* return: STUD_OUT_OF_MEMORY if allocation failed.
* return: STUD_SUCCES
* NOTE: change iterator.
*/
static studentResult addNewFriendRequest(Student student1,Friend other_friend,
	Student other_student ,friendshipStatus status, Friend iterator) {
	SetResult result;
	Friend friend1 = createFriend(student1, WAITING_FOR_YOU);
	if (friend1 == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	if (status == NOT_FRIENDS){
		changeFriendshipStatus(other_friend, FRIENDSHIP_REQUESTED);
		result = setAdd(student1->friends, other_friend);
		if (result == SET_OUT_OF_MEMORY){
			return STUD_OUT_OF_MEMORY;
		}
		result = setAdd(other_student->friends, friend1);
		if (result == SET_OUT_OF_MEMORY) {
			return STUD_OUT_OF_MEMORY;
		}
	}
	if (status == WAITING_FOR_YOU) {
		changeFriendshipStatus(iterator, BOTH_REQUESTED);
		//make iterator to point to friend1 in other_student set of friends
		checkFriendshipStatus(other_student->friends, friend1, &iterator);
		changeFriendshipStatus(iterator, BOTH_REQUESTED);
	}
    destroyFriend(friend1);
    destroyFriend(other_friend);
	return STUD_SUCCESS;
}
//-----------------------------------------------------------------------------
/*
*input: connected_student, student which his request should be handle
*(other student should come in student and in friend structs).
* iterator(pointing to other_friend).
* process: remove friendship from both student1 and other student
* return: STUD_OUT_OF_MEMORY in case of allocation failure.
* STUD_SUCCESS in any other case
*/
static studentResult removeFriendShip(Student student1, Friend other_friend,
	Student other_student, Friend iterator) {
	setRemove(student1->friends, iterator);
	Friend friend1 = createFriend(student1, WAITING_FOR_YOU);
	if (friend1 == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	checkFriendshipStatus(other_student->friends, friend1, &iterator);
	setRemove(other_student->friends, iterator);
	destroyFriend(friend1);
	destroyFriend(other_friend);
	return STUD_SUCCESS;
}
//-----------------------------------------------------------------------------

/*
* Input : student, course ID
* Output : the last semester for this specific course from grades list.
* if not exist, function will return -1.
*/
static int findLastSemester(Student student1, int course_id) {
	int max = COURSE_NOT_EXIST;
	LIST_FOREACH(Grade, ptr, student1->grade_sheet) {
		if ((isCoursesEqual(ptr, course_id)) && (getGradeSemester(ptr)>max)) {
			max = getGradeSemester(ptr);
		}
	}
	return max;
}

//-----------------------------------------------------------------------------

/*
* Input : student, course ID, semester
* Output : Find the last existing grade for this semester and course, if exist.
* If not exist, return NULL (should not happen)
*/
static Grade findLastGrade(Student student1, int course_id, int semester) {
	Grade ptr_last_grade = NULL;
	LIST_FOREACH(Grade, any_grade, student1->grade_sheet) {
		if ((isCoursesEqual(any_grade, course_id))&&
				(getGradeSemester(any_grade) == semester)) {
			ptr_last_grade = any_grade;
		}
	}
	return ptr_last_grade;
}

//-----------------------------------------------------------------------------

/*
 * Input : Grade, semester, course ID, current effective status
 * Output :
 * - EFFECTIVE_SEMESTER : Grade ID same as course ID, semester lower than grade
 * - NOT_EFFECTIVE : if course ID and semester in Grade are equal to parameters
 */
static effectiveResult checkEffectiveForCurrent (Grade iterator,
			int grade_semester, int course_id, effectiveResult is_exist)	{
	if ((grade_semester < getGradeSemester(iterator))&&
			(isCoursesEqual(iterator, course_id)))  {
		is_exist =  EFFECTIVE_SEMESTER;
	}
	if ((grade_semester == getGradeSemester(iterator))&&
			(isCoursesEqual(iterator, course_id)))  {
		is_exist = NOT_EFFECTIVE;
	}
	//Nothing is changed.
	return is_exist;
}

//-----------------------------------------------------------------------------

/*
 * Input : grade and a grade_sheet
 * Output : is it effective for this semester (as defined in mtm_ex3)
 * NOTE : sports grade is an exception (described inside the function.
 */
static effectiveResult isEffectiveGradeSemester(Grade grade1, List grades) {
    int grade_semester = getGradeSemester(grade1);
    int course_id = getGradeCourseID(grade1);
    //set iterator to current grade-check if there's later grade (same semester)
    Grade iterator = listGetFirst(grades);
    while (iterator != grade1)  {
        iterator = listGetNext(grades);
    }
    iterator = listGetNext(grades);
    effectiveResult is_exist = EFFECTIVE_GRADESHEET;
    //check if there's later grade in the same course in this semester
    while ((iterator != NULL) &&(is_exist!= NOT_EFFECTIVE))   {
        is_exist = checkEffectiveForCurrent(iterator,
										grade_semester, course_id, is_exist);
        iterator = listGetNext(grades);
    }
    //IMPORTANT! set iterator back to current grade.
    //check if there's a grade of the same course from bigger semester
    iterator = listGetFirst(grades);
    while (iterator != grade1)  {
        if (((grade_semester < getGradeSemester(iterator))&&
		(isCoursesEqual(iterator, course_id)))&&(is_exist != NOT_EFFECTIVE)){
            is_exist = EFFECTIVE_SEMESTER;
        }
        iterator = listGetNext(grades);
    }
    //course sport which is EFFECTIVE_SEMESTER is actually effective for good.
    if ((isSportCourse(course_id))&&(is_exist != NOT_EFFECTIVE)) {
        is_exist = EFFECTIVE_FOREVER;
    }
    return is_exist;
}

//-----------------------------------------------------------------------------

//input: grade to check if it effective, filter key who is point to the list
// of the current grade.
// output: true if grade effective, false if it not.
static bool isGradeEffective(Element grade1, ListFilterKey list) {
	effectiveResult result;
	result = isEffectiveGradeSemester((Grade)grade1, (List)list);
	if ((result == EFFECTIVE_GRADESHEET) || (result == EFFECTIVE_FOREVER)) {
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
//input: grades to comparison, key is parameter which determind
//if the comparison should be who is lower or who is higher
// output: which element is higher by the next factors
// 1: comparison by grade higher grade means lower in the comparison,
// so the sort will make him closer to te beginnig
// if grades are equal then
// 2: comparison by semeter higher semester means higher
// if semeters are equals
// comparison by semeter higher semester means higher
static int compareGrades(Element grade1, Element grade2, ListSortKey key) {
	int key1 = *((int*)key);
	int actual_grade_1 = getGradeActualGrade(grade1);
	int actual_grade_2 = getGradeActualGrade(grade2);
	int compare_grade_result = actual_grade_1 - actual_grade_2;
	if (compare_grade_result == 0) {
		int semester1 = getGradeSemester(grade1);
		int semester2 = getGradeSemester(grade2);
		int compare_semester_result = (semester1 - (semester2));
		if (compare_semester_result == 0) {
			int course_id1 = getGradeCourseID(grade1);
			int course_id2 = getGradeCourseID(grade2);
			int compare_course_result = ((course_id1) - (course_id2));
			return (compare_course_result);
		}
		return (compare_semester_result);
	}
	return ((-1)*compare_grade_result*key1);
}
//-----------------------------------------------------------------------------

/*
 * HELPER FUNCTION for handlePrintFormat
 * JOB :
 * update effective counters for summarize semsester and grade sheet,
 * according to course type (sport/not sport, effective/not effective)
 */
static void updateEffectiveCounters (effectiveResult is_effective, int points,
					int grade,int* effective_points_x2,int* effective_grade_x2,
					int* sum_effective_points_x2,int* sum_effective_grade_x2)  {
	if (is_effective != NOT_EFFECTIVE) {
		*effective_points_x2 +=points;
		*effective_grade_x2 += (grade*points);
		//Summarizes all not-sport grade sheet-relevant grades
		//and sports grades, which are relevant even if the student
		// took it more than once.
		if ((is_effective == EFFECTIVE_GRADESHEET)||
			(is_effective == EFFECTIVE_FOREVER))   {
			*sum_effective_points_x2 += points;
			*sum_effective_grade_x2 += (grade*points);
		}
	}
}

//-----------------------------------------------------------------------------

/*
 * Simple swapping function between two int*.
 * NOTE : helper function for maxSort implementation.
 */
void swap(int *a, int *b)   {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//-----------------------------------------------------------------------------

/*
 * Input : array of integers (positive only)
 * Output : it's maximum value
 * NOTE : helper function for maxSort implementation.
 */
static int indexOfMax(int* all_semesters, int length)  {
    int max_index = 0;
    int max_number = all_semesters[0];
    for (int i = 1; i < length; i++)    {
        if (all_semesters[i] > max_number)  {
            max_number = all_semesters[i];
            max_index = i;
        }
    }
    return max_index;
}

//-----------------------------------------------------------------------------

/*
 * Input : not sorted list of integers.
 * Output : same list sorted
 * NOTE : this function is usable for both course_id and semester as parameter.
 */
static void sortArrayOfIntegers (int* listElements, int length)   {
    int left_to_sort;
    for (left_to_sort = length; left_to_sort > 1; left_to_sort--)   {
        int max_index = indexOfMax(listElements, left_to_sort);
        swap(&listElements[max_index], &listElements[left_to_sort-1]);
    }
}

//-----------------------------------------------------------------------------

/*
 * Input : array and it's length
 * Process : remove duplicated values from array
 * Output : array's new length after removal.
 * NOTES :
 * 1. this function is usable for both course_id and semester as parameter.
 * 2. this function is not changing array's length in memory,
 *      but the return value helps to treat only the relevant.
 */
static int removeDuplicatesFromArray (int * listElements, int length)   {
    int* copied_semesters = malloc(sizeof(int)*length);
    for (int i = 0; i<length; i++)  {
        copied_semesters[i] = listElements[i];
    }
    //already existing : listElements[0] = copied_semester[0]
    int index = 0;
    for (int j = 1; j<length; j++)  {
        if (listElements[index] != copied_semesters[j])    {
            index++;
            listElements[index] = copied_semesters[j];
        }
    }
    free(copied_semesters);
    return index+1;
}

//-----------------------------------------------------------------------------

/*
 * Input : grade sheet of a certain student, empty array of integers
 * Process : move over student's grade sheet.
 * insert to all_semesters array all the semesters that appears in grade sheet.
 * Output : number of grades
 */
static int findAllSemesters (List grade_sheet, int* all_semesters) {
    int i = 0;
    if (listGetSize(grade_sheet) == 0)  {
        return i;
    }
    LIST_FOREACH(Grade, ptr, grade_sheet)   {
        all_semesters[i] = getGradeSemester(ptr);
        i++;
    }
    //In this point - all the semesters are in the array,
    //not sorted and with duplicates
    //i contains number of grades in general and should be changed
	//when duplicated are removed.
    sortArrayOfIntegers(all_semesters, i);
    i = removeDuplicatesFromArray (all_semesters, i);
    return i;
}

//-----------------------------------------------------------------------------

//input: student id
//output: true if student id is valid, false if not valid
static bool isIdValid(int student_id) {
    if ((student_id >= BILLION) || (student_id <= 0)) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------

/*
 Input : points and grade
 Process : update total points, effective points, effective grades
 and failed points accordingly.
*/
static void updateParameters (int points, int grade, int* sum_total_points_x2,
					int* sum_effective_points_x2, int* sum_effective_grade_x2,
							  int* sum_failed_points_x2)    {
    *sum_total_points_x2 += points;
    *sum_effective_points_x2 += points;
    *sum_effective_grade_x2 += (grade*points);
    if (grade < 55) {
        *sum_failed_points_x2 += points;
    }
}

//-----------------------------------------------------------------------------
/*
*input: list
* output: the highest grade in current list
* if list is empty return -1
*/
static int getHighestGrade(List list) {
	int highest_grade = -1;
	LIST_FOREACH(Grade, ptr, list) {
		int current_grade = getGradeActualGrade(ptr);
		if (current_grade>highest_grade) {
			highest_grade = current_grade;
		}
	}
	return highest_grade;
}
//-----------------------------------------------------------------------------
/*
*input:highest grade of student's friend, len of arrays, array of grades and
* array of students (the arrays are parallel), and current friend of student
* process: put the friend student in the right place in array,
* according to his grade 
* assuming: student grade higher then lowest in array.
*/
static void sortArrayOfFriendByGrades(int student_grade,int amount,
	int* array_of_grades, Student*array_of_students,Student student_friend){
	for (int i = (amount - 2); ((i == -1) || (i >= 0)); i--) {
		if ((i >= 0) && (array_of_students[i] == NULL)) {
			continue;
		}
		if ((i >= 0) && (student_grade > array_of_grades[i])) {
			continue;
		}
		assert((student_grade <= array_of_grades[i]) || (i == -1));
		if ((i >= 0) && (student_grade == array_of_grades[i]))
		{
			if (student_friend->id < array_of_students[i]->id) {
				continue;
			}
		}
		for (int j = amount - 1; j > i + 1; j--) {
			array_of_grades[j] = array_of_grades[j - 1];
			array_of_students[j] = array_of_students[j - 1];
		}
		array_of_grades[i + 1] = student_grade;
		array_of_students[i + 1] = student_friend;
		break;
	}
}

//-----------------------------------------------------------------------------
/*
*input: Grade, course_id
*output: true if the current grade course_id is equal to course_id
* NOTE: helper function to listSort function
*/
static bool filterByCourse(Element grade1, ListFilterKey course_id) {
	int course_id1 = (*(int*)course_id);
	if (getGradeCourseID(grade1) == course_id1) {
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------

/*
 * input: request
 * output: true if action is valid, else otherwise.
 * valid formats : ("remove_course"/"register_course"/"cancel_course")
 */
static bool isRequestValid(char* request) {
    if ((strcmp(request, "remove_course")==0)||
        (strcmp(request, "register_course") == 0)||
        (strcmp(request, "cancel_course") == 0)) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

//input: action
// output: true if action is valid(accept/reject), else return false
static bool isActionValid(char* action) {
    if ((strcmp(action, "accept")==0)||(strcmp(action, "reject") == 0)){
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

Student createStudent(int id, char* first_name, char* last_name,
        	studentResult* result,copyFunction copyGrade,freeFunction freeGrade,
			copyFunction copyFriend,freeFunction destroyFriend,
			compareFunction compareFriend)    {
    if (!isIdValid(id)) {
		*result = STUD_INVALID_ID;
		return NULL;
	}
	Student new_student = malloc(sizeof(*new_student));
	if (new_student == NULL)
	{
		*result = STUD_OUT_OF_MEMORY;
		return NULL;
	}
    SET_TO_NULL;
	new_student->id = id;
    new_student->first_name = malloc((sizeof(char)*(strlen(first_name)+1)));
    if (new_student->first_name == NULL)   {
        DESTORY_STUDENT;
    }
    strcpy(new_student->first_name, first_name);
    new_student->last_name = malloc((sizeof(char)*(strlen(last_name)+1)));
    if (new_student->last_name == NULL)   {
        DESTORY_STUDENT;
    }
    strcpy(new_student->last_name, last_name);
    new_student->grade_sheet = listCreate(copyGrade, freeGrade);
    if (new_student->grade_sheet == NULL)   {
        DESTORY_STUDENT;
    }
	new_student->friends = setCreate(copyFriend, destroyFriend, compareFriend);
    if (new_student->friends == NULL)   {
        DESTORY_STUDENT;
    }
    *result = STUD_SUCCESS;
	return new_student;
}

//-----------------------------------------------------------------------------

Element copyStudent(Element student1) {
	if (student1 == NULL){
		return NULL;
	}
    Student copied_student = malloc(sizeof(*copied_student));
    if (copied_student == NULL) {
        return NULL;
    }
    SET_COPIED_TO_NULL;
    copied_student->first_name =
		malloc((sizeof(char)*((strlen(((Student)student1)->first_name))+1)));
    if (copied_student->first_name == NULL)   {
        destroyStudent(copied_student);
        return NULL;
    }
    strcpy(copied_student->first_name, ((Student)student1)->first_name);
    copied_student->last_name =
			malloc((sizeof(char)*(strlen(((Student)student1)->last_name)+1)));
    if (copied_student->last_name == NULL)   {
        destroyStudent(copied_student);
        return NULL;
    }
    strcpy(copied_student->last_name, ((Student)student1)->last_name);
    copied_student->id = ((Student)student1)->id;
    copied_student->grade_sheet = listCopy(((Student)student1)->grade_sheet);
    copied_student->friends = setCopy(((Student)student1)->friends);
    if ((copied_student->grade_sheet==NULL)||(copied_student->friends==NULL)) {
        destroyStudent(copied_student);
        return NULL;
    }
    return copied_student;
}

//-----------------------------------------------------------------------------

int compareStudents(Element student1, Element student2) {
	if (((Student)student1)->id > ((Student)student2)->id){
		return FIRST_BIGGER;
	}
	if (((Student)student1)->id == ((Student)student2)->id)	{
		return EQUAL;
	}
	return SECOND_BIGGER;
}

//-----------------------------------------------------------------------------


studentResult addFriendRequest(Student student1, Student other_student) {
	assert((student1 != NULL) && (other_student != NULL));
	if (!compareStudents(student1,other_student)){
		return STUD_ALREADY_FRIEND;
	}
	Friend other_friend = createFriend(other_student, NOT_FRIENDS);
	if (other_friend == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	//make from the other student a friend elemnet to use the set compare
	friendshipStatus status;
	Friend iterator; 
	status = checkFriendshipStatus(student1->friends, other_friend, &iterator);
	if ((status == NOT_FRIENDS)||(status == WAITING_FOR_YOU)){
		studentResult result=addNewFriendRequest(student1, other_friend,
										  other_student, status,iterator);
		if (result == STUD_OUT_OF_MEMORY) {
			return STUD_OUT_OF_MEMORY;
		}
        return STUD_SUCCESS;
	}
    destroyFriend(other_friend);
	if (status == FRIENDS){
		return STUD_ALREADY_FRIEND;
	}
	assert((status == FRIENDSHIP_REQUESTED) || (status == BOTH_REQUESTED));
		return STUD_ALREADY_REQUESTED;
}
//-----------------------------------------------------------------------------

studentResult handleRequest(Student student1,Student other_student,char* action){
	assert((student1 != NULL)&&(other_student != NULL));
	if (compareStudents(student1, other_student) == 0) {
		return STUD_ALREADY_FRIEND;
	}
	Friend other_friend = createFriend(other_student, NOT_FRIENDS);
	Friend iterator;
		if (other_friend == NULL)	{
			return STUD_OUT_OF_MEMORY;
		}
		friendshipStatus status = checkFriendshipStatus(student1->friends,
													other_friend, &iterator);
		if (status == FRIENDS){
			destroyFriend(other_friend);
			return STUD_ALREADY_FRIEND;
		}
		if (status == FRIENDSHIP_REQUESTED) {
			destroyFriend(other_friend);
			return STUD_NOT_REQUESTED;
		}
		if (status == NOT_FRIENDS)		{
			destroyFriend(other_friend);
			return STUD_NOT_REQUESTED;
		}
        bool valid = isActionValid(action);
        if (!valid)	{
            destroyFriend(other_friend);
            return STUD_INVALID_PARAMETERS;
        }
		assert((status == BOTH_REQUESTED) || (status == WAITING_FOR_YOU));
		if (strcmp(action, "accept")==0) {
			return acceptFriend(student1,other_friend,
								other_student,status, iterator);
		}
		return rejectFriend(student1, other_friend,other_student,
							status, iterator);


}
//-----------------------------------------------------------------------------
studentResult unfriend(Student student1, Student other_student) {
	if (compareStudents(student1, other_student) == 0) {
		return STUD_NOT_FRIEND;
	}
	Friend other_friend = createFriend(other_student, NOT_FRIENDS);
	Friend iterator;
	if (other_friend == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	friendshipStatus status = checkFriendshipStatus(student1->friends,
													other_friend, &iterator);
	if (status!=FRIENDS)	{
		destroyFriend(other_friend);
		return STUD_NOT_FRIEND;
	}
	assert(status == FRIENDS);
	return removeFriendShip(student1, other_friend, other_student, iterator);
}

//-----------------------------------------------------------------------------
studentResult removeFriend(Student current, Student student_to_remove) {
	Friend friend_to_remove = createFriend(student_to_remove, NOT_FRIENDS);
	if (friend_to_remove == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	setRemove(current->friends, friend_to_remove);
	destroyFriend(friend_to_remove);
	return STUD_SUCCESS;
}
//-----------------------------------------------------------------------------

int getStudentId(Student student1) {
    return student1->id;
}

//-----------------------------------------------------------------------------

studentResult studentAddGrade(Student student1, int semester, int course_id,
	char* points, int certain_grade) {
    bool valid_grade = isGradeValid(semester, course_id, points, certain_grade);
    if (!valid_grade)   {
        return STUD_INVALID_PARAMETERS;
    }
	Grade newGrade = createGrade(semester, course_id, points, certain_grade);
	if (newGrade == NULL) {
		return STUD_OUT_OF_MEMORY;
	}
	ListResult list_result = listInsertLast(student1->grade_sheet, newGrade);
    DestroyGrade(newGrade);
	if (list_result == LIST_OUT_OF_MEMORY) {
		return STUD_OUT_OF_MEMORY;
}
	assert(list_result == LIST_SUCCESS);
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------


studentResult studentUpdateGrade(Student student1,int course_id,int new_grade){
	int semester = findLastSemester(student1, course_id);
	if (semester == COURSE_NOT_EXIST) {
		return STUD_COURSE_DOES_NOT_EXIST;
	}
    if (!isCeratinGradeValid(new_grade))    {
        return STUD_INVALID_PARAMETERS;
    }
	Grade changed_grade = findLastGrade(student1, course_id, semester);
	updateGrade(changed_grade, new_grade);
	return STUD_SUCCESS;
}
//-----------------------------------------------------------------------------

studentResult studentRemoveGrade(Student student1,int course_id,int semester) {
    Grade last_grade = findLastGrade(student1, semester, course_id);
    if (last_grade == NULL) {
        return STUD_COURSE_DOES_NOT_EXIST;
    }
    Grade ptr = listGetFirst(student1->grade_sheet);
    while ((ptr != last_grade)&&(ptr != NULL)) {
        ptr = listGetNext(student1->grade_sheet);
    }
    listRemoveCurrent(student1->grade_sheet);
    return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------


studentResult printFullReport(Student student1, FILE* output_channel)  {
    mtmPrintStudentInfo(output_channel, student1->id, student1->first_name,
						student1->last_name);
    int gradesheet_length  = listGetSize(student1->grade_sheet);
    int* all_semesters = malloc(sizeof(int)*gradesheet_length);
	if (all_semesters ==NULL)	{
		return STUD_OUT_OF_MEMORY;
	}
    int count_semesters = findAllSemesters(student1->grade_sheet,all_semesters);
    handlePrintFormat(student1, all_semesters, count_semesters, output_channel);
    free (all_semesters);
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------

void handlePrintFormat(Student student1, int* all_semesters,
					   int count_semesters, FILE* output_channel) {
    int certain_semester, course_id, grade, points;
    int total_points_x2,failed_points_x2,effective_points_x2,effective_grade_x2;
    int sum_total_points_x2 = 0, sum_failed_points_x2 = 0;
    int sum_effective_points_x2 = 0, sum_effective_grade_x2 = 0;
    for (int i = 0; i<count_semesters; i++) {
        total_points_x2 = 0, failed_points_x2 = 0;
        effective_points_x2 = 0, effective_grade_x2 = 0;
        certain_semester = all_semesters[i];
        //For each grade of this semester,
		//print it and update parameters accordingly.
        LIST_FOREACH(Grade, ptr, student1->grade_sheet) {
            if (getGradeSemester(ptr) == certain_semester)  {
                //Fill parameters for semester summary
                course_id = getGradeCourseID(ptr);
                grade = getGradeActualGrade(ptr);
                points = getGradePointsMultiplied(ptr);
                total_points_x2 = total_points_x2 + points;
                if (grade < 55) {
                    failed_points_x2 += points;
                }
                //Update parameters according to grades validation.
                //Course valid for semester will increase only semester points.
                //Course valid for gradesheet will increase gradesheet points.
                effectiveResult is_effective = isEffectiveGradeSemester
						(ptr, student1->grade_sheet);
				updateEffectiveCounters (is_effective, points, grade,
						&effective_points_x2,&effective_grade_x2,
						&sum_effective_points_x2,&sum_effective_grade_x2);
                mtmPrintGradeInfo(output_channel, course_id, points, grade);
            }
        }
        sum_total_points_x2 = sum_total_points_x2 + total_points_x2;
        sum_failed_points_x2 = sum_failed_points_x2 + failed_points_x2;
        mtmPrintSemesterInfo(output_channel, certain_semester, total_points_x2,
                     failed_points_x2, effective_points_x2, effective_grade_x2);
    }
    mtmPrintSummary(output_channel, sum_total_points_x2, sum_failed_points_x2,
                    sum_effective_points_x2, sum_effective_grade_x2);
}

//-----------------------------------------------------------------------------

/*
 * Helper function for printCleanReport.
 * Compare grades by : #1 : course ID, #2 : semester
 */
static int compareGradeById (Element grade1, Element grade2, ListSortKey key){
	key = NULL;
	int course_id1 = getGradeCourseID(grade1);
	int course_id2 = getGradeCourseID(grade2);
	int compare_grade_result = course_id1 - course_id2;
	if (compare_grade_result == 0)	{
		int semester1 = getGradeSemester(grade1);
		int semester2 = getGradeSemester(grade2);
		int compare_semester_result = semester1 - semester2;
		return compare_semester_result;
	}
	return compare_grade_result;
}

/*
 * Input : student, output channel
 * Output (to output channel) :
 * - student's details
 * - all student valid grades. Sorted by : #1 course_id, #2 semester
 * - summary for all grades
 */
studentResult printCleanReport(Student student1, FILE* output_channel) {
    int course_id, points, grade;
	mtmPrintStudentInfo(output_channel, student1->id, student1->first_name,
						student1->last_name);
	List effective_list = listFilter(student1->grade_sheet, isGradeEffective,
									 student1->grade_sheet);
	if (effective_list == NULL)	{
		return STUD_OUT_OF_MEMORY;
	}
	ListResult result = listSort(effective_list, compareGradeById, NULL);
	if (result == LIST_OUT_OF_MEMORY)	{
		return STUD_OUT_OF_MEMORY;
	}
	int sum_total_points_x2 = 0, sum_failed_points_x2 = 0;      //TBR
	int sum_effective_points_x2 = 0, sum_effective_grade_x2 = 0;
	LIST_FOREACH(Grade, ptr, effective_list)	{
		course_id = getGradeCourseID(ptr);
		grade = getGradeActualGrade(ptr);
		points = getGradePointsMultiplied(ptr);
		mtmPrintGradeInfo(output_channel, course_id, points, grade);
		updateParameters(points, grade, &sum_total_points_x2,
						 &sum_effective_points_x2,
						 &sum_effective_grade_x2, &sum_failed_points_x2);
	}
	mtmPrintCleanSummary(output_channel, sum_effective_points_x2,
						 sum_effective_grade_x2);
	listDestroy(effective_list);
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------
studentResult printBestReport(Student student1, int isBest,
							  FILE* output_channel, int amount) {
	if (amount == 0){
		return STUD_SUCCESS;
	}
	List effective_list = listFilter(student1->grade_sheet, isGradeEffective,
									 student1->grade_sheet);
	if (effective_list == NULL){
		return STUD_OUT_OF_MEMORY;
	}
	ListResult result = listSort(effective_list, compareGrades, &isBest);
	if (result == LIST_OUT_OF_MEMORY)	{
		return STUD_OUT_OF_MEMORY;
	}
	LIST_FOREACH(Grade, ptr, effective_list) {	
		mtmPrintGradeInfo(output_channel, getGradeCourseID(ptr),
			getGradePointsMultiplied(ptr), getGradeActualGrade(ptr));
		amount--;
		if (amount==0){
			break;
		}
	}
	listDestroy(effective_list);
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------
studentResult askReference(FILE* output, Student student1,
		int course_id, int amount) {
	int* array_of_grades = calloc(amount, sizeof(int));
	Student* array_of_students = calloc(amount,sizeof(Student));
	SET_FOREACH(Friend, ptr, student1->friends) {
		Student student_friend = getStudentFromFriend(ptr);
		List filtered_list = listFilter(student_friend->grade_sheet,
			filterByCourse, &course_id);
		if (filtered_list == NULL) {
			return STUD_OUT_OF_MEMORY;
		}
		int student_grade = getHighestGrade(filtered_list);
		if (student_grade<array_of_grades[amount - 1]) {
			continue;
		}
		if ((student_grade == array_of_grades[amount - 1])&&
			(array_of_students[amount-1]!=NULL)) {
			if (student_friend->id < array_of_students[amount - 1]->id) {
				continue;
			}
		}
		sortArrayOfFriendByGrades(student_grade,amount ,array_of_grades,
			array_of_students, student_friend);
		listDestroy(filtered_list);
	}
	for (int i = 0; i < amount; i++) {
		if (array_of_students[i]== NULL){
			break;
		}
		mtmPrintStudentName(output, array_of_students[i]->first_name,
			array_of_students[i]->last_name);
	}
	free(array_of_grades);
	free(array_of_students);
	
	return STUD_SUCCESS;
}

//-----------------------------------------------------------------------------

studentResult addFacultyRequest (Student student1,int course_id,char* request){
	if ((strcmp(request, "remove_course") == 0)&&
			(findLastSemester(student1,course_id) == COURSE_NOT_EXIST))	{
			return STUD_COURSE_DOES_NOT_EXIST;
	}
    if (!isRequestValid(request))   {
        return STUD_INVALID_PARAMETERS;
    }
	return STUD_SUCCESS;
}


//-----------------------------------------------------------------------------

/*
 * ----FOR INTERNAL TESTS ONLY----
 * Input : student
 * Output : print all his details
 */
void printStudent (Student student1)    {
    printf("%d %s %s ", student1->id,student1->first_name,student1->last_name);
    LIST_FOREACH(Grade, ptr, student1->grade_sheet) {
        printGrade(ptr);
    }
    SET_FOREACH(Friend, ptr2, student1->friends) {
        printFriend(ptr2);
    }
}

//-----------------------------------------------------------------------------

void destroyStudent (Element student) {
	Student student1 = student;
    free(student1->first_name);
    free(student1->last_name);
    setDestroy(student1->friends);
    listDestroy(student1->grade_sheet);
    free (student1);
}