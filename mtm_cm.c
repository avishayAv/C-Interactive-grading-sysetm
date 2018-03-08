#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mtm_ex3.h"
#include "Manager.h"

#define TAB_ASCII 9
#define INPUT_CHAR "-i"
#define OUTPUT_CHAR "-o"
#define READ_ONLY "r"
#define WRITE_ONLY "w"
#define FREE_ALL

/*
 * Input : 2 strings (input+output), 2 FILE pointers
 * Process :
 * opening the input/output files if exists.
 * Output :
 * TRUE for success
 * FALSE for failure
 */
static bool openFiles(char* input, char* output, FILE** input_channel,
                      FILE** output_channel) {
    if (strcmp(output, "stdout") != 0) {
        *output_channel = fopen(output, WRITE_ONLY);
        if (*output_channel == NULL) {
            return false;
        }
    }
    if (strcmp(input, "stdin") != 0) {
        *input_channel = fopen(input, READ_ONLY);
        if (*input_channel == NULL) {
            if (strcmp(output, "stdout") != 0) {
                fclose(*output_channel);
            }
            return false;
        }
    }
    return true;
}

/*
 * Input : argc, argv, possible input and output
 * Process :
 * checks if the input in one of the following formats :
 * - (EMPTY).
 * - -i <txtfile>
 * - -o <txtfile>
 * - -i <txtfile> -o <txtfile> or the opposite
 * Output :
 * TRUE if format is valid, FALSE otherwise
 * input and output pointers will not contains channels accordingly.
 */
static bool checkInput(int argc, char** argv, char** input, char** output){
    //Check length
    if ((argc != 1)&&(argc != 3)&&(argc != 5))  {
        return false;
    }
    switch (argc) {
        case 1:
        default:
            return true;
        case 3:
            if ((strcmp(argv[1],INPUT_CHAR) != 0)
                &&(strcmp(argv[1],OUTPUT_CHAR) != 0)) {
                return false;
            }
            if (strcmp(argv[1],INPUT_CHAR) == 0)  {
                *input = argv[2];
            }
            else    {
                *output = argv[2];
            }
            return true;
        case 5:
            if ((strcmp(argv[1],INPUT_CHAR) == 0)
                &&(strcmp(argv[3],OUTPUT_CHAR) == 0))   {
                *input = argv[2];
                *output = argv[4];
                return true;
            }
            if ((strcmp(argv[3],INPUT_CHAR) == 0)
                &&(strcmp(argv[1],OUTPUT_CHAR) == 0))   {
                *input = argv[4];
                *output = argv[2];
                return true;
            }
            return false;
    }
    return true;
}

/*
 * Input : "pure" line, without spaces (buffer_clean), index for copy
 * Process : copy current word to word pointer
 * Output : index of next available word.
 */


static int wordToParameter(char* word, char* buffer_clean, int len, int i) {
    int k = 0;
    for (; i < len; i++) {
        if (buffer_clean[i] == ' ') {
            word[k] = '\0';
            return (i+1);
        }
        word[k] = buffer_clean[i];
        k++;
    }
    word[k] = '\0';
    return i;
}

/*
 * Input : line (buffer), parameters for output.
 * Process :
 * 1. remove spaces from beginning of the line
 * 2. remove duplicate spaces
 * 3. separate new "pure" line into words
 * Output :
 * command : the first word.
 * sub_command : the second word (if exist)
 * arg1 : the third word (if exist)
 * and so on...
 */
static void lineToParameters(char* buffer,char* command,char* sub_command,
                            char* arg1,char* arg2,char* arg3,char* arg4)   {
    int length = strlen(buffer);
    int j = 0, i;
    char buffer_clean[MAX_LEN] = "";
    //turn tabs into spaces
    for (i = 0; i < length; i++)    {
        if (buffer[i] == TAB_ASCII)  {
            buffer[i] = ' ';
        }
    }
    //Remove spaces from beginning
    for (i =0; i<length; i++)  {
        if (buffer[i] != ' ')   {
            break;
        }
    }
    //Remove spaces between words
    for (; i <(length - 2) ; i++) {
        if ((buffer[i] == ' ')&&(buffer[i+1] == ' '))   {
            continue;
        }
        buffer_clean[j] = buffer[i];
        j++;
    }
    if (buffer[length-2] != ' ')    {
        buffer_clean[j] = buffer[length-2];
        j++;
    }
    //
    i = 0;
    i = wordToParameter (command, buffer_clean, j, i);
    i = wordToParameter (sub_command, buffer_clean, j, i);
    i = wordToParameter (arg1, buffer_clean, j, i);
    i = wordToParameter (arg2, buffer_clean, j, i);
    i = wordToParameter (arg3, buffer_clean, j, i);
    i = wordToParameter (arg4, buffer_clean, j, i);
}

