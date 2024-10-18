#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class Display {
    public:
        Display(int SCREEN_W, int SCREEN_H);
        void update();
        void updateLogic();
        void clear();

        void setScene1(int gas, int brake, int clutch, int wheel, int HShifter, int gear); // сцена 1 основная
        void setScene2(int gas, int brake, int clutch, int wheel, int HShifter, int gear); // сцена 2 калибровка
        void setScene3(int gas, int brake, int clutch, int wheel, int HShifter, int gear); // сцена 3 установка режима работы
        void drawSave();

        void setConnectionIndicator(bool connected, bool show = true);
    private:
        Adafruit_SSD1306 display;

        int SCREEN_WIDTH, SCREEN_HEIGHT;
        unsigned long lastTimeSave;
        bool saveShow = false;
};

#endif // DISPLAY_HPP