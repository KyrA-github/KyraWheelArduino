#ifndef CORE_HPP
#define CORE_HPP

#include <EEPROM.h>
//#include <GyverEncoder.h>
#include <GyverButton.h>
#include <HID-Project.h>

#include "display.hpp"


class Core
{
private:
   // Encoder* encoder = nullptr;
    

    
    Display* display = nullptr;    

    int gasMin = 0, gasMax = 1023;
    int brakeMin = 0, brakeMax = 1023;
    int clutchMin = 0, clutchMax = 1023;
    int wheelMaxDeg = 180;

    int scense = 1; 
    int collibrationSens = 0;
    volatile int encCounter = 0;
    unsigned long lastTimeDisplay = 0, lastTimePedal = 0, lastTimeGamepad = 0, lastTimefirmware = 0;

    int message[5] = {0, 0, 0, 0, 0};

    bool INVERSALEWHEEL = false;
    bool INVERTEDPEDAL = false;
    bool REFLASHINGMODE = false;

    // buttons
    const int button1Pin = 15, button2Pin = 14, button3Pin = 16;
    GButton* button1 = nullptr;
    GButton* button2 = nullptr;
    GButton* button3 = nullptr;

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