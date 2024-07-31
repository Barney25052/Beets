#include <stdarg.h>
#include <stdbool.h>
#ifndef CONSOLE_H
#define CONSOLE_H
    struct consoleLine {
        char* text;
        bool hasNext;
        struct consoleLine* next;
    };
    typedef struct consoleLine consoleLine;
    consoleLine* consoleLineCreate(char* text);
    void consoleLineSetNext(consoleLine* original, consoleLine* next);
    void printError(char* errorPattern, ...);
    void printLine(char* linePattern, ...);
    void printAll();
#endif