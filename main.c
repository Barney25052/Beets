#include <stdio.h>
#include <string.h>
#include "taskRecord.h"
#include "linkedList.c"

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

int main() {
    printf("Welcome to Beets!\n\nThese are your current tasks.\n------------------\n");

    taskList* taskList = taskListCreate();
    taskListPush(taskList, taskCreate("Serialization and saving."));
    taskListPush(taskList, taskCreate("Set up naming convention : 'objectFunction' e.g taskCreate."));
    taskListPush(taskList, taskCreate("Input to mark task complete."));
    taskListPush(taskList, taskCreate("Set up linked taskList."));
    taskListPush(taskList, taskCreate("Create a git repo."));

    taskMarkComplete(taskListGetTask(taskList, 1));

    printAllTasks(taskList);

    taskRecord* task = getTaskFromInput(taskList, false);
    if(task == NULL) {
        return -1;
    }
    taskMarkComplete(task);

    printAllTasks(taskList);

}