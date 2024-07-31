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


void taskListRemoveAtIndex(taskList* taskList, int index) {
    if(index < 0 || index >= taskList->count) {
        return;
    }

    taskListNode* taskToRemove = taskListGetNode(taskList, index);
    if(taskToRemove->prev != NULL) {
        taskToRemove->prev->next = taskToRemove->next;
    } else {
        taskList->head = taskToRemove->next;
    }
    if(taskToRemove->next != NULL) {
        taskToRemove->next->prev = taskToRemove->prev;
    } else {
        taskList->tail = taskToRemove->prev;
    }
    taskList->count --;
    free(taskToRemove);
}

taskRecord* taskListGetHead(taskList* taskList) {
    return taskList->head->data;
}

taskRecord* taskListGetTail(taskList* taskList) {
    return taskList->tail->data;
}

taskListNode* taskListGetNode(taskList* taskList, int index) {
    if(index >= taskList->count || index < 0) {return NULL;}

    taskListNode* currentNode = taskList->head;
    for(int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }
    return currentNode;
}

taskRecord* taskListGetTask(taskList* taskList, int index) {
    return taskListGetNode(taskList, index)->data;
}