#include <stdbool.h>
#ifndef TASKRECORD_H
#define TASKRECORD_H
    struct taskRecord {
        char* text;
        bool isComplete;
    };
    typedef struct taskRecord taskRecord;
    taskRecord* taskCreate(char* text);
    void taskMarkComplete(taskRecord* task);
    void taskSetTask(taskRecord* task, char* text);
    char* taskPrint(taskRecord* task);
#endif