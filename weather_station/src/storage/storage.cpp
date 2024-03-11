// Weather station
// Copyright Artem Botnev 2024
// MIT License

#include "storage.h"

DataManager::DataManager() {
    _cache = new Cache();

    for (uint8_t i = 0; i < MEASURE_TYPES_COUNT + DALLAS_SENSORS_COUNT; i++) {
        _cache->items[i] = {
                static_cast<int8_t>(i), "", DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, -100.0f, 1
        };
    }
}

measureSet<int16_t> DataManager::getMeasureSet(const char *title, int8_t measureIndex, int16_t currentVal) {
    item *item = getCacheItemByIndex(measureIndex);
    if (!item) {
        // error
        return measureSet<int16_t> { "", -DEFAULT_VALUE, DEFAULT_VALUE, -DEFAULT_VALUE, -DEFAULT_VALUE };
    }

    item->title = title;
    item->current = currentVal;

    if (currentVal < item->min || item->min == DEFAULT_VALUE) {
        item->min = currentVal;
    }

    if (currentVal > item->max || item->max == DEFAULT_VALUE) {
        item->max = currentVal;
    }

    if (item->factor > 1) {
        item->average = (item->average * item->factor + (float) currentVal) / ++item->factor;
    } else {
        item->average = currentVal;
        item->factor++;
    }

    return measureSet<int16_t> { title, currentVal, item->min, item->average , item->max };
}

void DataManager::clearCache() {
    delete _cache;
    _cache = new Cache();
}

DataManager::item *DataManager::getCacheItemByIndex(int8_t index) {
    return &_cache->items[index];
}

measureSet<int16_t> *DataManager::getMeasuresArray() {
    uint8_t measuresCount = MEASURE_TYPES_COUNT + DALLAS_SENSORS_COUNT;
    static measureSet<int16_t> result[measuresCount];

    for (uint8_t i = 0; i < measuresCount; i++) {
        item *item = getCacheItemByIndex(i);
        result[i] = itemToMeasureSet(item);
    }

    return result;
}

measureSet<int16_t> DataManager::itemToMeasureSet(item *item,) {
    return measureSet<int16_t> { item->title, item->current, item->min, item->average, item->max };
}