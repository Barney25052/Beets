#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "taskList.h"
#include "loader.c"
#include "saver.c"

void printAllTasks(taskList* taskList) {
    for(int i = 0; i < taskList->count; i++) {
        printf("%d - %s\n", i, taskPrint(taskListGetTask(taskList, i)));
    }
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
//Bash commands to add tasks
//Filtered searches
//Priority
//Optional metadata and links on tasks e.g homework file and description

const char* FILE_LOCATION = "/home/ryan-bell/Documents/Beets/notes.txt";
int main() {
    printf("Welcome to Beets!\n\nThese are your current tasks.\n------------------\n");

    //Opening file.

    taskList* taskList = taskListCreate();
    readFileIntoTaskList(FILE_LOCATION, taskList);
    printAllTasks(taskList);

    while(true) {
        taskRecord* task = getTaskFromInput(taskList, false);
        if(task == NULL) {
            return -1;
        }

        taskMarkComplete(task);
        saveData(FILE_LOCATION, taskList);
        printAllTasks(taskList);
    }


}