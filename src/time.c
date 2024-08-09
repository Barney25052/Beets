#include "time.h"
#include <stdio.h>
#include <stdlib.h>

/*
TIME DATE STRUCTURE

00000000 00000000 00000000 00000000 
|        day         || hr  || mn |

first 6 bits = minute 0-59
next 5 bits = hour    0-23
last 21 bits = day    

day will be counted from 0 AD.
max can store is 31st December 5740
*/

#define YEAR 365
#define LEAP_YEAR 366

int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//change to month and day
unsigned int timeCreate(int year, int month, int day, int hour, int minute) {
    unsigned int bitPointer = 1;
    int finalTime = 0;

    //minutes
    for(int i = 0; i < 6; i++) {
        finalTime += (bitPointer & minute);
        bitPointer <<= 1;
    }
    
    //hour
    bitPointer = 1;
    for(int i = 6; i < 11; i++) {
        finalTime += (bitPointer & hour) * 64;
        bitPointer <<= 1;
    }
    
    //year
    bitPointer = 1;

    int isLeapYear = year % 4 == 0 ? ((year % 100 == 0 && year % 400 != 0) ? 0 : 1) : 0;
    int numberOfLeapYears = year/4 - year/100 + year/400 + (1-isLeapYear);
    int numberOfNormalYears = year-numberOfLeapYears;

    unsigned int totalDays = numberOfLeapYears * LEAP_YEAR + numberOfNormalYears * YEAR + day-1;

    for(int i = 0; i < month-1; i ++) {
        int currentMonthDays = monthDays[i];
        if(i == 1 && isLeapYear) {
            currentMonthDays += 1;
        }
        totalDays += currentMonthDays;
    }


    for(int i = 11; i < 32; i++) {
        finalTime += (bitPointer & totalDays) * 2048;
        bitPointer <<= 1;
    }
    return finalTime;
}

char* timeAsString(int time) {
    int minute = 0;
    int hour = 0;
    unsigned int day = 0;
    int year = 0;

    int bitPointer = 1;

    for(int i = 0; i < 6; i++) {
        minute += bitPointer & time;
        bitPointer <<= 1;
    }

    for(int i = 6; i < 11; i++) {
        hour += bitPointer & time;
        bitPointer <<= 1;
    }
    hour >>= 6;

    unsigned int totalOfDays = 0;
    for(int i = 11; i < 32; i++) {
        totalOfDays += bitPointer & time;
        bitPointer <<= 1;
    }
    totalOfDays >>= 11;

    unsigned int currentDays = totalOfDays;
    int numberOfLeapYears = 0;
    while(currentDays > YEAR) {
        //leap year stuff every 100 is not a leap year unless it can be divided by 400
        if(year % 4 == 0) {
            currentDays -= 1;
            numberOfLeapYears ++;
        }
        if(year % 100 == 0 && year % 400 != 0) {
            currentDays += 1;
            numberOfLeapYears -= 1;
        }
        currentDays -= YEAR;
        year ++;
    }

    int isLeapYear = year % 4 == 0 ? ((year % 100 == 0 && year % 400 != 0) ? 0 : 1) : 0;
    int month;
    int i;
    for(i = 0; i < 12; i ++) {
        int currentMonthDays = monthDays[i];
        if(i == 1 && isLeapYear) {
            currentMonthDays += 1;
        }
        if(currentMonthDays > currentDays) {
            break;
        }
        currentDays -= currentMonthDays;
    }

    month = i + 1;
    day = currentDays+1;

    /*printf("Minute: %d\n", minute);
    printf("Hour: %d\n", hour);
    printf("Total Days: %d\n", totalOfDays);
    printf("Year: %d\n", year);
    printf("Month: %d\n", month);
    printf("Day: %d\n", day);
    printf("%d/%d/%d %d:%d\n", day, month, year, hour, minute);*/

    char* dateString = malloc(sizeof(char) * 52);
    char* dp = day <= 9     ? "0" : "";
    char* Mp = month <= 9   ? "0" : "";
    char* hp = hour <= 9    ? "0" : "";
    char* mp = minute <= 9  ? "0" : "";
    sprintf(dateString, "%s%d/%s%d/%d %s%d:%s%d", dp, day, Mp, month, year, hp, hour, mp, minute);
    
    return dateString;
}