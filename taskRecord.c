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

void taskSetComplete(taskRecord* task, bool complete) {
    task->isComplete = complete;
}

void taskSetTask(taskRecord* task, char* text) {
    int size = strlen(text);    
    int i;
    if(size > 64) {
        size = 64;
    }
    for(i = 0; i < size; i++) {
        task->text[i] = text[i];
    }
    task->text[i]=0;
}

void taskSetDeadline(taskRecord* task, int deadline) {
    task->deadline = deadline;
    task->hasDeadline = true;
}

void taskSetDeadlineFromTime(taskRecord* task, int year, int month, int day, int hour, int minute) {
    task->deadline = timeCreate(year, month, day, hour, minute);
    task->hasDeadline = true;
}

char* taskPrint(taskRecord* task) {
    char* taskString = malloc((strlen(task->text)+4+(task->hasDeadline?24:0)) * sizeof(char));
    sprintf(taskString, "[%c] %s", task->isComplete ? 'X' : ' ', task->text);
    if(task->hasDeadline) {
        char* dateString = timeAsString(task->deadline);
        sprintf(taskString, "%s\n\t%s", taskString, dateString);
        free(dateString);
    }
    sprintf(taskString, "%s\n", taskString);
    return taskString;
}

void taskAddTag(taskRecord* task, taskTag* tag) {
    if(tagCollectionContainsTag(task->tags, tag)) {
        return;
    }
    tagCollectionAddTag(task->tags, tag);   
}