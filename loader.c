#include <stdio.h>
#include <stdlib.h>
#include "taskList.h"
#include "taskRecord.h"
#include "serialization.h"

char* parseTaskSegmentInformation(char* segmentData, int size) {
    char* parsedData = malloc((size-1) * sizeof(char));
    for(int i = 1; i < size; i++) {
        parsedData[i-1] = segmentData[i];
    }
    parsedData[size-1] =0;
    return parsedData;
}

void parseTask(char* data, int size, taskList* taskList) {
    if(size < 1) {
        //printf("Invalid data!\n");
        return;
    }

    char dataType = data[0];
    taskRecord* mostRecentTask;
    switch(dataType) {
        case TASK_INFO:
            char* taskInfo = parseTaskSegmentInformation(data, size);
            taskRecord* task = taskCreate(taskInfo);
            taskListPush(taskList,task);
            break;
        case TASK_COMPLETED:
            mostRecentTask = taskListGetTail(taskList);
            if(mostRecentTask != NULL) {
                taskMarkComplete(mostRecentTask);
            }
            break;
        case TASK_DEADLINE:
            char* dateText = parseTaskSegmentInformation(data, size);
            int length = strlen(dateText);
            int currentNum = 0, j = 0;
            int dateInfo[4] = {0};
            char currentChar;
            for(int i = 0; i < length; i++) {
                currentChar = dateText[i];
                if(currentChar < '0' || currentChar > '9') {
                    dateInfo[j] = currentNum;
                    j++;
                    currentNum = 0;
                    continue;
                }
                currentNum*=10;
                currentNum += (currentChar - '0');
            }
            if(j < 4) {
                dateInfo[j] = currentNum;
            }
            mostRecentTask = taskListGetTail(taskList);
            if(mostRecentTask != NULL) {
                taskSetDeadline(mostRecentTask, dateInfo[0], dateInfo[1], dateInfo[2], dateInfo[3]);
            }
            break;
        default:
            //printf("Invalid data type '%c'", dataType);
            break;

    }

}

bool readPart(FILE* filePtr, taskList* taskList) {
    char buffer[256];
    char currentChar;

    int i;
    for(i = 0; i < 256; i++) {
        currentChar = fgetc(filePtr);
        if(currentChar == '\n' || currentChar == EOF) {
            buffer[i] = 0;
            break;
        }
        buffer[i] = currentChar;
    }
    parseTask(buffer, i, taskList);
    return currentChar == EOF;
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
        isEndOfFile = readPart(filePtr, taskList);
    }

    fclose(filePtr);
}