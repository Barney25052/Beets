#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "taskList.h"
#include "loader.c"
#include "saver.c"

#define VIEW_TASKS 0
#define ADD_TASK 1
#define EXIT_PROGRAM 2

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

int getOption(char* messageText) {
    if(messageText == NULL) {
        messageText = "Please enter an option: ";
    }
    int userInput;
    printf("%s", messageText);
    scanf("%d", &userInput);
    printf("\nUser input: %d\n", userInput);
    return userInput;
}

int mainMenu() {
    char* options[3] = {"View Tasks", "Add Task", "Exit"};
    int optionsLength = 3;

    for(int i = 0; i < optionsLength; i++) {
        printf("%d - %s\n", i , options[i]);
    }

    int userInput = getOption(NULL);
}

int main() {
    printf("Welcome to Beets!\n\nThese are your current tasks.\n------------------\n");

    //Opening file.

    taskList* taskList = taskListCreate();
    readFileIntoTaskList(FILE_LOCATION, taskList);
    int mainMenuOption ;

    while(mainMenuOption != EXIT_PROGRAM) {

        mainMenuOption = mainMenu();
        switch(mainMenuOption) {
            case VIEW_TASKS:
                printAllTasks(taskList);
                taskRecord* task = getTaskFromInput(taskList, false);
                if(task == NULL) {
                    printf("Invalid task selection");
                    return -1;
                }
                taskMarkComplete(task);
                saveData(FILE_LOCATION, taskList);
                printf("\n\n");
                printAllTasks(taskList);
                break;
            case ADD_TASK:
                printf("Please enter the task: ");
                char userInput[64];
                scanf("%c", userInput);
                readLine(userInput, 65);
                taskRecord* newTask = taskCreate(userInput);
                taskListPush(taskList, newTask);
                saveData(FILE_LOCATION, taskList);
                break;
            case EXIT_PROGRAM:
                return 0;
            default:
                printf("Unkown command");
                return -1;
            
        }
    }


}