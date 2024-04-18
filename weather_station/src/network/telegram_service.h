// Weather station
// Copyright Artem Botnev 2024
// MIT License

#ifndef TELEGRAM_SERVICE_H
#define TELEGRAM_SERVICE_H

#define MEASURE_STRING_SIZE 15

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "../common.h"
#include "../sensors.h"

class TelegramService {

public:
    /**
     * telegram service initialization
     * @param token - bot token
     * @param client - wifi network client
     */
    TelegramService(const char *token, WiFiClientSecure *client);

    /**
     * starts dot work
     * @param time - timePack representing measures time
     * @param measureArrayGetter - function for receiving measures data
     */
    void botInteraction(timePack time, measureSet<int16_t> *(*measureArrayGetter)());

private:
    UniversalTelegramBot _bot;

    const char *_str_current = "current";
    const char *_str_min = "min";
    const char *_str_max = "max";
    const char *_str_av = "average";

    void replay(uint8_t messageNum, timePack time, measureSet<int16_t> *measures);

    char *buildMeasuresShortBlock(measureSet<int16_t> *measures);

    char *buildMeasuresFullBlock(measureSet<int16_t> *measures);
};

#endif //TELEGRAM_SERVICE_H
