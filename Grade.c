#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "Grade.h"

#define MILLION 1000000
#define DIV_FOR_SPORTS 10000

//-----------------------------------------------------------------------------

struct grade {
    int semester;
    int course_id;
    char* points;
    int certain_grade;
};

//-----------------------------------------------------------------------------

/*
 * Input : points string
 * Output : is it valid
 * Expected format : X / X.0 / X.5
 */

static bool isPointsFormatValid (char* points)  {
    int length = strlen(points);
    int dot_index = 0;
    //0 means no point, other number is the index of point.
    //return false for more than one point.
    for (int i = 0; i < length; i++)    {
        if ((points[i] == '.')&&(dot_index != 0))   {
            return false;
        }
        if ((points[i] == '.')&&(dot_index == 0))   {
            dot_index = i;
        }
    }

    //Handle format X
    if (dot_index == 0) {
        for (int j = 0; j<length; j++)  {
            if ((points[j] < '0')||(points[j] > '9'))   {
                return false;
            }
        }
    }

    //Handle formats X.0 / X.5
    if (dot_index != 0) {
        for (int j = 0; j<dot_index; j++)  {
            if ((points[j] < '0')||(points[j] > '9'))   {
                return false;
            }
        }
        if ((points[dot_index+1] != '5')&&(points[dot_index+1] != '0')) {
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------


bool isCeratinGradeValid (int certain_grade)    {
    if ((certain_grade < 0)||(certain_grade > 100)) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------


bool isGradeValid (int semester,int course_id,char* points,int certain_grade)  {
    //Check semester
    if (semester <= 0)  {
        return false;
    }
    //Check course_id
    if ((course_id <= 0)||(course_id >= MILLION)) {
        return false;
    }
    //Check points format
    if (!isPointsFormatValid(points))   {
        return false;
    }
    //Check grade
    if (!isCeratinGradeValid(certain_grade))    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------

bool isCoursesEqual (Grade grade1, int other_course_id) {
    return (grade1->course_id == other_course_id);
}

//-----------------------------------------------------------------------------

int getGradeSemester (Grade grade1) {
    return grade1->semester;
}

//-----------------------------------------------------------------------------

int getGradeCourseID (Grade grade1) {
    return grade1->course_id;
}

//-----------------------------------------------------------------------------


bool isSportCourse (int course_id)   {
    int is_sport = course_id / DIV_FOR_SPORTS;
    if (is_sport == 39) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------


int getGradeActualGrade (Grade grade1) {
    return grade1->certain_grade;
}

//-----------------------------------------------------------------------------

int getGradePointsMultiplied (Grade grade1) {
    char* points = grade1->points;
    int length = strlen(points);
    int dot_index = 0;
    //0 means no point, other number is the index of point.
    //return false for more than one point.
    for (int i = 0; i < length; i++)    {
        if (points[i] == '.')   {
            dot_index = i;
        }
    }

    int number = points[0] - '0';

    //handle format without dot
    if (dot_index == 0) {
        for (int i = 1; i < length; i++)    {
            number = number * 10;
            number += points[i] - '0';
        }
        number = number * 2;
    }

    //handle format with dot
    if (dot_index > 0) {
        for (int i = 1; i < dot_index; i++)    {
            number = number * 10;
            number += points[i] - '0';
        }
        if (points[dot_index+1] == '5') {
            number = (number * 2) + 1;
        }
        else    {
            number = number * 2;
        }
    }

    return number;
}

//-----------------------------------------------------------------------------

Grade createGrade (int semester,int course_id,char* points,int certain_grade)  {
    Grade newGrade = malloc(sizeof(struct grade));
    if (newGrade == NULL)   {
        return NULL;
    }
    newGrade->semester = semester;
    newGrade->course_id = course_id;
    newGrade->points = malloc((sizeof(char)*(strlen(points)+1)));
    if (newGrade->points == NULL)   {
        free (newGrade);
        return NULL;
    }
    strcpy(newGrade->points, points);
    newGrade->certain_grade = certain_grade;
    return newGrade;
}

//-----------------------------------------------------------------------------


GradeResult updateGrade (Grade current, int new_grade)  {
    current->certain_grade = new_grade;
    return GRADE_SUCCESS;
}

//-----------------------------------------------------------------------------


Element copyGrade (Element element) {
    if (element == NULL)    {
        return NULL;
    }
    Grade copiedGrade = malloc(sizeof(*copiedGrade));
    if (copiedGrade == NULL)   {
        return NULL;
    }
    copiedGrade->semester = ((Grade)element)->semester;
    copiedGrade->course_id = ((Grade)element)->course_id;

    copiedGrade->points = malloc((sizeof(char)*
            (strlen(((Grade)element)->points)+1)));
    if (copiedGrade->points == NULL)   {
        free(copiedGrade);
        return NULL;
    }
    strcpy(copiedGrade->points, ((Grade)element)->points);
    copiedGrade->certain_grade = ((Grade)element)->certain_grade;
    return copiedGrade;
}

//-----------------------------------------------------------------------------


void DestroyGrade (Element current)   {
	free(((Grade)current)->points);
    free(current);
}

//-----------------------------------------------------------------------------

/*
 * Input : pointer for grade
 * Output : print all parameters
 * NOTE : for tests only
 */
void printGrade (Grade current) {
    printf("%d %d %s %d \n", current->semester, current->course_id,
           current->points, current->certain_grade);
}