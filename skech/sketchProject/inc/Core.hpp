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
   // Encoder* encoder = nullptr;

    Pedal* pedal = nullptr;
    Display* display = nullptr;    

    int gasMin = 0, gasMax = 1023, brakeMin = 0, brakeMax = 1023, clutchMin = 0, clutchMax = 1023;
    int wheelMaxDeg = 180;

    int scense = 1; 


    volatile int encCounter = 0;

    unsigned long lastTimeDisplay = 0, lastTimePedal = 0, lastTimeGamepad = 0;
    int8_t* message = nullptr;

    bool button1State = 0, button2State = 0, button3State = 0;

    bool INVERSALEWHEEL = false;
    bool INVERTEDPEDAL = false;
    bool firmwareMode = false;

    const int button1Pin = 15, button2Pin = 14, button3Pin = 16;

    void scenses();
    void pedalLogic(); 
    void gamepad();
    //void logicEncoder();
    void Keyboard();

    void getingFromEEPROM();
    void savingToEEPROM();
public:
    Core();
    void gameLoop(int en);
    ~Core();
};


#endif // CORE_HPP