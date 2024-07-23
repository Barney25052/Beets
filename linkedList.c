#include <stdlib.h>
#include "taskRecord.c"

struct taskListNode {

    taskRecord* data;
    struct taskListNode* next;

};

typedef struct taskListNode taskListNode;

typedef struct {

    taskListNode* head;
    int count;

} taskList;

taskListNode* taskNodeCreate(taskRecord* data) {
    taskListNode* node = malloc(sizeof(taskListNode));
    node->data = data;
    node->next = NULL;
    return node;
}

taskList* taskListCreate() {
    taskList* taskList = malloc(sizeof(taskList));
    taskList->count = 0;
    taskList->head = NULL;
    return taskList;
}

void taskListPush(taskList* taskList, taskRecord* data) {
    taskListNode* nodeToAdd = taskNodeCreate(data);
    nodeToAdd->next = taskList->head;
    taskList->head = nodeToAdd;
    taskList->count ++;
}

taskRecord* taskListGetTask(taskList* taskList, int index) {
    if(index >= taskList->count) {
        return NULL;
    }
    taskListNode* currentNode = taskList->head;

    for(int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }

    return currentNode->data;
}