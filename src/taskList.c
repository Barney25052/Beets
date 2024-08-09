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
    taskList* list = malloc(sizeof(taskList));
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
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
    
    free(taskToRemove->data->tags->tags);
    free(taskToRemove->data->tags);
    free(taskToRemove->data);
    free(taskToRemove);
}

taskRecord* taskListGetHead(taskList* taskList) {
    if(taskList->head == NULL) {
        return NULL;
    }
    return taskList->head->data;
}

taskRecord* taskListGetTail(taskList* taskList) {
    if(taskList->tail == NULL) {
        return NULL;
    }
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

void taskListClean(taskList* taskList) {
    taskListNode* currentNode = taskListGetNode(taskList, 0);

    if(currentNode == NULL) {
        return;
    }

    for(int i = 0; i < taskList->count; i++) {
        free(currentNode->data->tags->tags);
        free(currentNode->data->tags);
        free(currentNode->data);
        taskListNode* nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    free(taskList);
}