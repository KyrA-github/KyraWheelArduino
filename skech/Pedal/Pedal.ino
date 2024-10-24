#include <VirtualWire.h>

void setup() {
    Serial.begin(9600);
    vw_set_rx_pin(9); // Установите пин приемника
    vw_setup(2000);    // Установите скорость передачи
    vw_rx_start();     // Начинаем прием
}

void loop() {
    uint8_t buf[6]; // Буфер для получаемого сообщения
    uint8_t buflen = sizeof(buf);

    if (vw_get_message(buf, &buflen)) { // Если получено сообщение
        Serial.print("Received: ");
        for (int i = 0; i < buflen; i++) {
            Serial.print((char)buf[i]); // Печатаем полученное сообщение
        }
        Serial.println();
    }
}
