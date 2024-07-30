#include "taskRecord.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "time.h"

taskRecord* taskCreate(char* text) {
    taskRecord* task = malloc(sizeof(taskRecord));
    taskSetTask(task, text);
    task->isComplete = false;
    task->hasDeadline = false;
    task->tags = tagCollectionCreate(0);
    return task;
}

void taskMarkComplete(taskRecord* task) {
    task->isComplete = true;
}

void taskMarkUncomplete(taskRecord* task) {
    task->isComplete = false;
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

void taskSetDeadline(taskRecord* task, int year, int month, int day, int hour, int minute) {
    task->deadline = timeCreate(year, month, day, hour, minute);
    task->hasDeadline = true;
}

char* taskPrint(taskRecord* task) {
    char* dateString = calloc(sizeof(char), 24);
    if(task->hasDeadline) {
        sprintf(dateString, "\n\t%s", timeAsString(task->deadline));
    }
    char* taskString = malloc((strlen(task->text)+4+(task->hasDeadline?24:0)) * sizeof(char));
    sprintf(taskString, "[%c] %s%s\n", task->isComplete ? 'X' : ' ', task->text, dateString);
    free(dateString);
    return taskString;
}

void taskAddTag(taskRecord* task, taskTag* tag) {
    if(tagCollectionContainsTag(task->tags, tag)) {
        return;
    }
    tagCollectionAddTag(task->tags, tag);   
}