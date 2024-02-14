// Weather station
// Copyright Artem Botnev 2023
// MIT License

// The project use SPIFFS file system available for ESP32 which allows to store current state.
/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
// If you don't want to use storage just turn USE_STORAGE constant to false.

#include <Wire.h>
#include <OneWire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DallasTemperature.h>

#include "src/common.h"
#include "src/sensors.h"
#include "src/display/display.h"
#include "src/clock/clock.h"
#include "src/storage/storage.h"
//#include "src/network/network.h"

#define SENSOR_DELAY 100
#define SCREEN_DELAY 2800

#define I2C_SDA 33
#define I2C_SCL 32

#define ONE_WIRE_BUS 14

// Turn to false if you don't want to use storage
#define USE_STORAGE true
// Turn to false if you don't want to use network
#define NETWORK_ENABLED true
// Turn to false if you don't want to use telegram (available only if network enabled)
#define TELEGRAM_ENABLED true

#define SSID "XXXXXX"     // your network SSID (name)
#define PASSWORD "YYYYYY" // your network key
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)

Adafruit_BME280 bme;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Display display;
TClock cl;
DataManager dataManager;
//NetworkManager networkManager;

bool isNetAvailable;

bool storageIsAvailable;

//measureSet<int16_t> *getMeasuresArray() {
//    return dataManager.getMeasuresArray();
//}

// you can use a lot of dallas sensors
dallasSensor dallasArr[DALLAS_SENSORS_COUNT] = {
        // change to yours title and address
        { "Out t, C", { 0x28, 0xFF, 0x71, 0x38, 0x90, 0x15, 0x03, 0x65 } },
        { "Balcony t, C", { 0x28, 0xFF, 0x84, 0x3A, 0x90, 0x15, 0x03, 0x16 } },
};

void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);
    bme.begin();

    display.begin();
    display.showTitle = true;
    display.showAdditionData = true;

    cl.init();

//    if (USE_STORAGE) {
//        storageIsAvailable = dataManager.initStorage(cl.getTimePack());
//    }

//    if (NETWORK_ENABLED) {
//        networkManager.init(SSID, PASSWORD);
//        if (TELEGRAM_ENABLED) networkManager.initTelegramService(BOT_TOKEN);
//    }
}

void loop() {
    display.setTitle(cl.getTimeString());

    readTemperatureAndShow();
    readHumidityAndShow();
    readAtmPressureAndShow();

    timePack time = cl.getTimePack();

    if (USE_STORAGE) {
        // reset additional data if a new day has come
        if (cl.isNewDay()) dataManager.clearCache();
//        dataManager.updateTimeData(time);
    }
//
//    if (NETWORK_ENABLED) doNetWork(time, getMeasuresArray);
}

void readTemperatureAndShow() {
    // read room temp
    int16_t roomCurrentTemper = round(bme.readTemperature());
    delay(SENSOR_DELAY);
    measureSet<int16_t> roomT = dataManager.getMeasureSet(ROOM_TEMPER_TITLE, ROOM_TEMPER_INDEX, roomCurrentTemper);
//    display.setTitle(cl.getTimeString());
    display.drawRoomTemperatureMenu(roomT);
    delay(SCREEN_DELAY);
    // read dallas sensors
    sensors.requestTemperatures();
    int8_t dallasArraySize = sizeof(dallasArr) / sizeof(dallasArr[0]);
    for (int8_t i = 0; i < dallasArraySize; i = i + 2) {
        int8_t firstDallasIndex = MEASURE_TYPES_COUNT;
        measureSet<int16_t> measure1 =dataManager
                .getMeasureSet(dallasArr[i].title, firstDallasIndex, readDallasSensor(dallasArr[i].addr));
        if (dallasArraySize > i + 1) {
            int8_t dallasIndex = firstDallasIndex + i;
            measureSet<int16_t> measure2 = dataManager
                    .getMeasureSet(dallasArr[i + 1].title, dallasIndex, readDallasSensor(dallasArr[i + 1].addr));
            display.setTitle(cl.getTimeString());
            display.drawDoubleTemperatureMenu(measure1, measure2);
        } else {
            display.drawSingleTemperatureMenu(measure1);
        }
        delay(SCREEN_DELAY);
    }
}

void readHumidityAndShow() {
    uint16_t currentHumidity = round(bme.readHumidity());
    delay(SENSOR_DELAY);
    measureSet<int16_t> humidity = dataManager.getMeasureSet(ROOM_HUM_TITLE, ROOM_HUM_INDEX, currentHumidity);

    display.drawHumidityMenu(humidity);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    // mmhg
    uint16_t currentPressure = round(bme.readPressure() * 0.0075F);
    delay(2 * SENSOR_DELAY);

    measureSet<int16_t> pressure = dataManager.getMeasureSet(PRESSURE_TITLE, PRESSURE_INDEX, currentPressure);

    display.drawAtmPressureMenu(pressure);
    delay(SCREEN_DELAY);
}

//void doNetWork(timePack time, measureSet<int16_t> *(*measureArrayGetter)()) {
//    if (isNetAvailable = networkManager.connectionEstablished()) {
//        networkManager.runTasks(time, measureArrayGetter);
//    }
//}

float readDallasSensor(uint8_t addr[8]) {
    return sensors.getTempC(addr);
}
