#include <stdarg.h>
#include <stdbool.h>
#ifndef CONSOLE_H
#define CONSOLE_H
    void printError(char* errorPattern, ...);
    void printLine(char* linePattern, ...);
    void clearScreen();
#endif