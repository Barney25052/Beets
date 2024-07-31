#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "taskList.h"
#include "taskTag.h"
#include "loader.c"
#include "saver.c"

#define EXIT_PROGRAM 0
#define NOTHING 1
#define VIEW_TASKS 2
#define COMPLETE_TASK 3
#define ADD_TASK 4
#define REMOVE_TASK 5

#define MAX_TASKS_PER_PAGE 4

#define CLEARLINE "\33[2K"  //erase the current line;
#define MOVECURSORUP "033[A"  //moves the cursor up one line
#define RETURNSTART "\r"  //goes to the start of the line

int currentLines = 0;
int currentPage = 0;

void consolePrint(char* text) {
    printf("%s", text);
    currentLines += 1;
}

typedef struct {
    char commandType;
    char* commandData;
    size_t commandDataLength;
} commandInfo;

commandInfo* commandInfoCreate(char commandType, char* commandData, size_t commandDataLength) {
    commandInfo* commandInfo = malloc(sizeof(commandInfo));
    commandInfo->commandType = commandType;
    commandInfo->commandData = commandData;
    commandInfo->commandDataLength = commandDataLength;
}

void clearScreen(int numberOfLines) {
    for(int i = 0; i < numberOfLines; i++) {
        printf("\r\33[2K\r\33[1A\r");
    }
    //printf("Clearing screen of %d lines\n", numberOfLines);
}

void printTaskPage(taskList* taskList, int page) {
    int startIndex = page * MAX_TASKS_PER_PAGE;
    int endIndex = startIndex + MAX_TASKS_PER_PAGE;
    endIndex = endIndex < taskList->count ? endIndex : taskList->count;
    taskListNode* currentTaskNode = taskList->head;
    for(int i = startIndex; i < endIndex; i++) {
        taskRecord* currentTask = currentTaskNode->data;
        char* taskText = taskPrint(taskListGetTask(taskList, i));
        printf("%d - %s\n", i, taskText);
        free(taskText);
        currentLines += 2;
        if(currentTask->hasDeadline) {
            currentLines ++;
        }
        currentTaskNode = currentTaskNode->next;
    }
}


void printCurrentScreen(taskList* taskList, int page) {
    int totalPages = taskList->count/MAX_TASKS_PER_PAGE + 1;
    //clearScreen(currentLines);
    currentLines = 0;
    printTaskPage(taskList, page);
    printf("\t\t<[p] Page %d/%d [n]>\n--------------------------------------------------\n>", page+1, totalPages);
    currentLines += 4;
}

void readLine(char* output, int maxSize) {
    maxSize--;
    char c = 'a';
    int count = 0;
    while(c != '\n') {
        scanf("%c", &c);
        if(count < maxSize && c != '\n') {
            output[count] = c;
            count++;
        }
    }
    output[count] = 0;
}

taskRecord* getTaskFromInput(taskList* taskList, bool mustReturnVal) {
    bool valid = false;
    int choice;
    do {
        printf("Please enter a task to mark complete: ");
        scanf("%d", &choice);
        if(choice < taskList->count) {
            valid = true;
        }
    } while(mustReturnVal && !valid);

    if(!valid) {
        printf("Invalid task selected!\n");
        return NULL;
    } else {
        return taskListGetTask(taskList, choice);
    }
}

//More advanced features
//Dates and time on tasks
//Tags/Categories
//Bash commands for all program functions
//Filtered searches
//Priority
//Optional metadata and links on tasks e.g homework file and description

const char* FILE_LOCATION = "/home/ryan-bell/Documents/Beets/notes.txt";

typedef struct {
    char* data;
    int size;
    bool isFinalPart;
} commandPart;

commandPart* commandPartCreate(char* data, int size, bool isFinalPart) {
    commandPart* commandPart = malloc(sizeof(commandPart));
    commandPart->data = data;
    commandPart->size = size;
    commandPart->isFinalPart = isFinalPart;
}

commandPart* readCommandPart() {
    char c;
    char* partData = calloc(64, sizeof(char));
    int partLength = 0;
    bool inText = false;
    while(true) {
        scanf("%c", &c);
        if(c == '"') {
            inText = !inText;
            continue;
        }
        if(c == '\n') {
            break;
        }
        if(c == ' ' && !inText) {
            break;
        }
        partData[partLength] = c;
        partLength++;
    }
    partData[partLength] = 0;
    partData = realloc(partData, partLength * sizeof(char));

    commandPart* commandPart = commandPartCreate(partData, partLength, c == '\n');
    return commandPart;
}

