#include "../inc/Pedal.hpp"

Pedal::Pedal(int speed, int pin) {
    RH_ASK rf_driver(speed, pin);  
}

bool Pedal::init() {
    return rf_driver.init();
}

bool Pedal::lisening() {
    return rf_driver.recv(buf, &buflen);
}

uint8_t* Pedal::getMessage() {
    return buf;
}