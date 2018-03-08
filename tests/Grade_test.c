#include "../Manager.h"
#include "../set.h"
#include "../list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../Student.h"
#include "../Friend.h"
#include "../Grade.h"
#include "test_utilities.h"

static bool testIsCeratinGradeValid() {
	ASSERT_TEST(isCeratinGradeValid(100)==true);
	ASSERT_TEST(isCeratinGradeValid(-200) == false);
	ASSERT_TEST(isCeratinGradeValid(0) == true);
	ASSERT_TEST(isCeratinGradeValid(100) == true);
	return true;
}

static bool testIsGradeValid() {
	ASSERT_TEST(isGradeValid(0, 200, "5", 90) == false);
	ASSERT_TEST(isGradeValid(5, 1000000, "5", 90) == false);
	ASSERT_TEST(isGradeValid(5, 0, "5", 90) == false);
	ASSERT_TEST(isGradeValid(5, 200, "3.3", 90) == false);
	ASSERT_TEST(isGradeValid(5, 200, "AV", 90) == false);
	ASSERT_TEST(isGradeValid(50, 200, "5", -1) == false);
	ASSERT_TEST(isGradeValid(1, 200, "5", 0) == true);
	return true;
}


static bool testIsCoursesEqual() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	// test cases
	ASSERT_TEST(isCoursesEqual(grade1, -1) == false);
	ASSERT_TEST(isCoursesEqual(grade1, 200) == true);
    DestroyGrade(grade1);
	return true;
}

static bool testGetGradeSemester() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	// test cases
	ASSERT_TEST(getGradeSemester(grade1) == 1);
    DestroyGrade(grade1);
	return true;
}

static bool testGetGradeCourseID() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	// test cases
	ASSERT_TEST(getGradeCourseID(grade1) == 200);
    DestroyGrade(grade1);
	return true;
}

static bool tesIsSportCourse() {
	ASSERT_TEST(isSportCourse(400000) == false);
	ASSERT_TEST(isSportCourse(289999) == false);
	ASSERT_TEST(isSportCourse(399999) == true);
	ASSERT_TEST(isSportCourse(390000) == true);
	return true;
}

static bool testGetGradeActualGrade() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	// test cases
	ASSERT_TEST(getGradeActualGrade(grade1) == 0);
    DestroyGrade(grade1);
	return true;
}

static bool testGetGradePointsMultiplied() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	Grade grade2 = createGrade(1, 200, "3.5", 0);
	Grade grade3 = createGrade(1, 200, "0", 0);
	Grade grade4 = createGrade(1, 200, "4.0", 0);
	// test cases
	ASSERT_TEST(getGradePointsMultiplied(grade1) == 10);
	ASSERT_TEST(getGradePointsMultiplied(grade2) == 7);
	ASSERT_TEST(getGradePointsMultiplied(grade3) == 0);
	ASSERT_TEST(getGradePointsMultiplied(grade4) == 8);
    DestroyGrade(grade1);
    DestroyGrade(grade2);
    DestroyGrade(grade3);
    DestroyGrade(grade4);

	return true;
}
static bool testCreateGrade(){
	Grade grade1 = createGrade(1, 200, "4.0", 0);
	ASSERT_TEST(getGradeCourseID(grade1) == 200);
	ASSERT_TEST(getGradeSemester(grade1) == 1);
	ASSERT_TEST(getGradeActualGrade(grade1) == 0);
    DestroyGrade(grade1);
	return true;
}
static bool testUpdateGrade() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	// test cases
	ASSERT_TEST(updateGrade(grade1, 50) == GRADE_SUCCESS);
	ASSERT_TEST(getGradeActualGrade(grade1) == 50);
    DestroyGrade(grade1);
	return true;
}

static bool testCopyGrade() {
	//test data
	Grade grade1 = createGrade(1, 200, "5", 0);
	Grade grade2 = copyGrade(grade1);
	// test cases
	ASSERT_TEST(copyGrade(NULL) == NULL);
	ASSERT_TEST(getGradeActualGrade(grade2) == 0);
	ASSERT_TEST(getGradePointsMultiplied(grade2) == 10);
    DestroyGrade(grade1);
    DestroyGrade(grade2);
	return true;
}

int main(){
	RUN_TEST(testIsCeratinGradeValid);
	RUN_TEST(testIsGradeValid);
	RUN_TEST(testIsCoursesEqual);
	RUN_TEST(testGetGradeSemester);
	RUN_TEST(testGetGradeCourseID);
	RUN_TEST(tesIsSportCourse);
	RUN_TEST(testGetGradeActualGrade);
	RUN_TEST(testGetGradePointsMultiplied);
	RUN_TEST(testCreateGrade);
	RUN_TEST(testUpdateGrade);
	RUN_TEST(testCopyGrade);
	return 0;
}