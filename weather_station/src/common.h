// Weather station
// Copyright Artem Botnev 2023
// MIT License

#ifndef COMMON_H
#define COMMON_H

#include "inttypes.h"
// count of enum items in MeasureType
//#define MEASURE_TYPES_COUNT 5
//
//enum MeasureType { ROOM_TEMPER, DALLAS_TEMPER, ROOM_HUM, PRESSURE };

/**
 * Container for measured parameters
 */
template <typename T> struct measureSet {
    const char *title;
    const T curValue;
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
