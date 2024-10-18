#include "../inc/Core.hpp"
Core::Core()
{
    Gamepad.begin();

    display = new Display(128, 64);
    pedal = new Pedal(2000, 9);
    encoder = new Encoder(3, 4, 13);

    encoder->setType(TYPE2); // TYPE1, TYPE2
    if (!pedal->init()) {
        Serial.println("init failed");
        while (1);
    }
    display->clear();
}

Core::~Core()
{
}

void Core::gameLoop() {
    if (display != nullptr) {
        scenses();
        pedal();
    }
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
    if (millis() - lastTimeDisplay > 150) {
        lastTimeDisplay = millis();
        if (scense == 1){

        } else if (scense == 2){

        } else if (scense == 3){ 

        }
    } 
}

void Core::gamepad() {
    if (millis() - lastTimeGamepad > 10) {
        lastTimeGamepad = millis();
        int wheel;
        if (INVERTEDWHEEL) wheel = constrain(-encCounter, -wheelMaxDeg, wheelMaxDeg);
        else wheel = constrain(encCounter, -wheelMaxDeg, wheelMaxDeg);
        wheel = map(wheel, -wheelMaxDeg, wheelMaxDeg, -32768, 32767);
        Gamepad.yAxis(gas);


        if (message != nullptr) {
            int gas, brake, clutch;
            gas = map(message[0], gasMin, gasMax, -32768, 32767);
            brake = map(message[1], brakeMin, brakeMax, -128, 127);
            clutch = map(, clutchMin, clutchMax, -128, 127);
            if (INVERTEDPEDAL) {
                gas = -gas;
                brake = -brake;
                clutch = -clutch;
            }
            Gamepad.xAxis(wheel);  // Устанавливаем ось X
            Gamepad.zAxis(brake);  // Устанавливаем ось Z
            Gamepad.rzAxis(clutch);  // Устанавливаем ось Rz
        }
        Gamepad.write(); 
    }
}

void Core::logicEncoder() {
    if (encoder != nullptr) {
        encoder.tick();
        if (encoder.isRight()) encCounter++;    
        if (encoder.isLeft()) encCounter--;
    }
}

void Core::savedInEEPROM() {

}

void Core::getingFromEEPROM() {
    
}






