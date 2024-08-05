#include <stdio.h>
#include <string.h>
#include "taskTag.h"
#include "taskList.h"
#include "taskRecord.h"
#include "serialization.h"
#include "time.h"

void saveData(const char* fileLocation, taskList* taskList) {

    FILE* fileptr = fopen(fileLocation, "w");
    int count = taskList->count;
    taskListNode* currentNode = taskList->head;

    for(int i = 0; i < count; i++) {

        taskRecord* currentTask = currentNode->data;

        //Add text
        char* text = currentTask->text;
        int textSize = strlen(text);
        fputc(TASK_INFO,fileptr);

        for(int i = 0; i < textSize; i++) {
            fputc(text[i], fileptr);
        }
        fputc('\n', fileptr);

        //Add Completed
        if(currentTask->isComplete) {
            fputc(TASK_COMPLETED, fileptr);
            fputc('\n', fileptr);
        }

        //Add deadline
        if(currentTask->hasDeadline) {
            fputc(TASK_DEADLINE, fileptr);
            char* deadline = malloc(24 * sizeof(char));
            sprintf(deadline, "%d", currentTask->deadline);
            fputs(deadline, fileptr);
            fputc('\n', fileptr);
            free(deadline);
        }

        //Add Tags
        int numberOfTags = currentTask->tags->numberOfTags;
        for(int i = 0; i < numberOfTags; i++) {
            char* tagText = currentTask->tags->tags[i]->name;
            int textSize = strlen(tagText);
            fputc(TASK_TAG, fileptr);
            for(int j = 0; j < textSize; j++) {
                fputc(tagText[j], fileptr);
            }
            fputc('\n', fileptr);
        }

        currentNode = currentNode->next;
    }

    fclose(fileptr);
}

void saveTags(const char* fileLocation, taskTagCollection* tagCollection) {
    FILE* fileptr = fopen(fileLocation, "w");
    int numberOfTags = tagCollection->numberOfTags;

    for(int i = 0; i < numberOfTags; i++) {
        char* tagText = tagCollection->tags[i]->name;
        int textSize = strlen(tagText);
        for(int j = 0; j < textSize; j++) {
            fputc(tagText[j], fileptr);
        }
        fputc('\n', fileptr);
    }

    fclose(fileptr);
}