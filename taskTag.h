#include <stdbool.h>
#define TAG_COLLECTION_INC 3

#ifndef TASKTAG_H
#define TASKTAG_H

struct taskTag {
    char name[17];  //16 + 1 for '/0'
};

typedef struct taskTag taskTag;

taskTag* taskTagCreate(char* name);

struct taskTagCollection {
    taskTag** tags;
    int currentSize;
    int numberOfTags;
};

typedef struct taskTagCollection taskTagCollection;

taskTagCollection* tagCollectionCreate(int size);

void tagCollectionAddTag(taskTagCollection* collection, taskTag* tag);

void tagCollectionLoadTag(taskTagCollection* collection, char*name);

bool tagCollectionContainsTag(taskTagCollection* collection, taskTag* tag);

void taskTagCollectionClean(taskTagCollection* collection);

#endif