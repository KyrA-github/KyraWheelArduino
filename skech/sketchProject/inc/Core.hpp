#ifndef CORE_HPP
#define CORE_HPP

#include <EEPROM.h>
#include <GyverEncoder.h>
#include <HID-Project.h>

#include "display.hpp"
#include "Pedal.hpp"


class Core
{
private:
    Encoder* encoder = nullptr;

    Pedal* pedal = nullptr;
    Display* display = nullptr;    

    int gasMin = 0, gasMax = 1023, brakeMin = 0, brakeMax = 1023, clutchMin = 0, clutchMax = 1023;
    int wheelMaxDeg = 180;

    volatile int encCounter = 0;

    unsigned long lastTimeDisplay = 0, lastTimePedal = 0, lastTimeGamepad = 0;
    uint8_t* message = nullptr;

    bool button1State = 0, button2State = 0;

    bool INVERSALEWHEEL = false;
    bool INVERTEDPEDAL = false;

    void scenses();
    void pedalLogic(); 
    void gamepad();
    void logicEncoder();
public:
    Core();
    void gameLoop();
    ~Core();
};


#endif // CORE_HPP