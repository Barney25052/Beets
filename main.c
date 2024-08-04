//More advanced features
//Tags/Categories
//Bash commands for all program functions
//Filtered searches
//Priority
//Optional metadata and links on tasks e.g homework file and description

#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "taskList.h"
#include "taskTag.h"
#include "loader.c"
#include "saver.c"
#include "console.h"

#define EXIT_PROGRAM 'e'
#define NOTHING ' '
#define VIEW_TASKS 'v'
#define COMPLETE_TASK 'x'
#define ADD_TASK 'a'
#define REMOVE_TASK 'r'

#define MAX_TASKS_PER_PAGE 4

int currentPage = 0;
const char* FILE_LOCATION = "/home/ryan-bell/Documents/Beets/notes.txt";

struct commandInfo {
    char commandType;
    char text[65];
    int number;
    //char extraData[65];
};
typedef struct commandInfo commandInfo;

void commandInfoSetText(commandInfo* command, char* text) {
    int length = strlen(text);
    if(length == 0) {
        return;
    }
    int i;
    if(length > 64) {
        length = 64;
    }
    for(i = 0; i < length; i++) {
        command->text[i] = text[i];
    }
    command->text[i] = 0;
}

void printTaskPage(taskList* taskList, int page) {
    int startIndex = page * MAX_TASKS_PER_PAGE;
    int endIndex = startIndex + MAX_TASKS_PER_PAGE;
    endIndex = endIndex < taskList->count ? endIndex : taskList->count;
    taskListNode* currentTaskNode = taskListGetNode(taskList, startIndex);
    for(int i = startIndex; i < endIndex; i++) {
        taskRecord* currentTask = currentTaskNode->data;
        char* taskText = taskPrint(currentTask);
        printLine("%d - %s\n", i, taskText);
        free(taskText);
        currentTaskNode = currentTaskNode->next;
    }
}

void printCurrentScreen(taskList* taskList, int page) {
    int totalPages = taskList->count/MAX_TASKS_PER_PAGE + 1;
    //clearScreen();
    printTaskPage(taskList, page);
    printLine("\t\t<[p] Page %d/%d [n]>\n--------------------------------------------------\n>", page+1, totalPages);
}

//Must be freed by caller
char* readLine() {
    char* buffer = malloc(128 * sizeof(char));
    int currentSize = 128;
    char currentChar = 1;
    int index = 0;

    while(currentChar != '\n') {
        currentChar = getchar();
        buffer[index] = currentChar;
        index ++;

        if(index >= currentSize) {  //Resize if too small
            currentSize += 128;
            buffer = realloc(buffer, currentSize * sizeof(char));
        }
    }

    buffer[index] = 0;
    buffer = realloc(buffer, (index+1) * sizeof(char));
    return buffer;
}

char** splitUserInput(char* userInput, int* numberOfParts) {
    int numberOfSpaces = 0;
    int userInputLength = strlen(userInput);
    bool nonSpaceCharacter = false;
    bool inText = false;
    char currentChar;
    char** splitInput;

    if(userInputLength == 0) {
        *numberOfParts = 0;
        return NULL;
    }

    for(int i = 0; i < userInputLength; i++) {
        currentChar = userInput[i];
        if(currentChar != ' ' && currentChar != '\n') {
            nonSpaceCharacter = true;
            if(currentChar == '"') {
                inText = !inText;
            }
        }
        else if(nonSpaceCharacter && !inText) {
            numberOfSpaces++;
            nonSpaceCharacter = false;
        }
    }

    if(inText) {
        *numberOfParts = 0;
        return NULL;
    }

    if(!nonSpaceCharacter) {
        numberOfSpaces --;
    }
    *numberOfParts = numberOfSpaces+1;
    splitInput = malloc(*numberOfParts * sizeof(char*)); //NOT FREED
    nonSpaceCharacter = false;

    int j = 0;
    inText = false;

    for(int i = 0; i < *numberOfParts; i++) {
        splitInput[i] = malloc(128 * sizeof(char)); //NOT FREED
        currentChar = userInput[j];
        int k = 0;
        while((userInput[j] != ' ' || inText) && userInput[j] != '\n') {
            if(userInput[j] == '"') {
                inText = !inText;
                j++;
                continue;
            }
            splitInput[i][k] = userInput[j];
            j ++;
            k ++;
        }
        while(userInput[j] == ' ') {
            j++;
        }
        splitInput[i][k] = 0;
        splitInput[i] = realloc(splitInput[i], (k+1)*sizeof(char));
        //printf("%d " , charIndex);
    }
    
    return splitInput;
}

