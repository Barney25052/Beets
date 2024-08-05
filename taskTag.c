#include "taskTag.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void taskTagSetName(taskTag* tag, char* name) {
    int length = strlen(name);
    if(length > 16) {
        length = 16;
    }

    int i = 0;
    for(i = 0; i < length; i++) {
        tag->name[i] = name[i];
    }
    tag->name[i] = 0;
}

taskTag* taskTagCreate(char* name) {
    taskTag* tag = malloc(sizeof(taskTag));
    taskTagSetName(tag, name);
    return tag;
}

taskTagCollection* tagCollectionCreate(int size) {
    taskTagCollection* tagCollection = malloc(sizeof(taskTagCollection));
    tagCollection->tags = malloc(size * sizeof(taskTag));
    tagCollection->currentSize = size;
    tagCollection->numberOfTags = 0;
}

taskTag* tagCollectionGetTagFromName(taskTagCollection* collection, char* name) {
    for(int i = 0; i < collection->numberOfTags; i++) {
        if(strcmp(collection->tags[i]->name, name) == 0) {
            return collection->tags[i];
        }
    } 
    return NULL;
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

void tagCollectionLoadTag(taskTagCollection* collection, char* name) {
    taskTag* newTaskTag = taskTagCreate(name);
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

void taskTagCollectionClean(taskTagCollection* collection) {
    for(int i = 0; i < collection->numberOfTags; i++) {
        free(collection->tags[i]);
    }
    free(collection->tags);
    free(collection);
}
