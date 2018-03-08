#ifndef FRIEND_H_
#define FRIEND_H_

typedef struct friend* Friend;

typedef enum {
	WAITING_FOR_YOU,
	FRIENDSHIP_REQUESTED,
	BOTH_REQUESTED,
	FRIENDS,
	NOT_FRIENDS
} friendshipStatus;

/* Allocates a new friend.
* input: new friend = the student which will be the new friend
* friendship status of the new friendship
* return:
* NULL -  NULL if allocations failed.
* A new Friend in case of success.
*/
Friend createFriend(Student new_friend, friendshipStatus status);

//-----------------------------------------------------------------------------

/**
* input: friend1, friend2 are friends to compare.
* the comparison is made by the id values of the Student in the friends.
* return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
* 		A negative integer if the second element is greater.
*/
int compareFriend(Element friend1, Element friend2);

//-----------------------------------------------------------------------------

/*
*copy an element of friend by all his parameters to a new element of friend
* input: Friend to copy
* return: NULL if allocation fail or if friend = NULL.
* else: return pointer to copied friend.
*/
Element copyFriend(Element friend);

//-----------------------------------------------------------------------------
/*input: pointer to friend
 * Assumption : friend is not NULL (tested in student ADT)
* output: the status of the current friend
*/
friendshipStatus getCurrentFriendStatus(Friend friend1);

//-----------------------------------------------------------------------------
/*input: friend
* output: the student that friend hold.
*/
Student getStudentFromFriend(Friend friend1);

//-----------------------------------------------------------------------------
/*input: friend, new friendship status to this friend
* Assumption : friend is not NULL (tested in student ADT)
* process: insert new status in the right place in friend
*/
void changeFriendshipStatus(Friend friend1, friendshipStatus new_status);

//-----------------------------------------------------------------------------

void printFriend (Friend friend1);

//-----------------------------------------------------------------------------
/*
* destroyFriend: Deallocate an existing friend.
* input: Friend - Target friend to be deallocate.
 * If Friend is NULL nothing will be done
*/
void destroyFriend(Element friend1);




#endif /*FRIEND_H_*/
