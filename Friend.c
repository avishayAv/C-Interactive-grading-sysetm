#include "Student.h"
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include <stdlib.h>
#include <stdio.h>
#include "Friend.h"
#include <assert.h>

struct friend {
	Student friend_student;
	friendshipStatus status;
};

//-----------------------------------------------------------------------------

Friend createFriend(Student friend_student, friendshipStatus status) {
	if (friend_student==NULL)	{
		return NULL;
	}
	Friend new_friend = malloc(sizeof(*new_friend));
	if (new_friend == NULL) {
		return NULL;
	}
	new_friend->status = status;
	new_friend->friend_student = friend_student;
	return new_friend;
}

//-----------------------------------------------------------------------------

int compareFriend(Element friend1, Element friend2) {
	return compareStudents(((Friend)friend1)->friend_student,
		((Friend)friend2)->friend_student);
}
//-----------------------------------------------------------------------------

Element copyFriend(Element friend) {
	Friend copied_friend = malloc(sizeof(*copied_friend));
	if ((copied_friend == NULL)||(friend== NULL)){
		return NULL;
	}
	copied_friend->friend_student = ((Friend)friend)->friend_student;
	copied_friend->status = ((Friend)friend)->status;
	return copied_friend;
}

//-----------------------------------------------------------------------------

friendshipStatus getCurrentFriendStatus(Friend friend1) {
	assert(friend1 != NULL);
	return friend1->status;
}

//-----------------------------------------------------------------------------

Student getStudentFromFriend(Friend friend1) {
    if (friend1 == NULL)	{
		return NULL;
	}
	return friend1->friend_student;
}

//-----------------------------------------------------------------------------

void changeFriendshipStatus(Friend friend1, friendshipStatus new_status) {
	friend1->status = new_status;
}

//-----------------------------------------------------------------------------

void printFriend (Friend friend1)   {
    printf("\n %d \n", friend1->status);
}

//-----------------------------------------------------------------------------

void destroyFriend(Element friend1) {
	assert(friend1 != NULL);
	((Friend)friend1)->friend_student = NULL;
	free (friend1);
}
