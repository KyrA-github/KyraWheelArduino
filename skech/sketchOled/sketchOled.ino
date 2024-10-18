#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define status 1 // Здесь измените статус для тестирования

void drawConnectionIndicator(bool connected) {
    // Удалите предыдущие символы
    display.fillRect(SCREEN_WIDTH - 16, 0, 16, 16, BLACK); // Очищаем область для индикатора

    if (connected) {
        // Вертикальные полоски связи
        display.drawLine(SCREEN_WIDTH - 12, 0, SCREEN_WIDTH - 12, 6, WHITE); // Первая полоска
        display.drawLine(SCREEN_WIDTH - 8, 2, SCREEN_WIDTH - 8, 6, WHITE);   // Вторая полоска
        display.drawLine(SCREEN_WIDTH - 4, 4, SCREEN_WIDTH - 4, 6, WHITE);   // Третья полоска
    } else {
        display.drawLine(SCREEN_WIDTH - 12, 0, SCREEN_WIDTH - 12, 6, WHITE); // Первая полоска
        display.drawLine(SCREEN_WIDTH - 8, 2, SCREEN_WIDTH - 8, 6, WHITE);   // Вторая полоска
        display.drawLine(SCREEN_WIDTH - 4, 4, SCREEN_WIDTH - 4, 6, WHITE);   // Третья полоска
        // Крестик при отсутствии связи
        display.drawLine(SCREEN_WIDTH - 3, 0, SCREEN_WIDTH - 1, 2, WHITE);
        display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 3, 2, WHITE);
    }
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with I2C address 0x3C
  display.clearDisplay(); // очистка
  display.setTextColor(WHITE); // цвет текста
  drawConnectionIndicator(false); // Отобразить полоски

  // Отображение информации в зависимости от статуса
  if (status == 1) {
    display.setTextSize(1); // Масштаб текста 1
    display.setCursor(0, 0);
    display.print("MAIN");
    

    display.setCursor(0, 10);
    display.print("Gas:           ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 20);
    display.print("Brake:         ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 30);
    display.print("Clutch:        ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 40);
    display.print("Wheel:         ");
    display.print(1000);
  } else if (status == 2) {
    display.clearDisplay(); 
    display.setTextSize(4); // Масштаб текста 4
    display.setCursor(5, 20);
    display.print("DEBUG");
  } else if (status == 3) {
    display.clearDisplay();      
    display.setTextSize(1);   
    display.setCursor(0, 0);
    display.print("Debug");

    display.setCursor(0, 10);
    display.print("Gas:           ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 20);
    display.print("Brake:         ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 30);
    display.print("Clutch:        ");
    display.print(100);
    display.print("%");

    display.setCursor(0, 40);
    display.print("Wheel:         ");
    display.print(1000);
  } else if (status == 4) {
    display.clearDisplay(); 
    display.setTextSize(4); // Масштаб текста 4
    display.setCursor(18, 20);
    display.print("SAVE");
  } else if (status == 5) {
    display.clearDisplay(); 
    display.setTextSize(4); // Масштаб текста 4
    display.setCursor(18, 20);
    display.print("MAIN");
  }
  display.setTextSize(1); // Масштаб текста 4
  display.setCursor(0, 0);
  display.print("         ");

  display.display(); // Обновление экрана
}

void loop() {
  // основной код здесь
}
