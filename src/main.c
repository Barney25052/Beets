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

#define EXIT_PROGRAM 'q'
#define NOTHING ' '
#define VIEW_TASKS 'v'
#define COMPLETE_TASK 'x'
#define UNCOMPLETE_TASK 'h'
#define ADD_TASK 'a'
#define REMOVE_TASK 'r'
#define EDIT_TASK 'e'
#define DATE_TASK 'd'
#define NEXT 'n'
#define PREV 'p'
#define CREATETAG 'c'
#define ADDTAG 't'
#define DELETETAG '-'
#define UNTAG 'u'
#define FILTER 'f'
#define SORT 's'

#define MAX_TASKS_PER_PAGE 5

int currentPage = 0;
const char* FILE_LOCATION = "/home/ryan-bell/Documents/Beets/notes.txt";
const char* TAG_LOCATION = "/home/ryan-bell/Documents/Beets/tags.txt";

int currentNumberOfPages = 0;
int numberOfFilteredTasks;
taskTag* currentFilter;

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

int countFilteredTasks(taskList* taskList) {
    if(currentFilter == NULL) {
        return taskList->count;
    }
    taskListNode* currentNode = taskListGetNode(taskList, 0);
    int runningCount = 0;
    for(int i = 0; i < taskList->count; i++) {
        if(tagCollectionContainsTag(currentNode->data->tags, currentFilter)) {
            runningCount ++;
        }
        currentNode = currentNode->next;
    }
    return runningCount;
}

void printTaskPage(taskList* taskList, int page) {
    int startIndex = currentFilter == NULL ? page * MAX_TASKS_PER_PAGE : 0;
    int filterIndex = 0;
    int endIndex = startIndex + MAX_TASKS_PER_PAGE;
    int maxTasks = countFilteredTasks(taskList);
    endIndex = endIndex < maxTasks ? endIndex : maxTasks;
    numberOfFilteredTasks = maxTasks;
    taskListNode* currentTaskNode = taskListGetNode(taskList, startIndex);
    for(int i = startIndex; i < endIndex; i++) {
        if(currentTaskNode == NULL) {
            break;
        }
        taskRecord* currentTask = currentTaskNode->data;
        if(tagCollectionContainsTag(currentTask->tags, currentFilter)) {
            if(currentFilter != NULL && filterIndex < MAX_TASKS_PER_PAGE * page) {
                filterIndex++;
                endIndex ++;
            } else {
                char* taskText = taskPrint(currentTask);
                printLine("%d - %s\n", i, taskText);
                free(taskText);
            }
        } else {
            endIndex++;
        }
        currentTaskNode = currentTaskNode->next;
    }
}

