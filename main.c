#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "taskList.h"
#include "loader.c"
#include "saver.c"

#define EXIT_PROGRAM 0
#define NOTHING 1
#define VIEW_TASKS 2
#define COMPLETE_TASK 3
#define ADD_TASK 4
#define REMOVE_TASK 5

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

void printAllTasks(taskList* taskList) {
    for(int i = 0; i < taskList->count; i++) {
        printf("%d - %s\n", i, taskPrint(taskListGetTask(taskList, i)));
    }
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

int main() {
    printf("Welcome to Beets!\n\nThese are your current tasks.\n------------------\n");

    //Opening file.

    taskList* taskList = taskListCreate();
    readFileIntoTaskList(FILE_LOCATION, taskList);
    commandInfo* command = commandInfoCreate(NOTHING, NULL, 0);
    int taskNumber;

    while(command->commandType != EXIT_PROGRAM) {

        command = parseCurrentCommand();
        switch(command->commandType) {
            case VIEW_TASKS:
                printAllTasks(taskList);
                break;
            case COMPLETE_TASK:
                sscanf(command->commandData, "%d", &taskNumber);
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    printf("Invalid task number\n");
                    break;
                }
                taskRecord* record = taskListGetTask(taskList, taskNumber);
                taskMarkComplete(record);
                saveData(FILE_LOCATION, taskList);
                break;
            case ADD_TASK:
                taskRecord* newTask = taskCreate(command->commandData);
                taskListPush(taskList, newTask);
                saveData(FILE_LOCATION, taskList);
                printAllTasks(taskList);
                break;
            case REMOVE_TASK:
                sscanf(command->commandData, "%d", &taskNumber);
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    printf("Invalid task number\n");
                    break;
                }
                taskListRemoveAtIndex(taskList, taskNumber);
                saveData(FILE_LOCATION, taskList);
                break;
            case EXIT_PROGRAM:
                return 0;
            default:
                printf("Unkown command\n");
                break;
            free(command);
        }
    }


}