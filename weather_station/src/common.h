// Weather station
// Copyright Artem Botnev 2023
// MIT License

#ifndef COMMON_H
#define COMMON_H

#define DEFAULT_VALUE 1000

#include "inttypes.h"
// count of measure types exclude dallas sensors
#define MEASURE_TYPES_COUNT 3

/**
 * Container for measured parameters
 */
template <typename T> struct measureSet {
    char *title;
    T curValue;
    T min;
    float average;
    T max;
    measureSet()
            : title { "" }, curValue { -DEFAULT_VALUE }, min { DEFAULT_VALUE }, average { -DEFAULT_VALUE }, max { -DEFAULT_VALUE } {  }

    measureSet(char* title, T curValue, T min, float average, T max)
            : title { title }, curValue { curValue }, min { min }, average { average }, max { max } {  }
};

/**
 * Pack for time data
 */
struct timePack {
    uint32_t epochSeconds;
    uint8_t day, minute;
    const char *timeDate, *time, *date;
};

#endif //COMMON_H