void printCurrentScreen(taskList* taskList, int page) {
    clearScreen();
    printf("\e[0;37m");
    printLine("---------------------BEETS------------------------\n\n");
    printTaskPage(taskList, page);
    currentNumberOfPages = (numberOfFilteredTasks/MAX_TASKS_PER_PAGE) + (numberOfFilteredTasks % MAX_TASKS_PER_PAGE == 0 ? 0 : 1);
    printLine("----------------<[p] Page %d/%d [n]>----------------\n\e[0;35m>", page+1, currentNumberOfPages);
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
    splitInput = malloc(*numberOfParts * sizeof(char*));
    nonSpaceCharacter = false;

    int j = 0;
    inText = false;

    for(int i = 0; i < *numberOfParts; i++) {
        splitInput[i] = malloc(128 * sizeof(char)); 
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

    //This feels wholly inefficient but I dont know a better way to do it
    if(strcmp(commandWord, "add") == 0 || strcmp(commandWord, "new") == 0|| strcmp(commandWord, "a") == 0) {
        return ADD_TASK;
    }
    if(strcmp(commandWord, "rm") == 0 || strcmp(commandWord, "remove") == 0 || strcmp(commandWord, "r") == 0 || strcmp(commandWord, "del") == 0 || strcmp(commandWord, "delete") == 0) {
        return REMOVE_TASK;
    }
    if(strcmp(commandWord, "complete") == 0 || strcmp(commandWord, "x") == 0 || strcmp(commandWord, "mark") == 0) {
        return COMPLETE_TASK;
    }
    if(strcmp(commandWord, "reset") == 0 || strcmp(commandWord, "rst") == 0 || strcmp(commandWord, "undo") == 0 || strcmp(commandWord, "u") == 0) {
        return UNCOMPLETE_TASK;
    }
    if(strcmp(commandWord, "edit") == 0 || strcmp(commandWord, "e") == 0) {
        return EDIT_TASK;
    }
    if(strcmp(commandWord, "view") == 0 || strcmp(commandWord, "v") == 0) {
        return VIEW_TASKS;
    }
    if(strcmp(commandWord, "exit") == 0 || strcmp(commandWord, "e") == 0 || strcmp(commandWord, "quit") == 0 || strcmp(commandWord, "q") == 0) {
        return EXIT_PROGRAM;
    }
    if(strcmp(commandWord, "datetask") == 0 || strcmp(commandWord, "dt") == 0 || strcmp(commandWord, "date") == 0) {
        return DATE_TASK;
    }
    if(strcmp(commandWord, "next") == 0 || strcmp(commandWord, "n") == 0) {
        return NEXT;
    }
    if(strcmp(commandWord, "prev") == 0 || strcmp(commandWord, "p") == 0) {
        return PREV;
    }
    if(strcmp(commandWord, "createtag") == 0 || strcmp(commandWord, "tc") == 0 || strcmp(commandWord, "tagcreate") == 0) {
        return CREATETAG;
    }
    if(strcmp(commandWord, "tagadd") == 0 || strcmp(commandWord, "tag") == 0 || strcmp(commandWord, "ta") == 0 || strcmp(commandWord, "addtag") == 0 || strcmp(commandWord, "t") == 0) {
        return ADDTAG;
    }
    if(strcmp(commandWord, "untag") == 0) {
        return UNTAG;
    }
    if(strcmp(commandWord, "filter") == 0 || strcmp(commandWord, "f") == 0) {
        return FILTER;
    }
    
    return NOTHING;
}

void generateCommand(commandInfo* commandInfo, char** commandParts, int numberOfParts) {
    char* commandWord = commandParts[0];
    char commandToken = tokenizeCommand(commandWord);
    commandInfo->commandType = commandToken;


    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;

    switch(commandToken) {
        case ADD_TASK:
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
                default:
                    commandInfo->commandType = NOTHING;
            }
            break;
        case REMOVE_TASK:
            //TODO add remove by tag
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case EDIT_TASK:
            if(numberOfParts != 3) {
                commandInfo->commandType = NOTHING;
                break;
            }            
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            commandInfoSetText(commandInfo, commandParts[2]);
            break;
        case COMPLETE_TASK:
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case UNCOMPLETE_TASK:
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case VIEW_TASKS:
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number > 0) {
                commandInfo->number -= 1;
            }
            break;
        case NEXT:
            commandInfo->commandType = VIEW_TASKS;
            commandInfo->number = currentPage + 1;
            break;
        case PREV:
            commandInfo->commandType = VIEW_TASKS;
            commandInfo->number = currentPage - 1;
            break;
        case DATE_TASK:
            int taskNumber = 0;
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
                    taskNumber = atoi(commandParts[1]);
                    commandInfo->number = timeCreate(year,month,day,hour,minute);
                    if(strcmp(commandParts[2], "remove") == 0 ) {
                        commandInfo->number = -2;
                    }
                    if(taskNumber == 0 && strcmp(commandParts[1], "0") != 0) {
                        commandInfo->number = -1;
                        break;
                    }
                    commandInfoSetText(commandInfo, commandParts[1]);
                    break;
                default:
                    commandInfo->commandType = NOTHING;
            }
            break;
        case CREATETAG:
            if(numberOfParts != 2) {
                break;
            }
            commandInfoSetText(commandInfo, commandParts[1]);
            break;
        case ADDTAG:
            if(numberOfParts != 3) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            commandInfoSetText(commandInfo, commandParts[2]);
            break;
        case UNTAG:
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfo->number = atoi(commandParts[1]);
            if(commandInfo->number == 0 && strcmp(commandParts[1], "0") != 0) {
                commandInfo->number = -1;
            }
            break;
        case FILTER:
            if(numberOfParts != 2) {
                commandInfo->commandType = NOTHING;
                break;
            }
            commandInfoSetText(commandInfo, commandParts[1]);
            break;
    }
}

