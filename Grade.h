#ifndef EX3_WET_GRADE_H
#define EX3_WET_GRADE_H

#include <stdbool.h>

//-----------------------------------------------------------------------------

typedef enum {
    GRADE_SUCCESS,
    GRADE_OUT_OF_MEMORY
} GradeResult;

typedef struct grade* Grade;

typedef void* Element;

//-----------------------------------------------------------------------------

/*
 * Input : certain grade
 * Output : TRUE if valid, FALSE if not.
 */
bool isCeratinGradeValid (int certain_grade);

//-----------------------------------------------------------------------------

/*
 * Input : semester, course_id, points and grade.
 * Output : TRUE - if grade is valid, FALSE - if not.
 * Tested Validations :
 - semester > 0
 - Course ID between 0 to million (and not equal both)
 - Points : format X / X.0 / X.5
 - Grade : between or equal 0 to 100
*/
bool isGradeValid (int semester, int course_id, char* points, int grade);

//-----------------------------------------------------------------------------

/*
 * Input : grade, a certain course id
 * Output : check if the course ID equals the course ID in the grade.
 */
bool isCoursesEqual (Grade grade1, int other_course_id);

//-----------------------------------------------------------------------------

/*
 * Input : grade
 * Output : it's semester
 */
int getGradeSemester (Grade grade1);

//-----------------------------------------------------------------------------

/*
 * Input : grade
 * Output : it's course ID
 */
int getGradeCourseID (Grade grade1);

//-----------------------------------------------------------------------------

/*
 * Input : grade
 * Output : is it sports or not (sports starting with 39xxxx)
 */
bool isSportCourse (int course_id);

//-----------------------------------------------------------------------------

/*
 * Input : Grade (STRUCT)
 * Output : the actual grade (int)
 */
int getGradeActualGrade (Grade grade1);

//-----------------------------------------------------------------------------

/*
 * Input : Grade (STRUCT)
 * Output : points as integer (originally it's char*) multiplied by 2
 */
int getGradePointsMultiplied (Grade grade1);

//-----------------------------------------------------------------------------

/*
 * Input : semester, course_id, points and grade.
 * Assumption : all the parameters are valid (checked by isGradeValid)
 * Process : created new struct grade and returns pointer to it.
 * Output : pointer to the new grade. NULL if can't allocate.
 */
Grade createGrade (int semester, int course_id, char* points, int grade);

//-----------------------------------------------------------------------------

/*
 * Input : pointer to grade and a new certain_grade
 * Assumption : all the parameters are valid (checked by isGradeValid)
 * Process : change grade's certain_grade to new_grade
 * Output : GRADE_SUCCESS for success (no other option)
 */
GradeResult updateGrade (Grade current, int new_grade);

//-----------------------------------------------------------------------------

/*
 * Input : a certain grade (as void*)
 * Used for linked list pointer for function.
 * Output : a copy of this grade (as void*).
 * if the original copy is NULL - return NULL.
 */
Element copyGrade (Element element);

//-----------------------------------------------------------------------------

/*
 * Input : pointer to grade
 * Process : destroy allocation grade
 */
void DestroyGrade (Element current);

//-----------------------------------------------------------------------------

/*
 * Input : pointer for grade
 * Output : print all parameters
 * NOTE : for tests only
 */
void printGrade (Grade current);

#endif //EX3_WET_GRADE_H
