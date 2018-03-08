#include <stdlib.h>
#include <stdio.h>
#include "test_utilities.h"
#include "../Student.h"
#include "../Friend.h"
#include "../Manager.h"

#define BILLION 1000000000
#define ZERO 0
#define VALID_ID 311378947
#define SECOND_BIGGER -1
#define EQUAL 0
#define FIRST_BIGGER 1

#define STUDENT_RESULT studentResult student_result
#define MANAGER_RESULT managerResult manager_result
#define CREATE_MANAGER  \
    Manager manager1 = createManager(&manager_result,    \
    copyStudent, compareStudents, destroyStudent)
#define ADD_STUDENT addStudent(manager1, 203207881, "isha", "goodman")
#define ADD_STUDENT_2 addStudent(manager1, 311378947, "Avishay", "Avisror")
#define ADD_STUDENT_3 addStudent(manager1, 111222111, "Yehuda", "Golan")
#define ADD_STUDENT_4 addStudent(manager1,222333222, "Yahav", "Gabbay")
#define STUDENT_LOGIN studentLogIn(manager1, 203207881)
#define STUDENT2_LOGIN studentLogIn(manager1, 311378947)
#define STUDENT3_LOGIN studentLogIn(manager1, 111222111)
#define STUDENT4_LOGIN studentLogIn(manager1, 222333222)
#define STUDENT_LOGOUT studentlogOut(manager1)
#define ADD_GRADES_TO_LOGGED_IN_STUDENT managerAddGrade \
            (manager1,7,112233,"4.5",100);\
managerAddGrade(manager1, 3, 390011, "4", 60);\
managerAddGrade(manager1, 6, 390011, "4", 100);\
managerAddGrade(manager1, 5, 390011, "4", 50);\
managerAddGrade(manager1, 5, 390011, "4", 70);\
managerAddGrade(manager1, 2, 222222, "3", 50);\
managerAddGrade(manager1, 1, 777777, "3", 54);\
managerAddGrade(manager1, 1, 111111, "3", 70);\
managerAddGrade(manager1, 2, 111111, "3", 50);\
managerAddGrade(manager1, 4, 222222, "3", 60);\
managerAddGrade(manager1, 2, 222222, "3", 70);

#define ADD_GRADES_STUDENT2  \
managerAddGrade(manager1, 7, 112233, "4.5", 20);     \
managerAddGrade(manager1, 3, 112233, "4.5", 60);    \
managerAddGrade(manager1, 6, 112233, "4.5", 30);   \
managerAddGrade(manager1, 5, 112233, "4.5", 70);    \

#define ADD_GRADES_STUDENT3  \
managerAddGrade(manager1, 7, 112233, "4.5", 30);     \
managerAddGrade(manager1, 3, 112233, "4.5", 80);    \
managerAddGrade(manager1, 6, 112233, "4.5", 30);   \
managerAddGrade(manager1, 5, 112233, "4.5", 20);    \

static bool testCreateManager() {
    MANAGER_RESULT;
    CREATE_MANAGER;
    ASSERT_TEST(manager1 != NULL);
    destroyManager(manager1);
    return true;
}

