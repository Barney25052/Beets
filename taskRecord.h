#include <stdbool.h>
#include "taskTag.h"
#ifndef TASKRECORD_H
#define TASKRECORD_H
    struct taskRecord {
        char text[65];
        bool isComplete;
        bool hasDeadline;
        unsigned int deadline;
        taskTagCollection* tags;
    };
    typedef struct taskRecord taskRecord;
    taskRecord* taskCreate(char* text);
    void taskSetComplete(taskRecord* task, bool complete);
    void taskSetTask(taskRecord* task, char* text);
    void taskSetDeadline(taskRecord* task, int deadline);
    void taskSetDeadlineFromTime(taskRecord* task, int year, int month, int day, int hour, int minute);
    void taskAddTag(taskRecord* task, taskTag* tag);
    char* taskPrint(taskRecord* task);
#endif