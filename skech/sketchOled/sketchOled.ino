#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include <HID-Project.h>
#include <EEPROM.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int button1Pin = 15, button2Pin = 14, button3Pin = 16;
GButton* button1Up = nullptr;
GButton* button2Down = nullptr;
GButton* button3ok = nullptr;

int CursorPos = 0, encCount = 0, pedalGas = 0, pedalBrake = 0, pedalClutch = 0, wheel = 0, HShifter = 0, gear = 0;

int wheelValues[] = {90, 180, 360, 540, 720, 900, 1080}; // Возможные значения wheelDec
int currentWheelIndex = 0; // Индекс текущего значения wheelDec
int wheelDec = wheelValues[currentWheelIndex]; // Начальное значение

int pedalAdjustmentGasMin = 0,
    pedalAdjustmentGasMax = 0,

    pedalAdjustmentBrakeMin = 0,
    pedalAdjustmentBrakeMax = 0,

    pedalAdjustmentClutchMin = 0,
    pedalAdjustmentClutchMax = 0;

int STATUS = 0;
int adjustmentStatus = 0;
int adjustmentLowStatus = 0;
bool resetStatusAdjustment = false;
bool update = false;
bool readflag = false;

bool INVERTEDPEDAL = false;


int pinGas = A0, pinBrake = A1, pinClutch = A2;

int gasMin = 0, gasMax = 0, brakeMin = 0, brakeMax = 0, clutchMin = 0, clutchMax = 0;



/** 
 *  Инициализация
 *  
 *  Выполняет инициализацию дисплея, кнопок и серийного порта.
 *  Устанавливает настройки антидребезга, таймаута на удержание и таймаута
 *  между кликами для кнопок.
 *  Выводит на дисплей стартовое сообщение.
 */
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with I2C address 0x3C
  display.clearDisplay(); // очистка
  display.setTextColor(WHITE); // цвет текста

  display.display(); // Обновление экрана
  button1Up = new GButton(button1Pin);
  button2Down = new GButton(button2Pin);
  button3ok = new GButton(button3Pin);

  button1Up->setDebounce(20);        // настройка антидребезга (по умолчанию 80 мс)
  button1Up->setTimeout(500);       // настройка таймаута на удержание (по умолчанию 500 мс)
  button1Up->setClickTimeout(100);   // настройка таймаута между кликами (по умолчанию 300 мс)

  button2Down->setDebounce(20); 
  button2Down->setTimeout(500);
  button2Down->setClickTimeout(100);

  button3ok->setDebounce(50);
  button3ok->setTimeout(1000);
  button3ok->setClickTimeout(300);
  Serial.begin(9600);
  Gamepad.begin();
  home();
}

/*
* Основной цикл программы.
*
* Эта функция вызывается многократно.
* Она вызывает функцию tick() всех кнопок и проверяет наличие щелчков.
* Если щелчок обнаружен, он вызывает соответствующую функцию на основе текущего СТАТУСА.
* СТАТУС — это глобальная переменная, которая указывает, какую функцию следует вызвать.
* СТАТУС может быть изменен функциями, вызываемыми этим циклом.
*/
void loop() {
    button1Up->tick();
    button2Down->tick();
    button3ok->tick();
    if (button1Up->hasClicks() || button2Down->hasClicks() || button3ok->hasClicks() || update) {
        update = false;
        if (STATUS == 0) 
            home();
        else if (STATUS == 1)
            mainFunc();
        else if (STATUS == 2)
            callibrationFunc();
        else if (STATUS == 3)
            adjustmentFunc();
        else if(STATUS == 4)
            debugFunc();
        else if(STATUS == 5)
            saveFunc();
    }
    if (readflag) 
        read();
    
}


/**
 * @brief Функция считывает и отображает аналоговые входы для руля и педалей.
 *
 * Эта функция считывает текущий счетчик энкодера и аналоговые значения для
 * педалей газа, тормоза и сцепления. Она отображает эти значения в указанном диапазоне
 * и назначает их глобальным переменным. Если педали инвертированы,
 * она инвертирует их значения.
 *
 * Значение `wheel` отображается из счетчика энкодера с использованием текущей
 * настройки градуса колеса. Значения педалей корректируются на основе их
 * соответствующих минимальных и максимальных настроек.
 */