//Both function are tested together in order to allow common test cases
static bool testAddRemoveStudent()    {
    MANAGER_RESULT;
    CREATE_MANAGER;
    manager_result = addStudent(manager1, -7, "avishay", "elihau");
    ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
    manager_result = addStudent(manager1, 31137894, "avishay", "elihau");
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = addStudent(manager1, 31137894, "Yehuda", "elihau");
    ASSERT_TEST(manager_result == MANAGER_STUDENT_ALREADY_EXISTS);
    manager_result = addStudent(manager1, BILLION, "Avi", "Avi");
    ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
    manager_result = removeStudent(manager1, 111222);
    ASSERT_TEST(manager_result == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_result = removeStudent(manager1, 31137894);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    destroyManager(manager1);
    return true;
}

//Login and logout are tested together
static bool testStudentLogInAndOut()    {
    MANAGER_RESULT;
    CREATE_MANAGER;
    ADD_STUDENT;
    manager_result = studentLogIn(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = studentLogIn(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_ALREADY_LOGGED_IN);
    manager_result = studentLogIn(manager1, 222222222);
    ASSERT_TEST(manager_result == MANAGER_ALREADY_LOGGED_IN);
    manager_result = studentlogOut(manager1);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = studentlogOut(manager1);
    ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
    manager_result = studentLogIn(manager1, 222222222);
    ASSERT_TEST(manager_result == MANAGER_STUDENT_DOES_NOT_EXIST);
    destroyManager(manager1);
    return true;
}

//ManagerAddFriendRequest, ManagerHandleRequest, ManagerUnfriend
//are tested together to handle all relevant test cases
static bool testManagerFriendsIssues()  {
    MANAGER_RESULT;
    CREATE_MANAGER;
    ADD_STUDENT;
    ADD_STUDENT_2;
    STUDENT_LOGIN;
    manager_result = managerAddFriendRequest(manager1, 311378947);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = managerAddFriendRequest(manager1, 311378947);
    ASSERT_TEST(manager_result == MANAGER_ALREADY_REQUESTED);
    STUDENT_LOGOUT;
    STUDENT2_LOGIN;
    manager_result = managerAddFriendRequest(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = managerAddFriendRequest(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_ALREADY_REQUESTED);
    manager_result = managerHandleRequest(manager1, 11111111, "reject");
    ASSERT_TEST(manager_result == MANAGER_STUDENT_DOES_NOT_EXIST);
    manager_result = managerHandleRequest(manager1, 203207881, "nothing!");
    ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
    manager_result = managerHandleRequest(manager1, 203207881, "reject");
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = managerHandleRequest(manager1, 203207881, "accept");
    ASSERT_TEST(manager_result == MANAGER_NOT_REQUESTED);
    STUDENT_LOGOUT;
    STUDENT_LOGIN;
    manager_result = managerHandleRequest(manager1, 311378947, "accept");
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = managerHandleRequest(manager1, 311378947, "reject");
    ASSERT_TEST(manager_result == MANAGER_ALREADY_FRIEND);
    manager_result = managerHandleRequest(manager1, 203207881, "reject");
    ASSERT_TEST(manager_result == MANAGER_ALREADY_FRIEND);
    manager_result = managerUnfriend(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_NOT_FRIEND);
    manager_result = managerUnfriend(manager1, 311378947);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    manager_result = managerUnfriend(manager1, 111111111);
    ASSERT_TEST(manager_result == MANAGER_STUDENT_DOES_NOT_EXIST);
    STUDENT_LOGOUT;
    manager_result = managerAddFriendRequest(manager1, 203207881);
    ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
    manager_result = managerHandleRequest(manager1, 311378947, "accept");
    ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
    manager_result = managerUnfriend(manager1, 311378947);
    ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
	destroyManager(manager1);
    return true;
}

static bool testManagerPrintBestReport()    {
    MANAGER_RESULT;
    CREATE_MANAGER;
    ADD_STUDENT;
    STUDENT_LOGIN;
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
    //test best reports
    manager_result = managerPrintBestReport(manager1, -2, 1, stdout);
    ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
    printf("\nPrintBest Output :\n");
    manager_result = managerPrintBestReport(manager1, 4, 1, stdout);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * PrintBest Output :
        Course: 390011          Points: 4.0             Grade: 100 << Wow!
        Course: 112233          Points: 4.5             Grade: 100 << Wow!
        Course: 390011          Points: 4.0             Grade: 70
        Course: 390011          Points: 4.0             Grade: 60
     */
    //test worst reports
    manager_result = managerPrintBestReport(manager1, -2, 0, stdout);
    ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
    printf("\nPrintWorst Output :\n");
    manager_result = managerPrintBestReport(manager1, 12, 0, stdout);
    ASSERT_TEST(manager_result == MANAGER_SUCCESS);
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
    STUDENT_LOGOUT;
    manager_result = managerPrintBestReport(manager1, 1, 0, stdout);
    ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
    destroyManager(manager1);
    return true;
}

static bool testManagerAddAndRemoveGrade() {
    //test data
    MANAGER_RESULT;
	CREATE_MANAGER;
    ADD_STUDENT;
    // test cases
    // tests managerAddGrade
    ASSERT_TEST(managerAddGrade(manager1, 5, 7, "3.5", 80)
                == MANAGER_NOT_LOGGED_IN);
    STUDENT_LOGIN;
    ASSERT_TEST(managerAddGrade(manager1, 5, -1, "3.5", 80)
                == MANAGER_INVALID_PARAMETER);
    ASSERT_TEST(managerAddGrade(manager1, 5, 7, "3.5", 80)
                == MANAGER_SUCCESS);
    //tests managerUpdateGrade
    ASSERT_TEST(managerUpdateGrade(manager1, 7, -1)
                == MANAGER_INVALID_PARAMETER);
    STUDENT_LOGOUT;
    ASSERT_TEST(managerUpdateGrade(manager1, 1, 20)
                == MANAGER_NOT_LOGGED_IN);
    STUDENT_LOGIN;
    ASSERT_TEST(managerUpdateGrade(manager1, 1, 20)
                == MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(managerUpdateGrade(manager1, 7, 20) == MANAGER_SUCCESS);
    //managerRemoveGrade
    ASSERT_TEST(managerRemoveGrade(manager1, 1, 20)
                == MANAGER_COURSE_DOES_NOT_EXIST);
    STUDENT_LOGOUT;
    ASSERT_TEST(managerUpdateGrade(manager1, 1, 20) == MANAGER_NOT_LOGGED_IN);
    STUDENT_LOGIN;
    ASSERT_TEST(managerUpdateGrade(manager1, 7, 20) == MANAGER_SUCCESS);
    destroyManager(manager1);
    return true;
}

static bool testManagerPrintFullReport() {
	MANAGER_RESULT;
	CREATE_MANAGER;
	ADD_STUDENT;
	manager_result = managerPrintFullReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
	STUDENT_LOGIN;
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
	manager_result = managerPrintFullReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected Output :
     * Grade sheet of isha goodman, id 203207881
    ========================================
    Course: 777777          Points: 3.0             Grade: 54
    Course: 111111          Points: 3.0             Grade: 70
    ========================================
    Semester 1              Points: 6.0             Average: 62.00          Success: 50%

    Course: 222222          Points: 3.0             Grade: 50
    Course: 111111          Points: 3.0             Grade: 50
    Course: 222222          Points: 3.0             Grade: 70
    ========================================
    Semester 2              Points: 6.0             Average: 60.00          Success: 33%

    Course: 390011          Points: 4.0             Grade: 60
    ========================================
    Semester 3              Points: 4.0             Average: 60.00          Success: 100%

    Course: 222222          Points: 3.0             Grade: 60
    ========================================
    Semester 4              Points: 3.0             Average: 60.00          Success: 100%

    Course: 390011          Points: 4.0             Grade: 50
    Course: 390011          Points: 4.0             Grade: 70
    ========================================
    Semester 5              Points: 4.0             Average: 70.00          Success: 50%

    Course: 390011          Points: 4.0             Grade: 100 << Wow!
    ========================================
    Semester 6              Points: 4.0             Average: 100.00         Success: 100%

    Course: 112233          Points: 4.5             Grade: 100 << Wow!
    ========================================
    Semester 7              Points: 4.5             Average: 100.00         Success: 100%

    Points: 25.5            Average: 73.02          Success: 66%
     */
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
	manager_result = managerPrintFullReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected Output :
     * same as Output #1, but all grades are duplicated.
     */
	destroyManager(manager1);
	return true;

}

static bool testManagerPrintCleanReport() {
	MANAGER_RESULT;
	CREATE_MANAGER;
	ADD_STUDENT;
	manager_result = managerPrintCleanReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
	STUDENT_LOGIN;
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
	manager_result = managerPrintCleanReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected Output :
     * Grade sheet of isha goodman, id 203207881
        ========================================
        Course: 111111          Points: 3.0             Grade: 50
        Course: 112233          Points: 4.5             Grade: 100 << Wow!
        Course: 222222          Points: 3.0             Grade: 60
        Course: 390011          Points: 4.0             Grade: 60
        Course: 390011          Points: 4.0             Grade: 70
        Course: 390011          Points: 4.0             Grade: 100 << Wow!
        Course: 777777          Points: 3.0             Grade: 54
        Points: 25.5            Average: 9.69           Success: 76%
     */
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
	manager_result = managerPrintCleanReport(manager1, stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected output : same as output #1
     * (All grades are duplicated but it should not affect clean report)
     */
	destroyManager(manager1);
	return true;
}

static bool testManagerFacultyRequest() {
	MANAGER_RESULT;
	CREATE_MANAGER;
	ADD_STUDENT;
	manager_result = managerFacultyRequest(manager1,
                                           123123, "remove_course", stdout);
	ASSERT_TEST(manager_result == MANAGER_NOT_LOGGED_IN);
    STUDENT_LOGIN;
	manager_result = managerFacultyRequest(manager1, 123123, "", stdout);
	ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
	manager_result = managerFacultyRequest(manager1,
                                           123123, "remove_course", stdout);
	ASSERT_TEST(manager_result == MANAGER_COURSE_DOES_NOT_EXIST);
	manager_result = managerFacultyRequest(manager1, 123123, "", stdout);
	ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
	manager_result = managerFacultyRequest(manager1, 123123, "fasf", stdout);
	ASSERT_TEST(manager_result == MANAGER_INVALID_PARAMETER);
	manager_result = managerFacultyRequest(manager1,
                                           123123, "cancel_course", stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
	/*
	 * Expected output :
	 * Faculty response: your request was rejected
	 */
	manager_result = managerFacultyRequest(manager1,
                                           123123, "register_course", stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected output :
     * Faculty response: your request was rejected
     */
	managerAddGrade(manager1, 7, 112233, "4.5", 100);
	manager_result = managerFacultyRequest(manager1,
                                           112233, "remove_course", stdout);
	ASSERT_TEST(manager_result == MANAGER_SUCCESS);
    /*
     * Expected output :
     * Faculty response: your request was rejected
     */
	destroyManager(manager1);
	return true;
}

static bool testManagerAskReferences() {
	MANAGER_RESULT;
	CREATE_MANAGER;
	ADD_STUDENT;
	ADD_STUDENT_2;
	ADD_STUDENT_3;
	ADD_STUDENT_4;
	STUDENT_LOGIN;
	ADD_GRADES_TO_LOGGED_IN_STUDENT;
	ASSERT_TEST(managerAddFriendRequest(manager1, 222333222)== MANAGER_SUCCESS);
	STUDENT_LOGOUT;
	STUDENT2_LOGIN;
	ADD_GRADES_STUDENT2;
	ASSERT_TEST(managerAddFriendRequest(manager1, 222333222)==MANAGER_SUCCESS);
	STUDENT_LOGOUT;
	STUDENT3_LOGIN;
	ADD_GRADES_STUDENT3;
	ASSERT_TEST(managerAddFriendRequest(manager1, 222333222)==MANAGER_SUCCESS);
	STUDENT_LOGOUT;
	ASSERT_TEST(managerAskReference(manager1, stdout, 112233, 5)
                == MANAGER_NOT_LOGGED_IN);
	ASSERT_TEST(managerAskReference(manager1, stdout, 112233, -1)
                == MANAGER_NOT_LOGGED_IN);
	STUDENT4_LOGIN;
	printf("\naskReference Output #1 : SHOULD BE EMPTY\n");
	ASSERT_TEST(managerAskReference(manager1, stdout, 112233, 5)
                == MANAGER_SUCCESS);
	ASSERT_TEST(managerHandleRequest(manager1, 311378947, "accept")
                == MANAGER_SUCCESS);
	ASSERT_TEST(managerHandleRequest(manager1, 203207881, "accept")
                == MANAGER_SUCCESS);
	ASSERT_TEST(managerHandleRequest(manager1, 111222111, "accept")
                == MANAGER_SUCCESS);
	printf("\naskReference Output #2 : \n");
	ASSERT_TEST(managerAskReference(manager1, stdout, 112233, 5)
                == MANAGER_SUCCESS);
	/*
	* askReference expected Output #2 :
	Isha Goodman
	Avishay Avisror
	Yehuda Golan
	*/
	destroyManager(manager1);
	return true;
}

int main() {
    RUN_TEST(testCreateManager);
   RUN_TEST(testManagerAddAndRemoveGrade);
    RUN_TEST(testAddRemoveStudent);
    RUN_TEST(testStudentLogInAndOut);
    RUN_TEST(testManagerFriendsIssues);
    RUN_TEST(testManagerPrintBestReport);
	printf("\n\n\n\n\n\n\n");
	RUN_TEST(testManagerPrintFullReport);
	printf("\n\n\n\n\n\n\n");
    RUN_TEST(testManagerPrintCleanReport);
    RUN_TEST(testManagerFacultyRequest);
    RUN_TEST(testManagerAskReferences);

    return 0;
}