/*
 * Input : char which is actually a number
 * Output : integer of the same number
 */
static int stringToInt (char* argument) {
    int len = strlen(argument);
    int student_id= 0;
    for (int i=0; i<len; i++)  {
        student_id += (argument[i] - '0');
        if (i != (len-1)) {
            student_id = student_id * 10;
        }
    }
    return student_id;
}

//student functions handling
static managerResult runCommandStudent (Manager manager1, char* sub_command,
                                       char* arg1, char* arg2,char* arg3)   {
    int student_id = stringToInt(arg1);
    managerResult result;
    //CASE : student add <id> <first_name> <last_name>
    if (strcmp(sub_command, "add") == 0) {
        result = addStudent(manager1, student_id, arg2, arg3);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_STUDENT_ALREADY_EXISTS:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_ALREADY_EXISTS);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : student remove <id>
    if (strcmp(sub_command, "remove") == 0) {
        result = removeStudent(manager1, student_id);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_STUDENT_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : student login <id>
    if (strcmp(sub_command, "login") == 0) {
        result = studentLogIn(manager1, student_id);
        switch (result) {
            case MANAGER_ALREADY_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_ALREADY_LOGGED_IN);
                break;
            case MANAGER_STUDENT_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : student logout
    if (strcmp(sub_command, "logout") == 0) {
        result = studentlogOut(manager1);
        switch (result) {
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }

    }
    //CASE : student friend_request <id>
    if (strcmp(sub_command, "friend_request") == 0) {
        result = managerAddFriendRequest(manager1,student_id);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_STUDENT_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_DOES_NOT_EXIST);
                break;
            case MANAGER_ALREADY_FRIEND:
                mtmPrintErrorMessage(stderr, MTM_ALREADY_FRIEND);
                break;
            case MANAGER_ALREADY_REQUESTED:
                mtmPrintErrorMessage(stderr, MTM_ALREADY_REQUESTED);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : student handle_request <id> <action>
    if (strcmp(sub_command, "handle_request") == 0) {
        result = managerHandleRequest(manager1, student_id, arg2);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_STUDENT_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_DOES_NOT_EXIST);
                break;
            case MANAGER_ALREADY_FRIEND:
                mtmPrintErrorMessage(stderr, MTM_ALREADY_FRIEND);
                break;
            case MANAGER_NOT_REQUESTED:
                mtmPrintErrorMessage(stderr, MTM_NOT_REQUESTED);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : student unfriend <id>
    if (strcmp(sub_command, "unfriend") == 0) {
        result = managerUnfriend(manager1, student_id);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_NOT_FRIEND:
                mtmPrintErrorMessage(stderr, MTM_NOT_FRIEND);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_STUDENT_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_STUDENT_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    return MANAGER_SUCCESS;
}

//Grade sheet functions handling
static managerResult runCommandGradeSheet(Manager manager1,char* sub_command,
                                   char* arg1,char* arg2,char* arg3,char* arg4){
    managerResult result;
    int arg1_int = stringToInt(arg1);
    int arg2_int = stringToInt(arg2);
    int arg4_int = stringToInt(arg4);
    //CASE : grade_sheet add <semester> <course_id> <points> <grade>
    if (strcmp(sub_command, "add") == 0){
        result = managerAddGrade(manager1, arg1_int, arg2_int, arg3, arg4_int);
        switch (result) {
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : grade_sheet remove <semester> <course_id>
    if (strcmp(sub_command, "remove") == 0) {
        result = managerRemoveGrade(manager1, arg1_int, arg2_int);
        switch (result) {
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_COURSE_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_COURSE_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : grade_sheet update <course_id> <new_grade>
    if (strcmp(sub_command, "update") == 0) {
        result = managerUpdateGrade(manager1, arg1_int, arg2_int);
        switch (result) {
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_COURSE_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_COURSE_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    return MANAGER_SUCCESS;
}

//Grade sheet functions handling
static managerResult runCommandReport(Manager manager1,char* sub_command,
                                  char* arg1,char* arg2, FILE* output_channel) {
    managerResult result;
    int arg1_int = stringToInt(arg1);
    int arg2_int = stringToInt(arg2);
    //CASE : report full
    if (strcmp(sub_command, "full") == 0) {
        result = managerPrintFullReport(manager1, output_channel);
        switch (result) {
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
			case MANAGER_OUT_OF_MEMORY:
				mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
				return result;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : report clean
    if (strcmp(sub_command, "clean") == 0) {
        result = managerPrintCleanReport(manager1, output_channel);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : report best <amount>
    if (strcmp(sub_command, "best") == 0) {
        result = managerPrintBestReport(manager1,arg1_int,true,output_channel);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : report worst <amount>
    if (strcmp(sub_command, "worst") == 0) {
        result = managerPrintBestReport(manager1,arg1_int,false,output_channel);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : report reference <course_id> <amount>
    if (strcmp(sub_command, "reference") == 0) {
        result = managerAskReference(manager1,output_channel,arg1_int,arg2_int);
        switch (result) {
            case MANAGER_OUT_OF_MEMORY:
                mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
                return result;
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    //CASE : report faculty_request <course_id> <request>
    if (strcmp(sub_command, "faculty_request") == 0) {
        result = managerFacultyRequest(manager1, arg1_int,arg2, output_channel);
        switch (result) {
            case MANAGER_INVALID_PARAMETER:
                mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
                break;
            case MANAGER_NOT_LOGGED_IN:
                mtmPrintErrorMessage(stderr, MTM_NOT_LOGGED_IN);
                break;
            case MANAGER_COURSE_DOES_NOT_EXIST:
                mtmPrintErrorMessage(stderr, MTM_COURSE_DOES_NOT_EXIST);
                break;
            case MANAGER_SUCCESS:
            default:
                return result;
        }
    }
    return MANAGER_SUCCESS;
}


int main(int argc,char** argv) {
    //OK
    managerResult result;
    Manager manager1 = createManager(&result,
                                  copyStudent, compareStudents, destroyStudent);
    if (result == MANAGER_OUT_OF_MEMORY)    {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
    }

    char* input = "stdin";
    char* output = "stdout";
    FILE* input_channel = stdin;
    FILE* output_channel = stdout;

    if (!checkInput(argc, argv, &input, &output))   {
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
    if (!openFiles(input, output, &input_channel, &output_channel))   {
        return MTM_CANNOT_OPEN_FILE;
    }
    char buffer[MAX_LEN] = "";
    while (fgets(buffer, MAX_LEN, input_channel) != NULL)   {
        char command[MAX_LEN] = "";
        char sub_command[MAX_LEN] = "";
        char arg1[MAX_LEN] = "";
        char arg2[MAX_LEN] = "";
        char arg3[MAX_LEN] = "";
        char arg4[MAX_LEN] = "";

        lineToParameters(buffer,command,sub_command,arg1,arg2,arg3,arg4);
        if ((command[0]== '\0')||(command[0]== '#')){
            continue;
        }

        //Assumption : all command are valid (name + number of parameters)
        if (strcmp(command, "student") == 0)    {
            if (runCommandStudent(manager1, sub_command, arg1, arg2,
                                  arg3) == MANAGER_OUT_OF_MEMORY)   {
                break;
            }
        }
        if (strcmp(command, "grade_sheet") == 0)    {
            if (runCommandGradeSheet(manager1, sub_command, arg1, arg2, arg3,
                                     arg4) == MANAGER_OUT_OF_MEMORY)   {
                break;
            }
        }
        if (strcmp(command, "report") == 0)    {
            if (runCommandReport(manager1, sub_command, arg1, arg2,
                                 output_channel) == MANAGER_OUT_OF_MEMORY)   {
                break;
            }
        }
    }

    if (input_channel != stdin) {
        fclose(input_channel);
    }
    if (output_channel != stdout)   {
        fclose(output_channel);
    }
    destroyManager(manager1);
    return 0;
}