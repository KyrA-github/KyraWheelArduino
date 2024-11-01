#include "../inc/Core.hpp"
Core::Core() : wheelMaxDeg(180), scense(1), encCounter(0), lastTimeDisplay(0), lastTimePedal(0), lastTimeGamepad(0), message(nullptr), 
INVERSALEWHEEL(false), INVERTEDPEDAL(false), button1Pin(15), button2Pin(14), button3Pin(16), gasMin(0), gasMax(1023), brakeMin(0), brakeMax(1023), clutchMin(0), clutchMax(1023), REFLASHINGMODE(false)
{
    Serial.begin(9600);

    Gamepad.begin();

    display = new Display(128, 64);
    pedal = new Pedal(2000, 10);
    button1 = new GButton(button1Pin);
    button2 = new GButton(button2Pin);
    button3 = new GButton(button3Pin);

    button1->setDebounce(50);        // настройка антидребезга (по умолчанию 80 мс)
    button1->setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)
    button1->setClickTimeout(600);   // настройка таймаута между кликами (по умолчанию 300 мс)

    button2->setDebounce(50); 
    button2->setTimeout(1000);
    button2->setClickTimeout(600);

    button3->setDebounce(50);
    button3->setTimeout(1000);
    button3->setClickTimeout(600);


    if (!pedal->init()) {
        Serial.println("init failed");
        while (1);
    }

    display->clear();

    getingFromEEPROM();
}

Core::~Core()
{
    //if (encoder) delete encoder;
    if (button1) delete button1;
    if (button2) delete button2;
    if (button3) delete button3;
    if (pedal) delete pedal;
    if (display) delete display;
    if (message) delete[] message; 
}

void Core::gameLoop(int en) {
    encCounter = en;
    if (!REFLASHINGMODE) {
        pedalLogic();
       // logicEncoder();
        gamepad();
        scenses();
    }
    Keyboard();
}


void Core::pedalLogic() {
    if (millis() - lastTimePedal > 10) {
        lastTimePedal = millis();
        if (pedal != nullptr) {
            if (pedal->lisening()) {
                message = pedal->getMessage();
            } else {
                message = nullptr;
            }
        }
    }
}

void Core::scenses() {
    if (millis() - lastTimeDisplay > 90) {
        lastTimeDisplay = millis();
        display->clear();
        if (scense == 1){
            if (message != nullptr) {
                display->setScene1(
                    constrain(message[0], 0, 100),
                     constrain(message[1], 0, 100),
                      constrain(message[2], 0, 100),
                       constrain(encCounter, -wheelMaxDeg, wheelMaxDeg),
                        message[3], message[4]);
                display->setConnectionIndicator(true);
            } else {
                display->setScene1(00, 00, 00, encCounter, 0, 0);
                display->setConnectionIndicator(false);
            }
        } else if (scense == 2){
             if (message != nullptr) {
                display->setConnectionIndicator(true);
                if (collibrationSens == 0) {
                    display->setScene2(message[0], message[1], message[2], encCounter, 0, 0);
                    gasMax = message[0];
                    brakeMax = message[1];
                    clutchMax = message[2];
                    display->setScene2(gasMax, brakeMax, clutchMax, encCounter, 0, 0);
                    display->drawSave(true);
                    if (button3->isSingle()) {
                        collibrationSens = 1;
                        EEPROM.put(2, gasMax);
                        EEPROM.put(6, brakeMax);
                        EEPROM.put(10, clutchMax);
                        EEPROM.put(12, abs(encCounter));
                    }
                } else if (collibrationSens == 1) {
                    display->setScene2(message[0], message[1], message[2], encCounter, 0, 0);
                    gasMax = message[0];
                    brakeMax = message[1];
                    clutchMax = message[2];
                    display->setScene2(gasMax, brakeMax, clutchMax, encCounter, 0, 0);
                    display->drawSave(true);
                    if (button3->isSingle()) {
                        collibrationSens = 0;
                        EEPROM.put(0, gasMin);
                        EEPROM.put(4, brakeMin);
                        EEPROM.put(8, clutchMin);
                    }
                }
            } else {
                display->setScene2(00, 00, 00, encCounter, 0, 0);
                display->setConnectionIndicator(false);
            }
        } 
        display->updateLogic();
        display->update();
    } 
}

void Core::gamepad() {
    if (millis() - lastTimeGamepad > 10) {
        lastTimeGamepad = millis();
        int wheel;
        if (INVERSALEWHEEL) wheel = constrain(-encCounter, -wheelMaxDeg, wheelMaxDeg);
        else wheel = constrain(encCounter, -wheelMaxDeg, wheelMaxDeg);
        wheel = map(wheel, -wheelMaxDeg, wheelMaxDeg, -32768, 32767);
        Gamepad.xAxis(wheel);

        int8_t gas = 0, brake = 0, clutch = 0;  // Инициализация значений
        
        if (message != nullptr) {
            gas = map(message[0], gasMin, gasMax, -32768, 32767);
            brake = map(message[1], brakeMin, brakeMax, -32768, 32767);
            clutch = map(message[2], clutchMin, clutchMax, -32768, 32767);
            if (INVERTEDPEDAL) {
                gas = -gas;
                brake = -brake;
                clutch = -clutch;
            }
        }
        Gamepad.xAxis(wheel);  // Устанавливаем ось X
        Gamepad.yAxis(gas);    // Устанавливаем ось Y (значение газа)
        Gamepad.zAxis(brake);  // Устанавливаем ось Z (значение тормоза)
        Gamepad.rzAxis(clutch);  // Устанавливаем ось Rz (значение сцепления)
        Gamepad.write();
    }
}


void Core::savingToEEPROM() {
    EEPROM.put(0, gasMin);
    EEPROM.put(2, gasMax);
    EEPROM.put(4, brakeMin);
    EEPROM.put(6, brakeMax);
    EEPROM.put(8, clutchMin);
    EEPROM.put(10, clutchMax);
    EEPROM.put(12, wheelMaxDeg);
    EEPROM.put(14, INVERSALEWHEEL);
}


void Core::getingFromEEPROM() {
    EEPROM.get(0, gasMin);
    EEPROM.get(2, gasMax);
    EEPROM.get(4, brakeMin);
    EEPROM.get(6, brakeMax);
    EEPROM.get(8, clutchMin);
    EEPROM.get(10, clutchMax);
    EEPROM.get(12, wheelMaxDeg);
    EEPROM.get(14, INVERSALEWHEEL);
}

void Core::Keyboard() {
    button1->tick();
    button2->tick();
    button3->tick();
    if (button1->isTriple()) {  
        REFLASHINGMODE = !REFLASHINGMODE; 
    }

    if (button2->isSingle())
    {
        if (scense >= 3){
            scense = 1;
            collibrationSens = 0;
        } else {
            scense++;
            collibrationSens = 0;
        }
    }
   
}

