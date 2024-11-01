#include "inc/Core.hpp"

Core* core = nullptr; 

// Тип энкодера
enum EncoderType {
    TYPE_1 = 1,
    TYPE_2
};

EncoderType ENC_TYPE = TYPE_1;

// Пины энкодера
int encoderPinA = 5;
int encoderPinB = 6;

volatile int encCounter = 0;    // Счетчик энкодера
volatile boolean lastState;     // Последнее состояние канала A энкодера
volatile boolean turnFlag;      // Флаг для типа энкодера

void setup() {
    core = new Core();  // Инициализируем ядро
    pinMode(encoderPinA, INPUT); // Настраиваем пин A как вход
    pinMode(encoderPinB, INPUT); // Настраиваем пин B как вход
    
    lastState = digitalRead(encoderPinA);  // Читаем начальное состояние пина A
    setupTmr();  // Настраиваем таймер для опроса энкодера
}

// Функция обработки изменения состояния энкодера
void encTick() {
    bool currentState = digitalRead(encoderPinA);  // Чтение текущего состояния пина A
    if (currentState != lastState) {  // Проверка изменения состояния
        if (ENC_TYPE == TYPE_1) {
            // Логика для энкодера первого типа
            turnFlag = !turnFlag;  // Инвертируем флаг при каждом изменении
            if (turnFlag) {
                // Увеличиваем или уменьшаем счетчик в зависимости от состояния пина B
                encCounter += (digitalRead(encoderPinB) != lastState) ? -1 : 1;
            }
        } else if (ENC_TYPE == TYPE_2) {
            // Логика для второго типа энкодера
            encCounter += (digitalRead(encoderPinB) != lastState) ? -1 : 1;
        }
        lastState = currentState;  // Обновляем предыдущее состояние
    }
}

void loop() {
    if (core != nullptr) {
        if (!digitalRead(15)) {  
            encCounter = 0;
        } 
        core->gameLoop(encCounter);  // Передаем текущее значение энкодера в основную логику игры
    }
}

// Настройка таймера 3, канал А, с периодом 0.5 мс для опроса энкодера
void setupTmr() {
  noInterrupts();  // Отключаем прерывания на время настройки таймера
  TCCR3A = 0;      // Очищаем регистр управления таймером A
  TCCR3B = 0b00001010;  // Выбираем делитель 8 и режим CTC (Clear Timer on Compare Match)
  TIMSK3 = 0b00000010;  // Разрешаем прерывания по сравнению с OCR3A
  OCR3A = 999;  // Устанавливаем значение для создания периода 0.5 мс (при частоте 16 МГц и делителе 8)
  interrupts();  // Включаем прерывания
}

// Опрос энкодера в прерывании
ISR(TIMER3_COMPA_vect) {
  encTick();  // Обработка энкодера
}
