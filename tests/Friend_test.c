#include <stdlib.h>
#include <stdio.h>
#include "test_utilities.h"
#include "../Student.h"
#include "../Friend.h"

#define SECOND_BIGGER -1
#define EQUAL 0
#define FIRST_BIGGER 1

static bool testCreateFriend() {
    //Data
    studentResult student_result;
    Student student1 = createStudent(311378947, "Avishay", "Avisror",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    //Tests cases
	Friend friend1 = createFriend(student1, NOT_FRIENDS);
    ASSERT_TEST(friend1 != NULL);
	destroyFriend(friend1);
	friend1 = createFriend(student1, FRIENDSHIP_REQUESTED);
	ASSERT_TEST(friend1 != NULL);
	destroyFriend(friend1);
	friend1 = createFriend(student1, WAITING_FOR_YOU);
	ASSERT_TEST(friend1 != NULL);
	destroyFriend(friend1);
	friend1 = createFriend(student1, BOTH_REQUESTED);
	ASSERT_TEST(friend1 != NULL);
	destroyFriend(friend1);
	friend1 = createFriend(student1, FRIENDS);
	ASSERT_TEST(friend1 != NULL);
	destroyFriend(friend1);
    ASSERT_TEST(createFriend(NULL, NOT_FRIENDS) == NULL);
    ASSERT_TEST(createFriend(NULL, FRIENDSHIP_REQUESTED) == NULL);
    ASSERT_TEST(createFriend(NULL, WAITING_FOR_YOU) == NULL);
    ASSERT_TEST(createFriend(NULL, BOTH_REQUESTED) == NULL);
    ASSERT_TEST(createFriend(NULL, FRIENDS) == NULL);
	destroyStudent(student1);
    return true;
}

static bool testCompareFriend() {
    //Data
    studentResult student_result;
    Student student1 = createStudent(311378947, "Isha", "Goodman",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    Friend friend1 = createFriend(student1, NOT_FRIENDS);
    Student student2 = createStudent(203207881, "Isha", "Goodman",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    Friend friend2 = createFriend(student2, NOT_FRIENDS);
    Student student3 = createStudent(311378947, "Isha", "Goodman",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    Friend friend3 = createFriend(student3, NOT_FRIENDS);

    //Test cases
    ASSERT_TEST(compareFriend(friend1, friend2) == FIRST_BIGGER);
    ASSERT_TEST(compareFriend(friend1, friend3) == EQUAL);
    ASSERT_TEST(compareFriend(friend1, friend1) == EQUAL);
    ASSERT_TEST(compareFriend(friend2, friend1) == SECOND_BIGGER);
	destroyStudent(student1);
	destroyStudent(student2);
	destroyStudent(student3);
	destroyFriend(friend1);
	destroyFriend(friend2);
	destroyFriend(friend3);
    return true;
}

static bool testCopyFriend() {
    //Data
    studentResult student_result;
    Student student1 = createStudent(311378947, "Avishay", "Avisror",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    Friend friend1 = createFriend(student1, NOT_FRIENDS);
    Friend friend2 = copyFriend(friend1);
    //Test cases
    ASSERT_TEST(compareFriend(friend1, friend2) == EQUAL);
	destroyStudent(student1);
	destroyFriend(friend1);
	destroyFriend(friend2);
    return true;
}

//Tests both getStudentFromFriend + changeFriendshipStatus functions.
//Assumption : friend is not NULL (designed like this in student)
static bool testGetCurrentFriendStatus() {
    //Data
    studentResult student_result;
    Student student1 = createStudent(311378947, "Avishay", "Avisror",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    //Tests cases
    Friend friend1 = createFriend(student1, NOT_FRIENDS);
    ASSERT_TEST(getCurrentFriendStatus(friend1) == NOT_FRIENDS);
    changeFriendshipStatus(friend1, FRIENDSHIP_REQUESTED);
    ASSERT_TEST(getCurrentFriendStatus(friend1) == FRIENDSHIP_REQUESTED);
    changeFriendshipStatus(friend1, WAITING_FOR_YOU);
    ASSERT_TEST(getCurrentFriendStatus(friend1) == WAITING_FOR_YOU);
    changeFriendshipStatus(friend1, BOTH_REQUESTED);
    ASSERT_TEST(getCurrentFriendStatus(friend1) == BOTH_REQUESTED);
    changeFriendshipStatus(friend1, FRIENDS);
    ASSERT_TEST(getCurrentFriendStatus(friend1) == FRIENDS);
	destroyStudent(student1);
	destroyFriend(friend1);
    return true;
}

static bool testGetStudentFromFriend() {
    //Data
    studentResult student_result;
    Student student1 = createStudent(311378947, "Avishay", "Avisror",
                                     &student_result, copyGrade, DestroyGrade,
                                     copyFriend, destroyFriend, compareFriend);
    Friend friend1 = createFriend(student1, NOT_FRIENDS);
    //Test cases
    ASSERT_TEST(getStudentFromFriend(friend1) != NULL);
    ASSERT_TEST(getStudentFromFriend(NULL) == NULL);
	destroyStudent(student1);
	destroyFriend(friend1);
    return true;
}

int main() {
    RUN_TEST(testCreateFriend);
    RUN_TEST(testCompareFriend);
    RUN_TEST(testCopyFriend);
    RUN_TEST(testGetCurrentFriendStatus);
    RUN_TEST(testGetStudentFromFriend);
    return 0;
}