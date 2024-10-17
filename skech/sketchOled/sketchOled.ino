#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;
#define status 5
void setup() {
  display.init();        // инициализация
  display.clear();       // очистка
  display.setScale(1);   // масштаб текста (1..4)
  display.home();        // курсор в 0,0
  
 
  if (status == 1){
      display.setCursor(0, 1);
      display.print("Main");
      display.setCursor(0, 3);
      display.print("Gas:           ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 4);
      display.print("Brake:         ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 5);
      display.print("Clutch:        ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 6);
      display.print("Wheel:         ");
      display.print(1000);
  }
  else if (status == 2) {
      display.clear(); 
      display.setCursorXY(5, 20);
      display.setScale(4); 
      display.print("DEBUG");
  } else if (status == 3) {
      display.clear();      
      display.setScale(1);   
      display.setCursor(0, 1);
      display.print("Debug");
      display.setCursor(0, 3);
      display.print("Gas:           ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 4);
      display.print("Brake:         ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 5);
      display.print("Clutch:        ");
      display.print(100);
      display.print("%");
      display.setCursor(0, 6);
      display.print("Wheel:         ");
      display.print(1000);
  } else if (status == 4) {
      display.clear(); 
      display.setCursorXY(18, 20);
      display.setScale(4); 
      display.print("SAVE");
  }  else if (status == 5) {
      display.clear(); 
      display.setCursorXY(18, 20);
      display.setScale(4); 
      display.print("MAIN");
  } 

}
void loop() {
  
}