void read() {
    // считываем текущий счетчик энкодера
    int wheelValue = encCount;
    // отображаем его в указанном диапазоне
    wheel = map(wheelValue, wheelDec/4, -wheelDec/4, -32768, 32767);
    // считываем аналоговые значения для педалей
    int gasValue = analogRead(pinGas);
    int brakeValue = analogRead(pinBrake);
    int clutchValue = analogRead(pinClutch);
    // корректируем значения педалей на основе их настроек
    pedalGas = map(gasValue, gasMin - pedalAdjustmentGasMin, gasMax + pedalAdjustmentGasMax, -128, 127);
    pedalBrake = map(brakeValue, brakeMin - pedalAdjustmentBrakeMin, brakeMax + pedalAdjustmentBrakeMax, -128, 127);
    pedalClutch = map(clutchValue, clutchMin - pedalAdjustmentClutchMin, clutchMax + pedalAdjustmentClutchMax, -128, 127);
    // если педали инвертированы, инвертируем их значения
    if (INVERTEDPEDAL) {
        pedalGas = -pedalGas;
        pedalBrake = -pedalBrake;
        pedalClutch = -pedalClutch;
    }
    // отображаем полученные значения на Gamepad
    Gamepad.xAxis(wheel);
    Gamepad.rxAxis(pedalGas);
    Gamepad.ryAxis(pedalBrake);
    Gamepad.rzAxis(pedalClutch);
    // отправляем данные на Gamepad
    Gamepad.write();
}


/**
* @brief Главное меню устройства.
*
* Эта функция вызывается при первом включении устройства или когда пользователь
* нажимает кнопку «ОК» из любого другого меню.
*
* Функция отображает меню с пятью опциями: «ГЛАВНАЯ», «КАЛИБРОВКА»,
* «НАСТРОЙКА», «ОТЛАДКА» и «СОХРАНЕНИЕ». Пользователь может использовать кнопки вверх и вниз
* для навигации по меню и кнопку «ОК» для выбора опции.
*
* Функция также обрабатывает логику навигации по меню и выбора
* опции. Когда опция выбрана, функция вызывает соответствующую
* функцию: mainFunc(), callibrationFunc(), adjustmentFunc(), debugFunc() или
* saveFunc().
*
* Функция использует глобальную переменную STATUS для отслеживания текущего
* выбора меню.
*/
void home() {
    display.clearDisplay(); // Очистка дисплея перед отрисовкой нового меню

    // Массив строк меню
    const char* menuItems[] = {
        "  MAIN",         // Главная функция
        "  CALLIBRATION", // Калибровка устройства
        "  ADJUSTMENT",   // Настройка параметров
        "  DEBUG",        // Режим отладки
        "  SAVE"          // Сохранение данных
    };

    // Отрисовка меню
    for (int i = 0; i < 5; i++) {
        display.setCursor(0, i * 9); // Установка позиции курсора для каждой строки
        display.print(menuItems[i]); // Вывод текста меню
    }

    // Управление курсором
    if (button2Down->isSingle()) { // Если кнопка вниз нажата
        CursorPos = (CursorPos + 1) % 5; // Перемещение курсора вниз с циклическим переходом
    }
    if (button1Up->isSingle()) { // Если кнопка вверх нажата
        CursorPos = (CursorPos - 1 + 5) % 5; // Перемещение курсора вверх с циклическим переходом
    }

    // Действия по выбору
    if (button3ok->isSingle()) { // Если кнопка OK нажата
        STATUS = CursorPos + 1; // Установка статуса в соответствии с позицией курсора
        if (STATUS == 3) { // Специальный случай для НАСТРОЙКИ
            adjustmentStatus = -1; // Сброс статуса настройки
            CursorPos = 0; // Сброс позиции курсора
        }
    }

    // Вызов соответствующей функции на основе текущего статуса
    switch (STATUS) {
        case 1: mainFunc(); break;        // Вызов основной функции
        case 2: callibrationFunc(); break; // Вызов функции калибровки
        case 3: adjustmentFunc(); break;  // Вызов функции настройки
        case 4: debugFunc(); break;       // Вызов функции отладки
        case 5: saveFunc(); break;        // Вызов функции сохранения
        default: break;                   // Ничего не делать по умолчанию
    }

    drawCursor(); // Отрисовка курсора на текущей позиции
    display.display(); // Обновление дисплея для отображения изменений
}


