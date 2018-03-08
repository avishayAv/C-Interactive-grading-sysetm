#include <stdlib.h>
#include <stdio.h>
#include "test_utilities.h"
#include "../Student.h"
#include "../Friend.h"
#include "../mtm_ex3.h"
#define BILLION 1000000000
#define ZERO 0
#define VALID_ID 311378947
#define SECOND_BIGGER -1
#define EQUAL 0
#define FIRST_BIGGER 1

#define CREATE_STUDENT_1  \
    createStudent(311378947, "Avishay", "Avisror", &student_result, copyGrade, \
    DestroyGrade, copyFriend, destroyFriend, compareFriend);
#define CREATE_STUDENT_2  \
    createStudent(203207881, "Isha", "Goodman", &student_result, copyGrade, \
DestroyGrade, copyFriend, destroyFriend, compareFriend);
#define CREATE_STUDENT_3  \
    createStudent(111222111, "Yehuda", "Golan", &student_result, copyGrade, \
DestroyGrade, copyFriend, destroyFriend, compareFriend);
#define CREATE_STUDENT_4    \
    createStudent(222333222, "Yahav", "Gabbay", &student_result, copyGrade, \
DestroyGrade, copyFriend, destroyFriend, compareFriend);

#define STUDENT_RESULT studentResult student_result;

#define ADD_GRADES_TO_STUDENT1 	studentAddGrade(student1, 7,112233,"4.5",100);\
 studentAddGrade(student1, 3, 390011, "4", 60);\
 studentAddGrade(student1, 6, 390011, "4", 100);\
studentAddGrade(student1, 5, 390011, "4", 50);\
studentAddGrade(student1, 5, 390011, "4", 70);\
studentAddGrade(student1, 2, 222222, "3", 50);\
studentAddGrade(student1, 1, 777777, "3", 54);\
studentAddGrade(student1, 1, 111111, "3", 70);\
studentAddGrade(student1, 2, 111111, "3", 50);\
studentAddGrade(student1, 4, 222222, "3", 60);\
studentAddGrade(student1, 2, 222222, "3", 70);

#define ADD_ANOTHER_GRADES_STUDENT1     \
studentAddGrade(student1, 7, 112233, "4.5", 100);     \
studentAddGrade(student1, 3, 390011, "4", 60);    \
studentAddGrade(student1, 6, 390011, "4", 100);   \
studentAddGrade(student1, 5, 390011, "4", 50);    \
studentAddGrade(student1, 5, 390011, "4", 70);    \
studentAddGrade(student1, 2, 222222, "3", 50);    \
studentAddGrade(student1, 1, 777777, "3", 54);    \
studentAddGrade(student1, 1, 111111, "3", 70);    \
studentAddGrade(student1, 2, 111111, "3", 50);    \
studentAddGrade(student1, 4, 222222, "3", 60);    \
studentAddGrade(student1, 2, 222222, "3", 70);

#define ADD_GRADES_STUDENT2  \
studentAddGrade(student2, 7, 112233, "4.5", 20);     \
studentAddGrade(student2, 3, 112233, "4.5", 60);    \
studentAddGrade(student2, 6, 112233, "4.5", 30);   \
studentAddGrade(student2, 5, 112233, "4.5", 70);    \

#define ADD_GRADES_STUDENT3  \
studentAddGrade(student3, 7, 112233, "4.5", 30);     \
studentAddGrade(student3, 3, 112233, "4.5", 80);    \
studentAddGrade(student3, 6, 112233, "4.5", 30);   \
studentAddGrade(student3, 5, 112233, "4.5", 20);    \

