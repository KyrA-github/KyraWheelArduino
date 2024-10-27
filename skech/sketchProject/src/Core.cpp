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
    getingFromEEPROM();
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
                display->setScene1(00, 00, 00, encCounter, 0, 0);
                display->setConnectionIndicator(false);
            }
            

        } else if (scense == 2){
            display->setScene2(00, 00, 00, encCounter, 0, 0);

        } else if (scense == 3){ 
            display->setScene3(mode50deg, 0);
            if (button1State != digitalRead(button1Pin)) {  
                button1State = false;
                mode50deg = !mode50deg;
                EEPROM.put(14, mode50deg);
                display->drawSave(true);
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

// void Core::logicEncoder() {
//     if (encoder != nullptr) {
//         encoder->tick();
//         if (encoder->isRight()) encCounter++;    
//         if (encoder->isLeft()) encCounter--;
//     }
// }

void Core::savingToEEPROM() {
    EEPROM.put(0, gasMin);
    EEPROM.put(2, gasMax);
    EEPROM.put(4, brakeMin);
    EEPROM.put(6, brakeMax);
    EEPROM.put(8, clutchMin);
    EEPROM.put(10, clutchMax);
    EEPROM.put(12, wheelMaxDeg);
    EEPROM.put(14, mode50deg);
    EEPROM.put(15, INVERSALEWHEEL);
}


void Core::getingFromEEPROM() {
    EEPROM.get(0, gasMin);
    EEPROM.get(2, gasMax);
    EEPROM.get(4, brakeMin);
    EEPROM.get(6, brakeMax);
    EEPROM.get(8, clutchMin);
    EEPROM.get(10, clutchMax);
    EEPROM.get(12, wheelMaxDeg);
    EEPROM.get(14, mode50deg);
    EEPROM.get(15, INVERSALEWHEEL);
}

void Core::Keyboard() {
    if (button1State != digitalRead(button1Pin)) {  
        button1State = false;
        firmwareMode = !firmwareMode; 
    }
    if (!digitalRead(button3Pin))
    {
        if (button1Counter > 400) {
            button1Counter = 0;
            button1State = true;
            if (scense => 3) scense = 1;
            else scense++;
        }
        button1Counter++;
    }
    if (button3State != digitalRead(button3Pin)) {  
        button3State = false;
        button1Counter = 0;
        encCounter = 0;
    }
    if (!digitalRead(button3Pin))
    {
        if (button3Counter > 400) {
            button1State = false;
            firmwareMode = !firmwareMode; 
        }
        button3Counter++;
    }

    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
    button3State = digitalRead(button3Pin);
}

// const int buttonPin = 2;       // Пин кнопки
// const int shortPressTime = 500; // Время для определения короткого нажатия (500 мс)
// bool buttonState = false;
// unsigned long pressStartTime = 0;

// void setup() {
//   pinMode(buttonPin, INPUT_PULLUP); // Настройка кнопки с подтяжкой к питанию
//   Serial.begin(9600);               // Запуск последовательного порта для отладки
// }

// void loop() {
//   bool currentState = digitalRead(buttonPin) == LOW;

//   // Если кнопка только что была нажата
//   if (currentState && !buttonState) {
//     pressStartTime = millis(); // Записываем время начала нажатия
//   }
  
//   // Если кнопка была отпущена после нажатия
//   if (!currentState && buttonState) {
//     unsigned long pressDuration = millis() - pressStartTime; // Длительность нажатия

//     // Короткое нажатие
//     if (pressDuration < shortPressTime) {
//       Serial.println("Короткое нажатие: выполняется действие 1");
//       // Действие для короткого нажатия
//     }
//     // Длинное нажатие
//     else {
//       Serial.println("Длинное нажатие: выполняется действие 2");
//       // Действие для длинного нажатия
//     }
//   }

//   // Обновляем текущее состояние кнопки
//   buttonState = currentState;
// }
