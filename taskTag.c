#include "taskTag.h"
#include <string.h>
#include <stdlib.h>

void taskTagSetName(taskTag* tag, char* name) {
    int length = strlen(name);
    if(length > 16) {
        return;
    }

    int i = 0;
    for(i = 0; i < length; i++) {
        tag->name[i] = name[i];
    }
    tag->name[i] = 0;
}

taskTag* taskTagCreate(int id, char* name) {
    taskTag* taskTag = malloc(sizeof(taskTag));
    taskTagSetName(taskTag, name);
    return taskTag;
}

taskTagCollection* tagCollectionCreate(int size) {
    taskTagCollection* tagCollection = malloc(sizeof(taskTagCollection));
    tagCollection->tags = malloc(size * sizeof(taskTag));
    tagCollection->currentSize = size;
    tagCollection->numberOfTags = 0;
}

void tagCollectionResize(taskTagCollection* collection) {
    if(collection->numberOfTags < collection->currentSize) {
        return;
    }
    collection->currentSize += TAG_COLLECTION_INC;
    collection->tags = realloc(collection->tags, sizeof(taskTag) * collection->currentSize);
}

void tagCollectionAddTag(taskTagCollection* collection, taskTag* tag) {
    tagCollectionResize(collection);
    collection->tags[collection->numberOfTags] = tag;
    collection->numberOfTags += 1;
}

void tagCollectionLoadTag(taskTagCollection* collection, int id, char*name) {
    taskTag* newTaskTag = taskTagCreate(id, name);
    tagCollectionAddTag(collection, newTaskTag);
}

bool tagCollectionContainsTag(taskTagCollection* collection, taskTag* tag) {
    for(int i = 0; i < collection->numberOfTags; i++) {
        if(tag == collection->tags[i]) {
            return true;
        }
    }
    return false;
}