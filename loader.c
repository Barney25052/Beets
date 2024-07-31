#include <stdio.h>
#include <stdlib.h>
#include "taskList.h"
#include "taskRecord.h"
#include "serialization.h"
#include "time.h"

char* parseTaskSegmentInformation(char* segmentData, int size) {
    char* parsedData = malloc((size-1) * sizeof(char));
    for(int i = 1; i < size; i++) {
        parsedData[i-1] = segmentData[i];
    }
    parsedData[size-1] = 0;
    return parsedData;
}

void parseTask(char* data, int size, taskList* taskList) {

    if(size == 0) {
        return;
    }

    char dataType = data[0];
    taskRecord* mostRecentTask;
    switch(dataType) {
        case TASK_INFO:
            char* taskInfo = parseTaskSegmentInformation(data, size);
            taskRecord* task = taskCreate(taskInfo);
            taskListPush(taskList,task);
            free(taskInfo);
            break;
        case TASK_COMPLETED:
            mostRecentTask = taskListGetTail(taskList);
            if(mostRecentTask == NULL) {break;}
            taskSetComplete(mostRecentTask, task);
            break;
        case TASK_DEADLINE:
            mostRecentTask = taskListGetTail(taskList);
            if(mostRecentTask == NULL) {break;}
            char* dateText = parseTaskSegmentInformation(data, size);
            int deadline = atoi(dateText); 
            mostRecentTask->deadline = deadline;
            mostRecentTask->hasDeadline = true;
            free(dateText);
            break;
        default:
            break;

    }

}

char* readPart(FILE* filePtr, taskList* taskList) {

    int bufferSize = 256;
    char* buffer = calloc(bufferSize, sizeof(char));
    char currentChar;

    int i;
    for(i = 0; i < bufferSize; i++) {
        currentChar = fgetc(filePtr);
        if(currentChar == '\n' || currentChar == EOF) {
            buffer[i] = 0;
            break;
        }
        buffer[i] = currentChar;
    }

    buffer = realloc(buffer, i+1);

    return buffer;
}

void readFileIntoTaskList(const char* fileLocation, taskList* taskList) {
    FILE* filePtr;
    filePtr = fopen(fileLocation, "r");

    if(filePtr == NULL) {
        printf("File at location %s can not be found.", fileLocation);
        return;
    } 

    bool isEndOfFile = false;
    while(!isEndOfFile) {
        char* buffer = readPart(filePtr, taskList);
        if(buffer[0] == 0) {
            isEndOfFile = true;
        } else {
            parseTask(buffer, strlen(buffer), taskList);
        }
    }

    fclose(filePtr);
}