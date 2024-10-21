#include "../inc/Core.hpp"
Core::Core() : wheelMaxDeg(180), scense(1), encCounter(0), lastTimeDisplay(0), lastTimePedal(0), lastTimeGamepad(0), message(nullptr), button1State(0), button2State(0), button3State(0), 
INVERSALEWHEEL(false), INVERTEDPEDAL(false), button1Pin(15), button2Pin(14), button3Pin(16), gasMin(0), gasMax(1023), brakeMin(0), brakeMax(1023), clutchMin(0), clutchMax(1023), firmwareMode(false)
{
    Serial.begin(9600);
    Gamepad.begin();
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);

    display = new Display(128, 64);
    pedal = new Pedal(2000, 9);
    //encoder = new Encoder(5, 6, 13);

    //encoder->setType(TYPE2); // TYPE1, TYPE2
    if (!pedal->init()) {
        Serial.println("init failed");
        while (1);
    }
    display->clear();
}

Core::~Core()
{
    //if (encoder) delete encoder;
    if (pedal) delete pedal;
    if (display) delete display;
    if (message) delete[] message; 
}

void Core::gameLoop(int en) {
    encCounter = en;
    if (firmwareMode) {
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
                display->setScene1(-99, -99, -99, encCounter, 0, 0);
                display->setConnectionIndicator(false);
            }
            display->update();

        } else if (scense == 2){

        } else if (scense == 3){ 

        }
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

// void Core::logicEncoder() {
//     if (encoder != nullptr) {
//         encoder->tick();
//         if (encoder->isRight()) encCounter++;    
//         if (encoder->isLeft()) encCounter--;
//     }
// }

void Core::savingToEEPROM() {

}

void Core::getingFromEEPROM() {
    
}

void Core::Keyboard() {
    if (button1State != digitalRead(button1Pin)) {  
        button1State = false;
        firmwareMode = !firmwareMode;
    }

    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
    button3State = digitalRead(button3Pin);
}

