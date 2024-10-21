#include "../inc/display.hpp"

Display::Display(int SCREEN_W, int SCREEN_H) : SCREEN_WIDTH(SCREEN_W), SCREEN_HEIGHT(SCREEN_H), display(SCREEN_W, SCREEN_H, &Wire, -1) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.clearDisplay();   // Инициализация дисплея с черной вкладкой
    display.display();  // Обновляем дисплей для отображения пустого экрана
    lastTimeSave = 0;
}

void Display::clear() {
    display.clearDisplay();
}

void Display::update() {
    display.display();
}

void Display::setConnectionIndicator(bool connected) {
    display.fillRect(SCREEN_WIDTH - 16, 0, 16, 16, BLACK);
    display.drawLine(SCREEN_WIDTH - 12, 0, SCREEN_WIDTH - 12, 6, WHITE); 
    display.drawLine(SCREEN_WIDTH - 8, 2, SCREEN_WIDTH - 8, 6, WHITE);   
    display.drawLine(SCREEN_WIDTH - 4, 4, SCREEN_WIDTH - 4, 6, WHITE);   
    if (!connected) {
        display.drawLine(SCREEN_WIDTH - 3, 0, SCREEN_WIDTH - 1, 2, WHITE);
        display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 3, 2, WHITE);
    } 
}

void Display::setScene1(int gas, int brake, int clutch, int wheel, int HShifter, int gear) {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(F("MAIN"));
    
    display.setCursor(0, 10);
    display.print("Gas:           ");
    display.print(gas);
    display.print("%");

    display.setCursor(0, 20);
    display.print("Brake:         ");
    display.print(brake);
    display.print("%");

    display.setCursor(0, 30);
    display.print("Clutch:        ");
    display.print(clutch);
    display.print("%");

    display.setCursor(0, 40);
    display.print("Wheel:         ");
    display.print(wheel); 
}

void Display::setScene2(int gas, int brake, int clutch, int wheel, int HShifter, int gear) {     
    display.setTextSize(1);   
    display.setCursor(0, 0);
    display.print("COLIBRATION");

    display.setCursor(0, 10);
    display.print("Gas:           ");
    display.print(gas);

    display.setCursor(0, 20);
    display.print("Brake:         ");
    display.print(brake);

    display.setCursor(0, 30);
    display.print("Clutch:        ");
    display.print(clutch);

    display.setCursor(0, 40);
    display.print("Wheel:         ");
    display.print(wheel);
}

void Display::setScene3(int pedalMode, int wheelMode) {
    display.setTextSize(1);   
    display.setCursor(0, 0);
    display.print("SET MODE");


    display.setTextSize(2); 
    display.setCursor(0, 20);
    display.print("Mode Pedal 50%:          ");
    if (pedalMode == 0) {
        display.print("OFF");
    } else {
        display.print("ON");
    }

    display.setCursor(0, 40);
    display.print("Mode Wheel 50%:          ");
    if (wheelMode == 0) {
        display.print("OFF");
    } else {
        display.print("ON");
    }
}

void Display::drawSave(bool show) {
    if (show) {
        saveShow = true;
        display.setTextSize(1);
        display.setCursor(5,5);
        display.print("SAVE");
    } else {
        saveShow = false;
        display.setTextSize(1);
        display.setCursor(5,5);
        display.print("   ");
    }
}

void Display::updateLogic() {
    if (saveShow) {
        if (millis() - lastTimeSave > 1000) {
            lastTimeSave = millis();
            drawSave(false);
        }
    }
}
 


// void Display::drawSteeringWheel(int angle) {
//     display.clearDisplay();

//     // Рисуем руль
//     display.drawCircle(CENTER_X, CENTER_Y, RADIUS, WHITE); // Внешний круг
//     display.drawCircle(CENTER_X, CENTER_Y, RADIUS - 4, WHITE); // Внутренний круг (обод)

//     // Рисуем спицы руля
//     display.drawLine(CENTER_X, CENTER_Y, CENTER_X + RADIUS * cos(angle * DEG_TO_RAD), CENTER_Y + RADIUS * sin(angle * DEG_TO_RAD), WHITE);
//     display.drawLine(CENTER_X, CENTER_Y, CENTER_X + RADIUS * cos((angle + 120) * DEG_TO_RAD), CENTER_Y + RADIUS * sin((angle + 120) * DEG_TO_RAD), WHITE);
//     display.drawLine(CENTER_X, CENTER_Y, CENTER_X + RADIUS * cos((angle + 240) * DEG_TO_RAD), CENTER_Y + RADIUS * sin((angle + 240) * DEG_TO_RAD), WHITE);

//     display.display(); // Обновление экрана
// }

// void Display::rotateWheel(int angle) {
//     // Вращаем руль в одну сторону
//     for (angle = 0; angle < 360; angle += 5) {
//         drawSteeringWheel(angle);
//         delay(50); // Задержка для анимации
//     }
    
//     // Вращаем руль в другую сторону
//     for (angle = 360; angle > 0; angle -= 5) {
//         drawSteeringWheel(angle);
//         delay(50); // Задержка для анимации
//     }
// }