void getCommandFromUser(commandInfo* command) {
    int numberOfParts = 0;

    char* userInputLine = readLine();

    char** splitInput = splitUserInput(userInputLine, &numberOfParts);
    free(userInputLine);

    if(numberOfParts == 0) {
        command->commandType = NOTHING;
        return;
    }

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
    //printf("Welcome to Beets!\n\n");
    //Opening file.

    taskList* taskList = taskListCreate();

    taskTagCollection* tagCollection = tagCollectionCreate(0);
    readFileIntoTagCollection(TAG_LOCATION, tagCollection);

    readFileIntoTaskList(FILE_LOCATION, taskList, tagCollection);

    currentNumberOfPages = (taskList->count/MAX_TASKS_PER_PAGE) + (taskList->count % MAX_TASKS_PER_PAGE == 0 ? 0 : 1);

    int taskNumber;
    printCurrentScreen(taskList, 0);

    //printf(">");
    commandInfo* command = malloc(sizeof(commandInfo));
    taskRecord* task;
    command->commandType = NOTHING;
    while(command->commandType != EXIT_PROGRAM) {
        getCommandFromUser(command);
        switch(command->commandType) {
            case VIEW_TASKS:
                currentPage = command->number;
                if(currentPage >= currentNumberOfPages-1) {
                    currentPage = currentNumberOfPages-1;
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
                task = taskListGetTask(taskList, taskNumber);
                taskSetComplete(task,true);
                saveData(FILE_LOCATION, taskList);
                break;
            case UNCOMPLETE_TASK:
                taskNumber = command->number;
                if(taskNumber < 0 || taskNumber >= taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                task = taskListGetTask(taskList, taskNumber);
                taskSetComplete(task,false);
                saveData(FILE_LOCATION, taskList);
                break;
            case ADD_TASK:
                task = taskCreate(command->text);
                if(command->number > 0) {
                    taskSetDeadline(task, command->number);
                }
                taskListPush(taskList, task);
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
            case EDIT_TASK:
                if(command->number < 0 || command->number > taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                task = taskListGetTask(taskList, command->number);
                taskSetTask(task, command->text);
                saveData(FILE_LOCATION, taskList);
                break;
            case DATE_TASK:
                if(command->number == -1) {
                    printLine("Invalid task number\n");
                    break;
                }
                int taskNumber = atoi(command->text);
                if(taskNumber < 0 || taskNumber > taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                task = taskListGetTask(taskList, taskNumber);

                if(command->number == -2) {
                    task->hasDeadline = false;
                    task->deadline = 0;
                } else {
                    taskSetDeadline(task, command->number);
                }
                saveData(FILE_LOCATION, taskList);
                break;
            case CREATETAG:
                tagCollectionLoadTag(tagCollection, command->text);
                saveTags(TAG_LOCATION, tagCollection);
                break;
            case ADDTAG:
                if(command->number < 0 || command->number > command->number > taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                task = taskListGetTask(taskList, command->number);
                taskTag* tag = tagCollectionGetTagFromName(tagCollection, command->text);
                if(tag == NULL) {
                    printLine("Tag %s does not exist\n", command->text);
                    break;
                }
                taskAddTag(task, tag);
                saveData(FILE_LOCATION, taskList);
                break;
            case UNTAG:
                if(command->number < 0 || command->number > command->number > taskList->count) {
                    printLine("Invalid task number\n");
                    break;
                }
                task = taskListGetTask(taskList, command->number);
                task->tags->currentSize = 0;
                task->tags->numberOfTags = 0;
                saveData(FILE_LOCATION, taskList);
                break;
            case FILTER:
                currentFilter = tagCollectionGetTagFromName(tagCollection, command->text);
                if(currentFilter == NULL) {
                    printLine("Invalid tag: %s\n", command->text);
                }
                currentPage = 0;
                break;
            case EXIT_PROGRAM:
                break;
            default:
                printLine("Unknown command\n");
                break;
        }
        printCurrentScreen(taskList, currentPage);
    }

    free(command);
    taskListClean(taskList);
    taskTagCollectionClean(tagCollection);
}