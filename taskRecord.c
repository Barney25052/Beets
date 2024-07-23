#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "taskRecord.h"

taskRecord* taskCreate(char* text) {
    taskRecord* task = malloc(sizeof(taskRecord));
    task->isComplete = false;
    task->text = text;
}

void taskMarkComplete(taskRecord* task) {
    task->isComplete = true;
}

void taskSetTask(taskRecord* task, char* text) {
    int size = strlen(text);
    int taskTextSize = text[size-1] == 0 ? size : size + 1;
    task->text = calloc(sizeof(char), size+1);
    for(int i = 0; i < size; i++) {
        task->text[i] = text[i];
    }
}

char* taskPrint(taskRecord* task) {
    char* taskString = malloc((strlen(task->text)+4) * sizeof(char));
    sprintf(taskString, "[%c] %s", task->isComplete ? 'X' : ' ', task->text);
    return taskString;
}