#include "console.h"
#include <stdlib.h>
#include <stdio.h>

int currentConsoleLines;
consoleLine* currentLine;
consoleLine* startLine;

#define BUFFER_SIZE 256

consoleLine* consoleLineCreate(char* text) {
    consoleLine* line = malloc(sizeof(line));

    line->text = malloc(sizeof(char) * BUFFER_SIZE);
    for(int i = 0; i < BUFFER_SIZE; i++) {
        line->text[i] = text[i];
        if(text[i] == 0) {
            line->text = realloc(line->text, (i+1) * sizeof(char));
            break;
        }
    }
    line->next = NULL;
    line->hasNext = false;
}

void consoleLineSetNext(consoleLine* original, consoleLine* next) {
    original->next = next;
    original->hasNext = true;
}

void addLine(consoleLine* consoleLine) {
    currentConsoleLines ++;
    if(startLine == NULL) {
        startLine = consoleLine;
        currentLine = consoleLine;
    } else {
        consoleLineSetNext(currentLine, consoleLine);
        currentLine = consoleLine;
    }
}

void printError(char* errorPattern, ...) {
    char buffer[BUFFER_SIZE]; 
    va_list args;
    va_start(args, errorPattern);
    vsprintf(buffer, errorPattern, args);
    va_end(args);
    consoleLine* consoleLine = consoleLineCreate(buffer);
    addLine(consoleLine);
}

void printLine(char* linePattern, ...) {
    char buffer[BUFFER_SIZE]; 
    va_list args;
    va_start(args, linePattern);
    vsprintf(buffer, linePattern, args);
    va_end(args);
    consoleLine* consoleLine = consoleLineCreate(buffer);
    addLine(consoleLine);
}
void printAll() {
    currentLine = startLine;
    for(int i = 0; i < currentConsoleLines; i++) {
        consoleLine* line = currentLine;
        printf("%s\n", line->text);
        if(line->hasNext) {
            currentLine = line->next;
            free(line);
        } else {
            free(line);
            break;
        }
    }
    startLine = NULL;
    currentConsoleLines = 0;
}