static bool testCreateStudent() {
    STUDENT_RESULT;
    Student student1 = createStudent(BILLION, "Avishay", "Avisror",
                             &student_result, copyGrade, DestroyGrade,
                             copyFriend, destroyFriend, compareFriend);
    ASSERT_TEST(student_result == STUD_INVALID_ID);
    ASSERT_TEST(student1 == NULL);
    student1 = createStudent(ZERO, "Avishay", "Avisror",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    ASSERT_TEST(student_result == STUD_INVALID_ID);
    ASSERT_TEST(student1 == NULL);
    student1 = CREATE_STUDENT_1;
    ASSERT_TEST(student_result == STUD_SUCCESS);
    ASSERT_TEST(student1 != NULL);
	destroyStudent(student1);
    return true;
}

static bool testCopyStudent()   {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    Student student2 = copyStudent(NULL);
    ASSERT_TEST(student2 == NULL);
    student2 = copyStudent(student1);
    ASSERT_TEST(compareStudents(student1, student2) == EQUAL);
    destroyStudent(student1);
    destroyStudent(student2);
    return true;
}

static bool testCompareStudent()    {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    Student student2 = CREATE_STUDENT_1;
    Student student3 = CREATE_STUDENT_2;
    ASSERT_TEST(compareStudents(student1, student2) == EQUAL);
    ASSERT_TEST(compareStudents(student1, student1) == EQUAL);
    ASSERT_TEST(compareStudents(student3, student1) == SECOND_BIGGER);
    ASSERT_TEST(compareStudents(student1, student3) == FIRST_BIGGER);
	destroyStudent(student1);
    destroyStudent(student2);
	destroyStudent(student3);
    return true;
}

//Assumption : functions' parameters are not NULL (handled by design)
//addFriendRequest, handleRequest, unfriend are tested together in order to test
//combined cases
static bool testFriendsFunctions()  {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    Student student2 = CREATE_STUDENT_2;
    //add friend test cases
    ASSERT_TEST(addFriendRequest(student1, student2) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student1, student2) == STUD_ALREADY_REQUESTED);
    ASSERT_TEST(addFriendRequest(student1, student1) == STUD_ALREADY_FRIEND);
    ASSERT_TEST(addFriendRequest(student2, student1) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student2, student1) == STUD_ALREADY_REQUESTED);
    //handle request test cases
    //Assumption : action is "reject" or "accept" (validation by student.c)
    ASSERT_TEST(handleRequest(student1, student2, "reject") == STUD_SUCCESS);
    ASSERT_TEST(handleRequest(student1, student2, "reject")
                == STUD_NOT_REQUESTED);
    ASSERT_TEST(handleRequest(student2, student1, "nothing")
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(handleRequest(student2, student1, "accept") == STUD_SUCCESS);
    ASSERT_TEST(handleRequest(student2,student1,"reject")==STUD_ALREADY_FRIEND);
    ASSERT_TEST(handleRequest(student1,student1,"reject")==STUD_ALREADY_FRIEND);
    //unfriend test cases
    ASSERT_TEST(unfriend(student1, student2) == STUD_SUCCESS);
    ASSERT_TEST(unfriend(student2, student1) == STUD_NOT_FRIEND);
    ASSERT_TEST(unfriend(student1, student1) == STUD_NOT_FRIEND);
    //check requests are clean again
    ASSERT_TEST(addFriendRequest(student1, student2) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student2, student1) == STUD_SUCCESS);
    destroyStudent(student1);
    destroyStudent(student2);
    return true;
}

static bool testGetStudentId()  {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    ASSERT_TEST(getStudentId(student1) == 311378947);
    destroyStudent(student1);
    return true;
}

