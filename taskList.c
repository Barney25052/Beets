#include "taskList.h"
#include <stdlib.h>
#include "taskRecord.h"

taskListNode* taskNodeCreate(taskRecord* data) {
    taskListNode* node = malloc(sizeof(taskListNode));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

taskList* taskListCreate() {
    taskList* taskList = malloc(sizeof(taskList));
    taskList->count = 0;
    taskList->head = NULL;
    taskList->tail = NULL;
    return taskList;
}

void taskListPush(taskList* taskList, taskRecord* data) {
    taskListNode* nodeToAdd = taskNodeCreate(data);
    nodeToAdd->prev = taskList->tail;
    if(taskList->tail != NULL) {
        taskList->tail->next = nodeToAdd;
    }
    if(taskList->count == 0) {
        taskList->head = nodeToAdd;
    }
    taskList->tail = nodeToAdd;
    taskList->count ++;
}

taskRecord* taskListGetHead(taskList* taskList) {
    return taskList->head->data;
}

taskRecord* taskListGetTail(taskList* taskList) {
    return taskList->tail->data;
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