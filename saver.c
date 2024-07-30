#include <stdio.h>
#include <string.h>
#include "taskList.h"
#include "taskRecord.h"
#include "serialization.h"
#include "time.h"

void saveData(const char* fileLocation, taskList* taskList) {
    FILE* fileptr = fopen(fileLocation, "w");
    int count = taskList->count;
    for(int i = 0; i < count; i++) {
        taskRecord* currentTask = taskListGetTask(taskList, i);

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
        }
    }
    fclose(fileptr);
}