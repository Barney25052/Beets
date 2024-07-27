#include <stdbool.h>
#include <time.h>
#ifndef TASKRECORD_H
#define TASKRECORD_H
    struct taskRecord {
        char text[65];
        bool isComplete;
        bool hasDeadline;
        struct tm* deadline;
    };
    typedef struct taskRecord taskRecord;
    taskRecord* taskCreate(char* text);
    void taskMarkComplete(taskRecord* task);
    void taskSetTask(taskRecord* task, char* text);
    void taskSetDeadline(taskRecord* task, int year, int month, int day, int hour);
    char* taskPrint(taskRecord* task);
#endif