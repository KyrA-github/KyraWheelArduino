#ifndef PEDAL_HPP
#define PEDAL_HPP

#include <VirtualWire.h>

class Pedal
{
public:
    Pedal(int speed, int pin);
    ~Pedal();

    bool init();
    bool lisening();
    int8_t* getMessage();

private:
    int8_t buf[5];  // Буфер для данных
    uint8_t buflen = sizeof(buf);
    int pin;        // Пин для VirtualWire
    int speed;      // Скорость передачи
};

#endif // PEDAL_HPP
