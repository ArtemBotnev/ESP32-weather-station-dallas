// Weather station
// Copyright Artem Botnev 2023
// MIT License

#ifndef SENSORS_H
#define SENSORS_H

#define ROOM_TEMPER_TITLE   "Room t, C"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

#define DALLAS_SENSORS_COUNT 2

#define ROOM_TEMPER_INDEX 0
#define ROOM_HUM_INDEX 1
#define PRESSURE_INDEX 2

/**
 * Dallas sensor data
 */
struct dallasSensor {
    const char *title;
    uint8_t addr[8];
};

#endif //SENSORS_H
