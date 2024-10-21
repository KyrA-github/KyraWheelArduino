#ifndef PEDAL_HPP
#define PEDAL_HPP

#include <RH_ASK.h>
#include <SPI.h> 

class Pedal
{
    public:
        Pedal(int speed, int pin);
        ~Pedal();

        bool init();
        bool lisening();
        int8_t* getMessage();

    private:
        int8_t buf[5]; // Буфер для данных
        int8_t buflen = sizeof(buf);
        RH_ASK* rf_driver;
};

#endif // PEDAL_HPP