#include "taskRecord.h"
#ifndef TASKLIST_H
#define TASKLIST_H
struct taskListNode {
    taskRecord* data;
    struct taskListNode* next;
};
typedef struct taskListNode taskListNode;

struct taskList{
    taskListNode* head;
    int count;
};
typedef struct taskList taskList;

taskListNode* taskNodeCreate(taskRecord* data);
taskList* taskListCreate();
void taskListPush(taskList* taskList, taskRecord* data);
taskRecord* taskListGetHead(taskList* taskList);
taskRecord* taskListGetTask(taskList* taskList, int index);
#endif