char** readCommand(int* size) {
    char** commandParts = malloc (8 * sizeof(char*));
    int currentLength = 0;
    bool readingCommand = true;

    while(readingCommand) {
        commandPart* currentPart = readCommandPart();
        if(currentPart->size > 0) {
            commandParts[currentLength] = currentPart->data;
            currentLength ++;
        }
        readingCommand = !currentPart->isFinalPart;
        free(currentPart);
    }
    commandParts = realloc(commandParts, currentLength);
    *size = currentLength;
    return commandParts;
}

commandInfo* parseCurrentCommand() {
    int commandPartsLength = 0;
    char** commandParts = readCommand(&commandPartsLength);
    if(commandPartsLength == 0) {
        return commandInfoCreate(NOTHING, NULL, 0);
    }

    char* typeAsText = commandParts[0];
    char type = NOTHING;

    if(strcmp("e", typeAsText) == 0 || strcmp("exit", typeAsText) == 0) {
        type = EXIT_SUCCESS;
        return commandInfoCreate(type, NULL, 0);
    }
    else if(strcmp("add", typeAsText) == 0 ) {
        type = ADD_TASK;
        return commandInfoCreate(type, commandParts[1], strlen(commandParts[1]));
    }
    else if(strcmp("view", typeAsText) == 0 || strcmp("v", typeAsText) == 0) {
        type = VIEW_TASKS;
        return commandInfoCreate(type, NULL, 0);
    }
    else if(strcmp("n", typeAsText) == 0) {
        type = VIEW_TASKS;
        char newPage = currentPage + 1;
        return commandInfoCreate(type, &newPage, 1);
    }
    else if(strcmp("p", typeAsText) == 0) {
        type = VIEW_TASKS;
        char newPage = currentPage - 1;
        if(newPage < 0) {
            newPage = 0;
        }
        return commandInfoCreate(type, &newPage, 1);
    }
    else if(strcmp("complete", typeAsText) == 0 || strcmp("x", typeAsText) == 0) {
        type = COMPLETE_TASK;
        return commandInfoCreate(type, commandParts[1], strlen(commandParts[1]));
    }
    else if(strcmp("remove", typeAsText) == 0 || strcmp("r", typeAsText) == 0) {
        type = REMOVE_TASK;
        return commandInfoCreate(type, commandParts[1], strlen(commandParts[1]));
    }
    return commandInfoCreate(-1, NULL, 0);
}

//TEMP FOR TESTING
void addTagsToTask(taskTagCollection* collection, taskList* taskList) {
    taskListNode* currentTask = taskList->head;
    for(int i = 0; i < taskList->count; i++) {
        if(i % 2 == 0) {
            currentTask = currentTask->next;
            continue;
        }
        taskAddTag(currentTask->data, collection->tags[0]);
        currentTask = currentTask->next;
    }
}

int main() {
    printf("Welcome to Beets!\n");

    //Opening file.

    taskList* taskList = taskListCreate();
    readFileIntoTaskList(FILE_LOCATION, taskList);
    
    taskTagCollection* tagCollection = tagCollectionCreate(0);
    tagCollectionLoadTag(tagCollection, "Homework");
    tagCollectionLoadTag(tagCollection, "Daily");
    addTagsToTask(tagCollection, taskList);

    commandInfo* command = commandInfoCreate(NOTHING, NULL, 0);
    int taskNumber;

    printCurrentScreen(taskList, 0);

    while(command->commandType != EXIT_PROGRAM) {

        int maxPages = (taskList->count/MAX_TASKS_PER_PAGE) + 1;
        command = parseCurrentCommand();
        switch(command->commandType) {
            case VIEW_TASKS:
                if(command->commandData != NULL) {
                    currentPage = *command->commandData;
                    if(currentPage >= maxPages-1) {
                        currentPage = maxPages-1;
                    } 
                    if(currentPage < 0) {
                        currentPage = 0;
                    }
                }
                break;
            case COMPLETE_TASK:
                sscanf(command->commandData, "%d", &taskNumber);
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    consolePrint("Invalid task number\n");
                    break;
                }
                taskRecord* record = taskListGetTask(taskList, taskNumber);
                taskSetComplete(record,!record->isComplete);
                saveData(FILE_LOCATION, taskList);
                break;
            case ADD_TASK:
                taskRecord* newTask = taskCreate(command->commandData);
                taskListPush(taskList, newTask);
                saveData(FILE_LOCATION, taskList);
                break;
            case REMOVE_TASK:
                sscanf(command->commandData, "%d", &taskNumber);
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    consolePrint("Invalid task number\n");
                    break;
                }
                taskListRemoveAtIndex(taskList, taskNumber);
                saveData(FILE_LOCATION, taskList);
                break;
            case EXIT_PROGRAM:
                return 0;
            default:
                consolePrint("Unkown command");
                break;
        }
        printCurrentScreen(taskList, currentPage);
        free(command);
    }


}