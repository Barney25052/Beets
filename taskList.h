#include "taskRecord.h"
#ifndef TASKLIST_H
#define TASKLIST_H
struct taskListNode {
    taskRecord* data;
    struct taskListNode* next;
    struct taskListNode* prev;
};
typedef struct taskListNode taskListNode;

struct taskList{
    taskListNode* head;
    taskListNode* tail;
    int count;
};
typedef struct taskList taskList;

taskListNode* taskNodeCreate(taskRecord* data);
taskList* taskListCreate();
void taskListPush(taskList* taskList, taskRecord* data);
void taskListRemoveAtIndex(taskList* taskList, int index);
taskRecord* taskListGetHead(taskList* taskList);
taskRecord* taskListGetTail(taskList* taskList);
taskListNode* taskListGetNode(taskList* taskList, int index);
taskRecord* taskListGetTask(taskList* taskList, int index);
#endif