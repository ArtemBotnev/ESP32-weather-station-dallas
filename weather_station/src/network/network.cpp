// Weather station
// Copyright Artem Botnev 2024
// MIT License

#include "network.h"

NetworkManager::NetworkManager() = default;

void NetworkManager::init(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

bool NetworkManager::connectionEstablished() {
    return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::runTasks(timePack time, measureSet<int16_t> *(*measureArrayGetter)()) {
    if (_telegramService) {
        _telegramService->botInteraction(time, measureArrayGetter);
    }
}

void NetworkManager::initTelegramService(const char* token) {
    _telegramService = new TelegramService(token, &_client);
}

uint32_t NetworkManager::getNtpMilliseconds(const char *ntpServer, uint8_t timeZone) {
    WiFiUDP ntpUDP;
    uint64_t offset = 60*60*timeZone;
    NTPClient timeClient(ntpUDP, *ntpServer, offset);
    delay(500);

    timeClient.begin();
    timeClient.update();

    return timeClient.getEpochTime();
}
