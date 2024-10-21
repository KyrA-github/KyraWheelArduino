#include "../inc/Pedal.hpp"

// Инициализация через динамическое выделение памяти
Pedal::Pedal(int speed, int pin) {
    rf_driver = new RH_ASK(speed, pin);  
}

Pedal::~Pedal() {
    delete rf_driver; 
}

bool Pedal::init() {
    return rf_driver->init();  
}

bool Pedal::lisening() {
    return rf_driver->recv(buf, &buflen);  
}

int8_t* Pedal::getMessage() {
    return buf;  
}