// Рисуем курсор меню в позиции CursorPos
void drawCursor() {
  display.setCursor(0, CursorPos*9);
  display.print("> ");
}

//указатель выборва
void drawCursorSelect(int line) {
  display.setCursor(110, line*9);
  display.print("<=");   
}

void mainFunc() {
    display.clearDisplay();

    if (button3ok->isDouble()) {
        resetStatusAdjustment = false;
    }

    display.setCursor(0, 0);
    display.print("  GAS:        ");
    display.print(pedalGas);
    display.setCursor(0, 9);
    display.print("  BRAKE:      ");
    display.print(pedalBrake);
    display.setCursor(0, 18);
    display.print("  CLUTCH:     ");
    display.print(pedalClutch);
    display.setCursor(0, 27);
    display.print("  WHEEL:      ");
    display.print(constrain(wheel, wheelDec, -wheelDec));

    display.display();
}
void callibrationFunc() {
}

/**
*Функция регулировки
*
* Функция, которая управляет регулировкой деталей отделки короля
*
* @return void
*/
void adjustmentFunc() {
    display.clearDisplay();

    // Обработка состояния
    switch (adjustmentStatus) {
        case 0: { // Основной режим выбора
            handleCursorMovement(4); // Обработка перемещения курсора в пределах [0-3]

            if (button3ok->isSingle()) {
                adjustmentStatus = CursorPos + 1; // Переход к выбранному состоянию
            }
            if (button3ok->isDouble()) { // Выход
                resetToMainStatus();
                return;
            }
            break;
        }
        case 1: // GAS
            if (adjustmentLowStatus == 0)
                processPedalAdjustment(pedalAdjustmentGasMax);
            else
                processPedalAdjustment(pedalAdjustmentGasMin);
            break;
        case 2: // BRAKE
            if (adjustmentLowStatus == 0)
                processPedalAdjustment(pedalAdjustmentBrakeMax);
            else
                processPedalAdjustment(pedalAdjustmentBrakeMin);
            break;
        case 3: // CLUTCH
            if (adjustmentLowStatus == 0)
                processPedalAdjustment(pedalAdjustmentClutchMax);
            else
                processPedalAdjustment(pedalAdjustmentClutchMin);
            break;
        case 4: // WHEEL
            processWheelAdjustment();
            break;
        case 5: // Gamepad readflag
            readflagSwitch();
        default:
            adjustmentStatus = 0; // Безопасное состояние
            break;
    }
    

    // Возврат к основному режиму
    if ((adjustmentStatus == 1 || adjustmentStatus == 2 || adjustmentStatus == 3) && resetStatusAdjustment) {
        if (button3ok->isSingle()) {
            if (adjustmentLowStatus == 0){
                adjustmentLowStatus = 1;
            } else {
                adjustmentLowStatus = 0;
                adjustmentStatus = 0;
            }      
            resetStatusAdjustment = false;
        }
    }
    // Возврат к основному режиму
    if ((adjustmentStatus == 4 || adjustmentStatus == 5) && resetStatusAdjustment) {
        if (button3ok->isSingle()) {
            adjustmentLowStatus = 0;
            adjustmentStatus = 0;
            resetStatusAdjustment = false;
        }
    }

    // Отрисовка данных на экране
    displayMenuData();
    drawCursor();
    if (adjustmentStatus != 0) {
        drawCursorSelect(CursorPos);
    }

    display.display();
}

/**
*Обработка перемещения курсора
*
* Функция, которая изменяет текущий индекс курсора, обработавая события
* нажатия кнопок "Вниз" и "Вверх". Индекс курсора изменяется в пределах
* [0, maxIndex] циклически.
*
* @param maxIndex - максимальный индекс курсора
*/
void handleCursorMovement(int maxIndex) {
    if (button2Down->isSingle()) {
        CursorPos = (CursorPos + 1) % (maxIndex + 1); // Циклический переход
    }
    if (button1Up->isSingle()) {
        CursorPos = (CursorPos - 1 + (maxIndex + 1)) % (maxIndex + 1); // Циклический переход
    }
}

