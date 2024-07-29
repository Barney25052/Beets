#include "taskRecord.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct tm* createTime(int year, int month, int day, int hour) {
    hour --;
    if(hour == -1) {
        hour = 23;
    }
    struct tm* time = malloc(sizeof(struct tm));
    time->tm_year = year - 1900;
    time->tm_mon = month - 1;
    time->tm_mday = day;
    time->tm_hour = hour;
    time->tm_min = 30;
    time->tm_sec = 30;
    mktime(time);
    return time;
}

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

void taskSetDeadline(taskRecord* task, int year, int month, int day, int hour) {
    task->deadline = createTime(year, month, day, hour);
    task->hasDeadline = true;
}

char* taskPrint(taskRecord* task) {
    char* dateString = calloc(sizeof(char), 24);
    if(task->hasDeadline) {
        strftime(dateString, 24, " - %d/%m/%y %H:00", task->deadline);
    }
    char* taskString = malloc((strlen(task->text)+4+(task->hasDeadline?24:0)) * sizeof(char));
    sprintf(taskString, "[%c] %s%s", task->isComplete ? 'X' : ' ', task->text, dateString);
    return taskString;
}