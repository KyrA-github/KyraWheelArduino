#include <VirtualWire.h>

#define transmitPin 12  // Пин для передачи данных
#define transmitSpeed 2000  // Скорость передачи в бит/с
#define PedalPin1 A1
#define PedalPin2 A2
#define PedalPin3 A5
// КПП пины
const int kppPin1 = 2, kppPin2 = 3, kppPin3 = 4, kppPin4 = 5, kppPin5 = 6, kppPin6 = 7;

void setup() {
    vw_set_tx_pin(transmitPin);  
    vw_setup(transmitSpeed);   
}

void loop() {
    int8_t Pedal1 = constrain(analogRead(PedalPin1), -128, 127);
    int8_t Pedal2 = constrain(analogRead(PedalPin2), -128, 127);
    int8_t Pedal3 = constrain(analogRead(PedalPin3), -128, 127);
    int8_t message[5] = {Pedal1, Pedal2, Pedal3, 0, 0};  

    vw_send(reinterpret_cast<uint8_t*>(message), sizeof(message)); 
    vw_wait_tx();  

    delay(10);    
}