char tokenizeCommand(char* commandWord) {
    if(!commandWord) {
        return 0;
    }
    if(strlen(commandWord) == 0) {
        return 0;
    }

    if(strcmp(commandWord, "add") == 0 || strcmp(commandWord, "new") == 0|| strcmp(commandWord, "a") == 0) {
        return ADD_TASK;
    }
    if(strcmp(commandWord, "rm") == 0 || strcmp(commandWord, "remove") == 0 || strcmp(commandWord, "r") == 0 || strcmp(commandWord, "del") == 0 || strcmp(commandWord, "delete") == 0) {
        return REMOVE_TASK;
    }
    if(strcmp(commandWord, "complete") == 0 || strcmp(commandWord, "x") == 0 || strcmp(commandWord, "mark") == 0) {
        return COMPLETE_TASK;
    }
    if(strcmp(commandWord, "view") == 0 || strcmp(commandWord, "v") == 0) {
        return VIEW_TASKS;
    }
    if(strcmp(commandWord, "exit") == 0 || strcmp(commandWord, "e") == 0 || strcmp(commandWord, "quit") == 0 || strcmp(commandWord, "q") == 0) {
        return EXIT_PROGRAM;
    }
    return NOTHING;
}

void generateCommand(commandInfo* commandInfo, char** commandParts, int numberOfParts) {
    char* commandWord = commandParts[0];
    char commandToken = tokenizeCommand(commandWord);
    commandInfo->commandType = commandToken;

    switch(commandToken) {
        case ADD_TASK:
            int year = 0;
            int month = 0;
            int day = 0;
            int hour = 0;
            int minute = 0;
            char* taskText = "\0";
            switch(numberOfParts) {
                case 7:
                    minute = atoi(commandParts[6]);
                case 6:
                    hour = atoi(commandParts[5]);
                case 5:
                    day = atoi(commandParts[4]);
                case 4:
                    month = atoi(commandParts[3]);
                case 3:
                    year = atoi(commandParts[2]);
                case 2:
                    taskText = commandParts[1];
                    commandInfo->number = timeCreate(year,month,day,hour,minute);
                    commandInfoSetText(commandInfo, taskText);
                    break;
                case 1:
                    commandInfo->commandType = NOTHING;
            }
            break;
        case REMOVE_TASK:
            //TODO add remove by tag
            if(numberOfParts != 2) {
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case COMPLETE_TASK:
            if(numberOfParts != 2) {
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case VIEW_TASKS:
            if(numberOfParts != 2) {
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            break;
    }

}

void getCommandFromUser(commandInfo* command) {
    int numberOfParts = 0;

    char* userInputLine = readLine();

    char** splitInput = splitUserInput(userInputLine, &numberOfParts);
    free(userInputLine);

    generateCommand(command, splitInput, numberOfParts);
    for(int i = 0; i < numberOfParts; i++) {
        if(!splitInput[i]) {
            continue;
        }
        free(splitInput[i]);
    }
    free(splitInput);
}
int main() {
    printf("Welcome to Beets!\n\n");
    //Opening file.

    taskList* taskList = taskListCreate();
    readFileIntoTaskList(FILE_LOCATION, taskList);
    
    taskTagCollection* tagCollection = tagCollectionCreate(0);
    tagCollectionLoadTag(tagCollection, "Homework");
    tagCollectionLoadTag(tagCollection, "Daily");
    
    int taskNumber;
    printCurrentScreen(taskList, 0);

    printf(">");
    commandInfo* command = malloc(sizeof(commandInfo));
    command->commandType = NOTHING;
    while(command->commandType != EXIT_PROGRAM) {
        getCommandFromUser(command);
        int maxPages = (taskList->count/MAX_TASKS_PER_PAGE) + 1;
        switch(command->commandType) {
            case VIEW_TASKS:
                currentPage = command->number;
                if(currentPage >= maxPages-1) {
                    currentPage = maxPages-1;
                } 
                if(currentPage < 0) {
                    currentPage = 0;
                }
                break;
            case COMPLETE_TASK:
                taskNumber = command->number;
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                taskRecord* record = taskListGetTask(taskList, taskNumber);
                taskSetComplete(record,!record->isComplete);
                saveData(FILE_LOCATION, taskList);
                break;
            case ADD_TASK:
                taskRecord* newTask = taskCreate(command->text);
                if(command->number > 0) {
                    taskSetDeadline(newTask, command->number);
                }
                taskListPush(taskList, newTask);
                saveData(FILE_LOCATION, taskList);
                break;
            case REMOVE_TASK:
                taskNumber = command->number;
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                taskListRemoveAtIndex(taskList, taskNumber);
                saveData(FILE_LOCATION, taskList);
                break;
            case EXIT_PROGRAM:
                break;
            default:
                printLine("Unknown command");
                break;
        }
        printCurrentScreen(taskList, currentPage);
    }

    free(command);
    taskListClean(taskList);
    taskTagCollectionClean(tagCollection);
}