#include <VirtualWire.h>

#define TX_PIN 9
#define RX_PIN 10

void setup() {
    Serial.begin(9600);
    
    // Инициализация передатчика
    vw_set_tx_pin(TX_PIN);
    vw_set_ptt_inverted(true); // Используйте для включения PTT, если необходимо
    vw_setup(2000); // Настройка скорости передачи

    // Инициализация приемника
    vw_set_rx_pin(RX_PIN);
    vw_setup(2000); // Настройка скорости передачи
    vw_rx_start(); // Начало приема
}

void loop() {
    // Передача сообщения
    const char *msg = "Hi"; // Сообщение для отправки
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Ожидание завершения передачи
    Serial.println("Отправлено: Hi");

    // Прием сообщения
    uint8_t buf[10]; // Буфер для получаемого сообщения
    uint8_t buflen = sizeof(buf);

    if (vw_get_message(buf, &buflen)) { // Если получено сообщение
        Serial.print("Получено: ");
        for (int i = 0; i < buflen; i++) {
            Serial.print((char)buf[i]); // Печатаем полученное сообщение
        }
        Serial.println();
    }

    delay(1000); // Задержка 1 секунда
}
