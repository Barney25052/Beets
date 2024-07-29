#include <stdbool.h>
#define TAG_COLLECTION_INC 5

#ifndef TASKTAG_H
#define TASKTAG_H

struct taskTag {
    char name[17];  //16 + 1 for '/0'
};

typedef struct taskTag taskTag;

taskTag* taskTagCreate(int id, char* name);

struct taskTagCollection {
    taskTag** tags;
    int currentSize;
    int numberOfTags;
};

typedef struct taskTagCollection taskTagCollection;

taskTagCollection* tagCollectionCreate(int size);

void tagCollectionAddTag(taskTagCollection* collection, taskTag* tag);

void tagCollectionLoadTag(taskTagCollection* collection, int id, char*name);

bool tagCollectionContainsTag(taskTagCollection* collection, taskTag* tag);

#endif