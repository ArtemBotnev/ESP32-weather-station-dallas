// Weather station
// Copyright Artem Botnev 2024
// MIT License

#include "telegram_service.h"

TelegramService::TelegramService(const char *token, WiFiClientSecure *client):
        _bot { UniversalTelegramBot(token, *client) } {}

void TelegramService::botInteraction(timePack time, measureSet<int16_t> *(*measureArrayGetter)()) {
    uint8_t newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);

    // there are no incoming message
    if(!newMessagesCount) return;

    measureSet<int16_t> *measures = measureArrayGetter();

    while(newMessagesCount) {
        for (uint8_t i = 0; i < newMessagesCount; i++) {
            replay(i, time, measures);
        }
        newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);
    }
}

void TelegramService::replay(uint8_t messageNum, timePack time, measureSet<int16_t> *measures) {
    String message = _bot.messages[messageNum].text;

    bool isLatinF = message[0] == 'f' || message[0] == 'F';
    bool isCyrillicP = message[0] == 0xd0 && (message[1] == 0x9f || message[1] == 0xbf);
    bool printFullBlock = isLatinF || isCyrillicP;

    char *result;
    if (printFullBlock) {
        result = buildMeasuresFullBlock(measures);
    } else {
        result = buildMeasuresShortBlock(measures);
    }

    char fullString[strlen(time.timeDate) + strlen(result)];
    sprintf(fullString, "\t%s\n%s", time.timeDate,result);

    _bot.sendMessage(_bot.messages[messageNum].chat_id, result, "");
    free((void *) result);
}

char *TelegramService::buildMeasuresShortBlock(measureSet<int16_t> *measures) {
    uint16_t blockSize = 0;
    for (uint8_t i = 0; i < MEASURE_TYPES_COUNT + DALLAS_SENSORS_COUNT; i++) {
        blockSize += strlen(measures[i].title) + MEASURE_STRING_SIZE;
    }
    char *result = (char *) calloc(blockSize, sizeof(char*));
    for (uint8_t i = 0; i < 5; i++) {
        char block[strlen(measures[i].title) + MEASURE_STRING_SIZE];
        sprintf(block, "\t%s: %d\n", measures[i].title, measures[i].curValue);
        strcat(result, block);
    }

    return result;
}

char *TelegramService::buildMeasuresFullBlock(measureSet<int16_t> *measures) {
    int16_t additionalDataSize = strlen(_str_current) + strlen(_str_min) + strlen(_str_max) + strlen(_str_av);
    uint16_t blockSize = 0;
    for (uint8_t i = 0; i < MEASURE_TYPES_COUNT + DALLAS_SENSORS_COUNT; i++) {
        blockSize += strlen(measures[i].title) + additionalDataSize + MEASURE_STRING_SIZE;
    }
    char *result = (char *) calloc(blockSize, sizeof(char*));
    for (uint8_t i = 0; i < MEASURE_TYPES_COUNT + DALLAS_SENSORS_COUNT; i++) {
        measureSet<int16_t> measure = measures[i];
        char block[strlen(measure.title) + additionalDataSize + MEASURE_STRING_SIZE];
        sprintf(
                block,
                "\t%s\n\t\t%s: %d\n\t\t%s: %d\n\t\t%s: %d\n\t\t%s: %d\n\n",
                measure.title,
                _str_current, measure.curValue,
                _str_min, measure.min,
                _str_max, measure.max,
                _str_av, (int16_t)(measure.average + 0.5f)
        );
        strcat(result, block);
    }

    return result;
}