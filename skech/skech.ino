///
/// KyraWheel
///

// =========== НАСТРОЙКИ ===========
bool DEBUG = 0; // 0 = off, 1 = on отладочный режим
#define DISPLAY 0 // 0 = off, 1 = on отображение на дисплее
#define INVERTEDWHEEL 1 // 0 = off, 1 = on инверсия Рулевого колеса
#define INVERTEDPEDAL 1 // 0 = off, 1 = on инверсия Педалей
#define ENC_TYPE 1    // тип энкодера, 0 или 1


// =========== ПИНЫ ===========
#define encoderPinA 5 // пин A энкодера
#define encoderPinB 6 // пин B энкодера

#define button1Pin 15 // пин Кнопки1 
#define button2Pin 16 // пин Кнопки2 

#define pedalGasPin A0 // пин Газа
#define pedalBrakePin A1 // пин тормоза
#define pedalClutchPin A2 // пин Сцепления

#include <EEPROM.h>
#include "HID-Project.h"

volatile int encCounter = 0;
volatile boolean state0, lastState, turnFlag;
uint32_t timer;

int gasMin = 0, gasMax = 0, brakeMin = 0, brakeMax = 0, clutchMin = 0, clutchMax = 0;
int  wheelMaxDeg = 0;

bool button1State = false, button2State = false;

void setup() {
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    setupTmr();
    EEPROM.get(0, gasMin);
    EEPROM.get(2, brakeMin);
    EEPROM.get(4, clutchMin);
    EEPROM.get(6, wheelMaxDeg);
    EEPROM.get(8, gasMax);
    EEPROM.get(10, brakeMax);
    EEPROM.get(12, clutchMax);
    timer = millis();
    if (!digitalRead(button1Pin) && button1State == 0) {
        button1State = 1;
        DEBUG = 1; 
        Serial.begin(9600);
    }
    Gamepad.begin();
    Serial.begin(9600);
}
void encTick() {
    bool currentState = digitalRead(encoderPinA); // Чтение текущего состояния пина A энкодера
    if (currentState != lastState) { // Проверка изменения состояния
        // В зависимости от типа энкодера, применяем разную логику
        if (ENC_TYPE == 1) {
            turnFlag = !turnFlag; // Инвертируем флаг при каждом изменении
            if (turnFlag) {
                // Увеличиваем или уменьшаем счетчик в зависимости от состояния пина B
                encCounter += (digitalRead(encoderPinB) != lastState) ? -1 : 1;
            }
        } else {
            // Для другого типа энкодера - просто корректируем счетчик
            encCounter += (digitalRead(encoderPinB) != lastState) ? -1 : 1;
        }
        lastState = currentState; // Обновляем предыдущее состояние
    }
}


void loop() {
    if (!digitalRead(button2Pin) && button2State == 0){
        button2State = 1;
        DEBUG = 1;
    }
    if (DEBUG) {
        if (!digitalRead(button1Pin) && button1State == 0) {
            button1State = 1;
            calibrate(1);
        }
        if (!digitalRead(button2Pin) && button2State == 0) {
            button2State = 1;
            DEBUG = 0;
            calibrate(0);
        }
    } else {
       if (!digitalRead(button1Pin) && button1State == 0) {
            button1State = 1;
            encCounter = 0;
        }
        logicGamepad();
    }
        
    button1State = !digitalRead(button1Pin);
    button2State = !digitalRead(button2Pin);
}

void calibrate(int value) {
    int gas = analogRead(pedalGasPin);
    int brake = analogRead(pedalBrakePin);
    int clutch = analogRead(pedalClutchPin);
    if (value == 1) {
        Serial.begin(9600);
        Serial.println(F("Calibration begin"));
        int gas = analogRead(pedalGasPin);
        int brake = analogRead(pedalBrakePin);
        int clutch = analogRead(pedalClutchPin);
        EEPROM.put(6, abs(encCounter));
        EEPROM.put(8, gas);
        EEPROM.put(10, brake);
        EEPROM.put(12, clutch);
        clutchMax = clutch;
        brakeMax = brake;
        gasMax = gas;
    } else if (value == 0) {
        clutchMin = clutch;
        brakeMin = brake;
        gasMin = gas;
        EEPROM.put(0, gas);
        EEPROM.put(2, brake);
        EEPROM.put(4, clutch);
        Serial.println(F("Calibration end"));
        Serial.print(F("Wheel: "));
        Serial.println(abs(encCounter));
        Serial.print(F("ThroatMax: "));
        Serial.print(gasMax);
        Serial.print(" - ");
        Serial.println(gasMin);
        Serial.print(F("Brake: "));
        Serial.print(brakeMax);
        Serial.print(" - ");
        Serial.println(brakeMin);
        Serial.print(F("Clutch: "));
        Serial.print(clutchMax);
        Serial.print(" - ");
        Serial.println(clutchMin);
        Serial.end();
    }
}


void logicGamepad() {
    if (millis() - timer > 10) {
        timer = millis();
        int wheel;
        if (INVERTEDWHEEL) wheel = constrain(-encCounter, -wheelMaxDeg, wheelMaxDeg);
        else wheel = constrain(encCounter, -wheelMaxDeg, wheelMaxDeg);
        wheel = map(wheel, -wheelMaxDeg, wheelMaxDeg, -32768, 32767);
        
        int gas, brake, clutch;
        gas = map(analogRead(pedalGasPin), gasMin-10, gasMax+10, -32768, 32767);
        brake = map(analogRead(pedalBrakePin), brakeMax-10, brakeMin-15, -128, 127);
        clutch = map(analogRead(pedalClutchPin), clutchMin, clutchMax, -128, 127);
        //gas = constrain(gas, -128, 127);
        //brake = constrain(brake, -128, 127);
        //clutch = constrain(clutch, -128, 127);
        if (INVERTEDPEDAL) {
            gas = -gas;
            brake = -brake;
            clutch = -clutch;
        }
        Gamepad.xAxis(wheel);  // Устанавливаем ось X
      //  Gamepad.yAxis(gas);  // Устанавливаем ось Y
      //  Gamepad.zAxis(brake);  // Устанавливаем ось Z
       // Gamepad.rzAxis(clutch);  // Устанавливаем ось Rz
        Gamepad.write(); 
    }
}

// опрос энкодера в прерывании
ISR(TIMER3_COMPA_vect) {
  encTick();
}

// ставим таймер 3, канал А, период 0.5 мс. Для опроса энкодера
void setupTmr() {
  TCCR3B = 0b00001001;
  TIMSK3 = 0b00000010;
  OCR3AH = highByte(15999 / 2);
  OCR3AL = lowByte(15999 / 2);
}
