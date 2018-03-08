# C-Interactive-grading-sysetm

Interactive grading system for students. Features :

#1 : adding a new student to the system
FORMAT : student add <id> <FirstName> <LastName>

#2 : removing student from the system
FORMAT : student remove <id>

#3 : login as a certain student / logout
FORMAT : student login <id> / student logout

#4 : sending a friend request to another student
FORMAT : student friend_request <other_id>

#5 : handling a request from another student
FORMAT : student handle_request <OtherId> <action>
POSSIBLE ACTIONS : accept, reject.

#6 : unfriend
FORMAT : student unfriend <OtherId>

#7 : adding a new grade to a certain student grade sheet
FORMAT : grade_sheet add <semester> <CourseId> <points> <grade>

#8 : removing a certain grade from a certain student grade sheet
FORMAT : grade_sheet remove <semester> <CourseId>

#9 : updating a certain grade from a certain student grade sheet
FORMAT : grade_sheet update <CourseId> <NewGrade>

#10 : get full grade sheet report
FORMAT : report full
Output : all student's grade, average, points, etc...

#11 : get clean grade sheet report
FORMAT : report clean
Output : only valid grades, average, points, etc... (F.I - if the student did MOED B - MOED A should be cleaned)

#12 : get best/worst grades for a certain course
FORMAT : report best <amount> / report worst <amount>
Output : <amount> of student's best/worst grades (course | grade)

#13 : get the best students of a certain course
FORMAT : report reference <CourseId> <amount>
Output : <amount> of the best students in this course (names)

NOTE : in order to use featuers 4-13, please follow :
- login as a certain student
- apply any features you want. this features will be affected on the logged in student
- logout

thanks for reading.
