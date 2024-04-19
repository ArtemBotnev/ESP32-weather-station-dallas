// Weather station
// Copyright Artem Botnev 2024
// MIT License

#ifndef NETWORK_H
#define NETWORK_H

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>

#include "telegram_service.h"
#include "../common.h"

class NetworkManager {

public:
    NetworkManager();

    /**
     * wifi network initialization
     * @param ssid - wifi network ssid
     * @param password - wifi network password
     */
    void init(const char* ssid, const char* password);

    /**
     * checks if wifi connection is established
     * @return true if connection is established, otherwise false
     */
    bool connectionEstablished();

    /**
     * runs all tasks related to network
     * @param time - timePack representing measures time
     * @param measureArrayGetter - function for receiving measures data
     */
    void runTasks(timePack time, measureSet<int16_t> *(*measureArrayGetter)());

    /**
     * telegram service initialization
     * @param token - telegram bot token
     */
    void initTelegramService(const char* token);

    /**
     * real time from ntp server
     * @param ntpServer - ntp server address
     * @param timeZone - time zone GMT
     * @return real time in milliseconds with time zone offset
     */
    uint32_t getNtpMilliseconds(const char *ntpServer, uint8_t timeZone);

private:
    WiFiClientSecure _client;
    TelegramService *_telegramService;
};

#endif //NETWORK_H
