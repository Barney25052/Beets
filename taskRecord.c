#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "taskRecord.h"

taskRecord* taskCreate(char* text) {
    taskRecord* task = malloc(sizeof(taskRecord));
    taskSetTask(task, text);
    task->isComplete = false;
}

void taskMarkComplete(taskRecord* task) {
    task->isComplete = true;
}

void taskSetTask(taskRecord* task, char* text) {
    int size = strlen(text);
    if(size > 64) {
        size = 64;
    }
    int taskTextSize = text[size-1] == 0 ? size : size + 1;
    int i;
    for(i = 0; i < size; i++) {
        task->text[i] = text[i];
    }
    task->text[i]=0;
}

char* taskPrint(taskRecord* task) {
    char* taskString = malloc((strlen(task->text)+4) * sizeof(char));
    sprintf(taskString, "[%c] %s", task->isComplete ? 'X' : ' ', task->text);
    return taskString;
}