static bool testAddAndRemoveGrade() {
    //test data
    STUDENT_RESULT
    Student student2 = CREATE_STUDENT_2;
    // test cases
    ASSERT_TEST(studentAddGrade(student2, 5, 7, "3.5", 80) == STUD_SUCCESS);
    ASSERT_TEST(studentAddGrade(student2, 5, 7, "3.5", -2)
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentAddGrade(student2, 5, 7, "3.5", 105)
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentAddGrade(student2, 5, 7, "11", 80)
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentAddGrade(student2, 5, -2, "3.5", 80)
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentAddGrade(student2, 0, 7, "3.5", 90)
                == STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentUpdateGrade(student2,20,0)==STUD_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(studentUpdateGrade(student2, 7, 115)==STUD_INVALID_PARAMETERS);
    ASSERT_TEST(studentUpdateGrade(student2, 7, 95) == STUD_SUCCESS);
    ASSERT_TEST(studentRemoveGrade(student2, 4, 7)==STUD_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(studentRemoveGrade(student2, 5, 3)==STUD_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(studentRemoveGrade(student2, 5, 7) == STUD_SUCCESS);
    destroyStudent(student2);
    return true;
}

static bool testPrintCleanReport() {
    STUDENT_RESULT;
	Student student1 = CREATE_STUDENT_1;
	ASSERT_TEST(printCleanReport(student1, stdout) == STUD_SUCCESS);
	ADD_GRADES_TO_STUDENT1;
	ASSERT_TEST(printCleanReport(student1, stdout) == STUD_SUCCESS);
	ADD_GRADES_TO_STUDENT1;
	ASSERT_TEST(printCleanReport(student1, stdout) == STUD_SUCCESS);
	destroyStudent(student1);
	return true;
}
static bool testPrintFullReport() {
	STUDENT_RESULT;
	Student student1 = CREATE_STUDENT_1;
	
	ADD_GRADES_TO_STUDENT1;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printFullReport(student1,stdout);
	ADD_GRADES_TO_STUDENT1;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printFullReport(student1, stdout);
	destroyStudent(student1);
	return true;
}

static bool testPrintBestReport()   {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    ADD_ANOTHER_GRADES_STUDENT1;
    printf("\nPrintBest empty report : next row should be empty. \n");
    ASSERT_TEST(printBestReport(student1, 1, stdout, 0) == STUD_SUCCESS);
    printf("\nPrintBest Output :\n");
    ASSERT_TEST(printBestReport(student1, 1, stdout, 4) == STUD_SUCCESS);
    /*
     * PrintBest Output :
        Course: 390011          Points: 4.0             Grade: 100 << Wow!
        Course: 112233          Points: 4.5             Grade: 100 << Wow!
        Course: 390011          Points: 4.0             Grade: 70
        Course: 390011          Points: 4.0             Grade: 60
     */
    printf("\nPrintWorst Output :\n");
    ASSERT_TEST(printBestReport(student1, -1, stdout, 15) == STUD_SUCCESS);
    /*
     * PrintWorst Output :
        Course: 111111          Points: 3.0             Grade: 50
        Course: 777777          Points: 3.0             Grade: 54
        Course: 390011          Points: 4.0             Grade: 60
        Course: 222222          Points: 3.0             Grade: 60
        Course: 390011          Points: 4.0             Grade: 70
        Course: 390011          Points: 4.0             Grade: 100 << Wow!
        Course: 112233          Points: 4.5             Grade: 100 << Wow!
     */
    destroyStudent(student1);
    return true;
}

static bool testAskReferences() {
    STUDENT_RESULT;
    Student student1 = CREATE_STUDENT_1;
    Student student2 = CREATE_STUDENT_2;
    Student student3 = CREATE_STUDENT_3;
    Student student4 = CREATE_STUDENT_4;
    ADD_ANOTHER_GRADES_STUDENT1;
    ADD_GRADES_STUDENT2;
    ADD_GRADES_STUDENT3;
    printf("\naskReference Output #1 : SHOULD BE EMPTY\n");
    ASSERT_TEST(askReference(stdout, student4, 112233, 5) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student4, student1) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student4, student2) == STUD_SUCCESS);
    ASSERT_TEST(addFriendRequest(student4, student3) == STUD_SUCCESS);
    ASSERT_TEST(handleRequest(student1, student4, "accept") == STUD_SUCCESS);
    ASSERT_TEST(handleRequest(student2, student4, "accept") == STUD_SUCCESS);
    ASSERT_TEST(handleRequest(student3, student4, "accept") == STUD_SUCCESS);
    printf("\naskReference Output #2 : \n");
    ASSERT_TEST(askReference(stdout, student4, 112233, 5) == STUD_SUCCESS);
    /*
     * askReference expected Output #2 :
     * Avishay Avisror
        Yehuda Golan
        Isha Goodman
     */
	destroyStudent(student1);
	destroyStudent(student2);
	destroyStudent(student3);
	destroyStudent(student4);
    return true;
}

static bool tstAddFacultyRequest() {
	STUDENT_RESULT;
	Student student1 = CREATE_STUDENT_1;
	student_result = addFacultyRequest(student1, 123456, "register_course");
	ASSERT_TEST(student_result == STUD_SUCCESS);
	student_result = addFacultyRequest(student1, 123456, "cancel_course");
	ASSERT_TEST(student_result == STUD_SUCCESS);
	student_result = addFacultyRequest(student1, 123456, "remove_course");
	ASSERT_TEST(student_result == STUD_COURSE_DOES_NOT_EXIST);
	ADD_GRADES_TO_STUDENT1;
	student_result = addFacultyRequest(student1, 123456, "remove_course");
	ASSERT_TEST(student_result == STUD_COURSE_DOES_NOT_EXIST);
	student_result = addFacultyRequest(student1, 112233, "remove_course");
	ASSERT_TEST(student_result == STUD_SUCCESS);
    student_result = addFacultyRequest(student1, 112233, "remove");
    ASSERT_TEST(student_result == STUD_INVALID_PARAMETERS);
	destroyStudent(student1);
	return true;
}

int main() {
    RUN_TEST(testCreateStudent);
    RUN_TEST(testCopyStudent);
    RUN_TEST(testCompareStudent);
    RUN_TEST(testFriendsFunctions);
    RUN_TEST(testGetStudentId);
    RUN_TEST(testAddAndRemoveGrade);
	RUN_TEST(testPrintCleanReport);
	RUN_TEST(testPrintFullReport);
    RUN_TEST(testPrintBestReport);
    RUN_TEST(testAskReferences);
	RUN_TEST(tstAddFacultyRequest);

    return 0;
}