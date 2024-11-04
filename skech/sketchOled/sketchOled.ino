#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <GyverButton.h>
const int button1Pin = 15, button2Pin = 14, button3Pin = 16;
GButton* button1Up = nullptr;
GButton* button2Down = nullptr;
GButton* button3ok = nullptr;

int CursorPos = 0;

int wheelValues[] = {90, 180, 360, 540, 720, 900, 1080}; // Возможные значения wheelDec
int currentWheelIndex = 0; // Индекс текущего значения wheelDec
int wheelDec = 90;


int STATUS = 0;
int adjustmentStatus = 0;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with I2C address 0x3C
  display.clearDisplay(); // очистка
  display.setTextColor(WHITE); // цвет текста

  display.display(); // Обновление экрана
  button1Up = new GButton(button1Pin);
  button2Down = new GButton(button2Pin);
  button3ok = new GButton(button3Pin);

  button1Up->setDebounce(20);        // настройка антидребезга (по умолчанию 80 мс)
  button1Up->setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)
  button1Up->setClickTimeout(300);   // настройка таймаута между кликами (по умолчанию 300 мс)

  button2Down->setDebounce(50); 
  button2Down->setTimeout(1000);
  button2Down->setClickTimeout(600);

  button3ok->setDebounce(50);
  button3ok->setTimeout(1000);
  button3ok->setClickTimeout(600);
  home();
  Serial.begin(9600);
}

void loop() {
  button1Up->tick();
  button2Down->tick();
  button3ok->tick();
  if (button1Up->isSingle()) {
    Serial.println(1);
  }
  if (button2Down->isSingle()) {
    Serial.println(2);
  }
  if (button3ok->isSingle()) {
    Serial.println(3);
  }
  if (button1Up->hasClicks() || button2Down->hasClicks() || button3ok->hasClicks()) {
    if (STATUS == 0) {
      home();
    }else if (STATUS == 1){
      mainFunc();
    }else if (STATUS == 2){
      callibrationFunc();
    }else if (STATUS == 3){
      adjustmentFunc();
    }else if(STATUS == 4){
      debugFunc();
    }else if(STATUS == 5){
        saveFunc();
    }
  }
}

void home() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("  MAIN");
  display.setCursor(0, 9);
  display.print("  CALLIBRATION");
  display.setCursor(0, 18);
  display.print("  ADJUSTMENT");
  display.setCursor(0, 27);
  display.print("  DEBUG");
  display.setCursor(0, 36);
  display.print("  SAVE");
  display.setCursor(0, 45);
  if (button2Down->isSingle()) {
    CursorPos++;
    if (CursorPos > 4) 
      CursorPos = 0;
  }
  if (button1Up->isSingle()) {
    CursorPos--;
    if (CursorPos < 0) 
      CursorPos = 4;
  }
  if (button3ok->isSingle()) {
    switch (CursorPos) {
      case 0:
        STATUS = 1;
        break;
      case 1:
        STATUS = 2;
        break;
      case 2:
        STATUS = 3;
        adjustmentStatus = 0;
        CursorPos = 0;
        break;
      case 3:
        STATUS = 4;
        break;
      case 4:
        STATUS = 5;
        break;
      default:
        break;
    }
  }
  switch (STATUS) {
      case 1:
        mainFunc();
        break;
      case 2:
        callibrationFunc();
        break;
      case 3:
        adjustmentFunc();
        break;
      case 4:
        debugFunc();
        break;
      case 5:
        saveFunc();
        break;
      default:
        break;
    }
  drawCursor();
  display.display();
}

void drawCursor() {
  display.setCursor(0, CursorPos*9);
  display.print("> ");
}

void mainFunc() {
}
void callibrationFunc() {
}
void adjustmentFunc() {
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("  GAS:");
  display.setCursor(0, 9);
  display.print("  BRAKE:");
  display.setCursor(0, 18);
  display.print("  CLUTCH:");
  display.setCursor(0, 27);
  display.print("  WHEEL: ");
  display.print(wheelDec);
  
  switch (adjustmentStatus) {
        case 0:
          if (button2Down->isSingle()) {
            CursorPos++;
            if (CursorPos > 3) 
              CursorPos = 0;
          }
          if (button1Up->isSingle()) {
            CursorPos--;
            if (CursorPos < 0) 
              CursorPos = 3;
          }
          if (button3ok->isSingle()) {
            switch (CursorPos) {
              case 1:
                adjustmentStatus = 1;
                break;
              case 2:
                adjustmentStatus = 2;
                break;
              case 3:
                adjustmentStatus = 3;
                break;
              case 4:
                adjustmentStatus = 4;
                break;
              default:
                break;
            }
          }
          if (button3ok->isDouble()) {
            STATUS = 0;
            CursorPos = 0;
            return;
          }
          if (button2Down->isSingle()) {
              currentWheelIndex++;
              if (currentWheelIndex >= sizeof(wheelValues) / sizeof(wheelValues[0])) {
                currentWheelIndex = 0; // Вернуться к началу, если достигли конца
              }
              wheelDec = wheelValues[currentWheelIndex];
            }

          if (button1Up->isSingle()) {
            currentWheelIndex--;
            if (currentWheelIndex < 0) {
              currentWheelIndex = sizeof(wheelValues) / sizeof(wheelValues[0]) - 1; // Переход к последнему элементу
            }
            wheelDec = wheelValues[currentWheelIndex];
          }

          break;
        case 1:
            if (button3ok->isSingle()) {
              adjustmentStatus = 0;
            }
          
          break;
        case 2:
            if (button3ok->isSingle()) {
              adjustmentStatus = 0;
            }

          break;
        case 3:
            if (button3ok->isSingle()) {
              adjustmentStatus = 0;
            }
          break;
        case 4:
          
          
          if (button3ok->isSingle()) {
            adjustmentStatus = 0;
          }
          break;
        default:
          break;
      }
  drawCursor();
  display.display();

}
void debugFunc() {
  
}
void saveFunc() {

}