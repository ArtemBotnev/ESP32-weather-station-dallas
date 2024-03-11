// Weather station
// Copyright Artem Botnev 2024
// MIT License

#ifndef STORAGE_H
#define STORAGE_H

#define DEFAULT_VALUE 1000

#include "../common.h"
#include "../sensors.h"

class DataManager {
public:
    DataManager();

    measureSet<int16_t> getMeasureSet(const char *title, int8_t measureIndex, int16_t currentVal);

    void clearCache();

    /**
     * @return array of measureSet where index corresponds to index of measure from sensors.h
     */
    measureSet<int16_t> *getMeasuresArray();

private:
    struct item {
        int8_t index;
        const char *title;
        int16_t current;
        int16_t min;
        int16_t max;
        float average;
        uint16_t factor;
    };

    struct Cache {
        item items[DALLAS_SENSORS_COUNT + MEASURE_TYPES_COUNT];
    };

    Cache *_cache;

    item *getCacheItemByIndex(int8_t index);

    measureSet<int16_t> itemToMeasureSet(item *item);
};

#endif //STORAGE_H
