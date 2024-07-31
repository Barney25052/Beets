#include "console.h"
#include <stdlib.h>
#include <stdio.h>

int currentConsoleLines = 0;


void printError(char* errorPattern, ...) {
    //TODO: Implement
}

void printLine(char* linePattern, ...) {
    if(currentConsoleLines == 0) {
        currentConsoleLines ++;
    }
    char buffer[256];
    va_list args;
    va_start(args, linePattern);
    vsprintf(buffer, linePattern, args);
    va_end(args);
    int i = 0;
    while(buffer[i] != 0) {
        if(buffer[i] == '\n') {
            currentConsoleLines ++;
        }
        i++;
    }
    printf("%s", buffer);
}

void clearScreen() {
    for(int i = 0; i < currentConsoleLines; i++) {
        printf("\r\33[2K\r\33[1A\r");
    }
    currentConsoleLines = 0;
}