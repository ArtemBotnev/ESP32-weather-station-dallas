// Weather station
// Copyright Artem Botnev 2023
// MIT License

#include "display.h"

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

Display::Display() = default;

void Display::begin() {
    tft.begin();
    // rotate screen on 180
    tft.setRotation(ROTATE_180);
}

void Display::setScreenColor(uint16_t color) {
    _screenColor = color;
}

void Display::fillScreen() {
    tft.fillScreen(_screenColor);
}

void Display::setTitle(const char *title) {
    _title = (char*) title;
}

void Display::drawHumidityMenu(measureSet<int16_t> humidity) {
    fillScreen();
    // header
    drawHeadMenu();
    // center
    drawMenuCell(humidity, 45, CENTER_MENU_SHIFT_Y, get_humidity_color);
    drawBottom();
}

void Display::drawAtmPressureMenu(measureSet<int16_t> press) {
    fillScreen();
    drawHeadMenu();
    tft.setCursor(10, 25);
    drawMenuCell(press, 10, CENTER_MENU_SHIFT_Y, get_atm_press_color);
    drawBottom();
}

void Display::drawRoomTemperatureMenu(measureSet<int16_t> temperature) {
    fillScreen();
    // header
    drawHeadMenu();
    // center
    drawMenuCell(temperature, 68, CENTER_MENU_SHIFT_Y, get_room_temper_color);
    drawBottom();
}

void Display::drawSingleTemperatureMenu(measureSet<int16_t> temperature) {
    fillScreen();
    // header
    drawHeadMenu();
    // center
    drawMenuCell(temperature, DEFAULT_SHIFT_X, CENTER_MENU_SHIFT_Y, get_dallas_temper_color);
    drawBottom();
}

void Display::drawDoubleTemperatureMenu(measureSet<int16_t> firstTemperature, measureSet<int16_t> secondTemperature) {
    fillScreen();
    // header
    drawHeadMenu();
    // top
    drawMenuCell(firstTemperature, DEFAULT_SHIFT_X, TOP_MENU_SHIFT_Y, get_dallas_temper_color);
    // bottom
    drawMenuCell(secondTemperature, DEFAULT_SHIFT_X, BOTTOM_MENU_SHIFT_Y, get_dallas_temper_color);
    drawBottom();
}

void Display::drawMenuCell(
        measureSet<int16_t> measure,
        uint8_t shiftX,
        uint8_t shiftY,
        uint16_t (*value_color)(int16_t)) {

    tft.setCursor(shiftX, 5 + shiftY);

    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(measure.title);

    uint8_t y = 42 + shiftY;
    if (measure.curValue < -9){
        tft.setCursor(35, y);
    } else if (measure.curValue < 0) {
        tft.setCursor(60, y);
    } else if (measure.curValue < 10) {
        tft.setCursor(95, y);
    } else if (measure.curValue < 100) {
        tft.setCursor(68, y);
    } else {
        tft.setCursor(35, y);
    }

    // current value
    tft.setTextColor(value_color(measure.curValue));
    tft.setTextSize(VALUE_TEXT_SIZE);
    tft.print(measure.curValue);

    drawAdditionalData(shiftY, measure, value_color);
}

void Display::drawAdditionalData(uint8_t shiftY, measureSet<int16_t> measure, uint16_t (*value_color)(int16_t)) {
    if (!showAdditionData) return;

    uint16_t yb = 120 + shiftY;
    // min value
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.setCursor(20, yb);
    tft.setTextColor(value_color(measure.min));
    tft.print(measure.min);
    // average value
    tft.setCursor(110, yb);
    int16_t average = round(measure.average);
    tft.setTextColor(value_color(average));
    tft.print(average);
    // max value
    tft.setCursor(200, yb);
    tft.setTextColor(value_color(measure.max));
    tft.print(measure.max);
}

void Display::drawHeadMenu() {
    if (!showTitle) return;

    tft.setCursor(25, 7);
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(_title);
}

void Display::drawBottom() {
    if (!showAdditionData) return;

    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);

    tft.setCursor(12, 300);
    tft.print(MIN);
    tft.setCursor(78, 300);
    tft.print(AVERAGE);
    tft.setCursor(192, 300);
    tft.print(MAX);
}

int16_t Display::round(float value) {
    return (int16_t)(value + 0.5f);
}
