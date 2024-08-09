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
    task->deadline = 0;
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

char* taskGetDeadlineString(taskRecord* task) {
    if(!task) {
        return "";
    }
    if(!task->hasDeadline) {
        return "";
    }
    char* timeString = timeAsString(task->deadline);
    char* deadlineString = malloc(24 * sizeof(char));
    sprintf(deadlineString, "\n\t %s", timeString);
    free(timeString);
    return deadlineString;
}

char* taskPrint(taskRecord* task) {
    if(!task) {
        return "Error Loading Task\n";
    }
    char* taskString = malloc(128 * sizeof(char));
    char* deadlineString = taskGetDeadlineString(task);
    sprintf(taskString, "[\e[1;33m%c\e[0;37m] %s%s\n", task->isComplete ? 'X' : ' ', task->text, deadlineString);
    if(deadlineString != "") {
        free(deadlineString);
    }
    return taskString;
}

void taskAddTag(taskRecord* task, taskTag* tag) {
    if(tagCollectionContainsTag(task->tags, tag)) {
        return;
    }
    tagCollectionAddTag(task->tags, tag);   
}