/**
* Функция для переключения флага readflag
*
* readflag - глобальный флаг, который отображает состояние
* возможности считывания данных с потенциометров.
* Если флаг установлен, то считывание данных
* происходит. Если флаг сброшен, то считывание
* данных не происходит.
*
* Функция readflagSwitch() переключает значение
* флага readflag, используя кнопку button3ok.
* Если кнопка button3ok была нажата, то
* значение флага readflag изменяется на противоположное.
*/
void readflagSwitch() {
    if (button3ok->isSingle()) {
        readflag = !readflag; // Переключение флага
    }
}

/** 
* @brief Сбрасывает состояние системы в основное состояние.
*
* Эта функция сбрасывает несколько глобальных переменных в их начальное состояние,
* устанавливая STATUS в 0, сбрасывая позицию курсора в 0 и отмечая
* статус настройки как -1. Она используется для возврата системы
* в ее основное состояние из любого другого состояния.
*/
void resetToMainStatus() {
    STATUS = 0;
    CursorPos = 0;
    adjustmentStatus = -1;
    update = true;

}

/**
* @brief Обработка настройки педали
*
* Функция, которая изменяет переданное значение настройки педали,
* используя функцию adjustmentFunc, и отмечает, что состояние
* настройки было изменено.
*
* @param pedalAdjustmentValue - значение настройки педали
*/
void processPedalAdjustment(int& pedalAdjustmentValue) {
    pedalAdjustmentValue = adjustmentFunc(pedalAdjustmentValue);
    resetStatusAdjustment = true;
}


/**
* @brief Обработка настройки колеса
*
* Функция, которая изменяет выбранное значение колеса,
* используя кнопки 1 и 2, и отмечает, что состояние
* настройки было изменено.
*/
void processWheelAdjustment() {
    if (button2Down->isSingle()) {
        currentWheelIndex = (currentWheelIndex + 1) % (sizeof(wheelValues) / sizeof(wheelValues[0]));
    }
    if (button1Up->isSingle()) {
        currentWheelIndex = (currentWheelIndex == 0)
                            ? (sizeof(wheelValues) / sizeof(wheelValues[0])) - 1
                            : currentWheelIndex - 1;
    }
    wheelDec = wheelValues[currentWheelIndex];
    resetStatusAdjustment = true;
}


/**
* @brief Вывод меню настройки
*
* Функция выводит на дисплей текущие значения настройки
* педалей газа, тормоза и сцепления, а также выбранное
* значение колеса.
*/
void displayMenuData() {
    display.setCursor(0, 0);
    display.print("  GAS:        ");
    display.print(pedalAdjustmentGasMax + " " + pedalAdjustmentGasMin);
    display.setCursor(0, 9);
    display.print("  BRAKE:      ");
    display.print(pedalAdjustmentBrakeMax + " " + pedalAdjustmentBrakeMin);
    display.setCursor(0, 18);
    display.print("  CLUTCH:     ");
    display.print(pedalAdjustmentClutchMax + " " + pedalAdjustmentClutchMin);
    display.setCursor(0, 27);
    display.print("  WHEEL:      ");
    display.print(wheelDec);
    display.setCursor(0, 36);
    display.print("  STATUS:     ");
    display.print(readflag ? "ON" : "OFF");
    display.display();
}

/**
* @brief Увеличить или уменьшить заданное значение с помощью кнопок
*
* Эта функция увеличит или уменьшит заданное значение на 1, если соответствующая кнопка нажата один раз,
* или на 5, если кнопка нажата дважды быстро.
*
* @param value Значение, которое нужно увеличить или уменьшить
* @return Скорректированное значение
*/
int adjustmentFunc(int value) {
  if (button2Down->isSingle())
      value--;
  if (button1Up->isSingle()) 
      value++;            
  if (button2Down->isDouble()) 
      value -= 10;            
  if (button1Up->isDouble()) 
      value += 10;          

  return value;
}

void debugFunc() {
  
}
void saveFunc() {

}