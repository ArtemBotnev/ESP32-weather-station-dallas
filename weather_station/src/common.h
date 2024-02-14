// Weather station
// Copyright Artem Botnev 2023
// MIT License

#ifndef COMMON_H
#define COMMON_H

#include "inttypes.h"
// count of measure types exclude dallas sensors
#define MEASURE_TYPES_COUNT 3

/**
 * Container for measured parameters
 */
template <typename T> struct measureSet {
    const char *title;
    const T curValue;
    const T min;
    const float average;
    const T max;
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
