#include "../inc/Pedal.hpp"

// Инициализация с передачей параметров скорости и пина
Pedal::Pedal(int speed, int pin) : speed(speed), pin(pin) {}

Pedal::~Pedal() {
    vw_rx_stop();  // Остановка приёмника VirtualWire
}

bool Pedal::init() {
    vw_set_rx_pin(pin);            // Устанавливаем пин для приёма
    vw_setup(speed);               // Устанавливаем скорость
    vw_rx_start();                 // Запуск приёмника
    return true;
}

bool Pedal::lisening() {
    return vw_get_message(reinterpret_cast<uint8_t*>(buf), &buflen);  // Получаем сообщение, если оно доступно
}

int8_t* Pedal::getMessage() {
    return buf;
}
