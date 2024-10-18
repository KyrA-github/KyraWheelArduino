#ifndef PEDAL_HPP
#define PEAL_HPP

#include <RH_ASK.h>
#include <SPI.h> 

class Pedal
{
    public:
        Pedal();

        bool init();
        bool lisening();
        uint8_t* getMessage();

    private:
        uint8_t buf[5]; // Буфер для данных
        uint8_t buflen = sizeof(buf);
        RH_ASK rf_driver;
};

#endif // PEDAL_HPP