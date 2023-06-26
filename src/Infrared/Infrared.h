#ifndef INFRARED_H
#define INFRARED_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

#define IR_RX_FREQUENCY 50
#define IR_TX_FREQUENCY 50


class InfraredClass {
    public:
        InfraredClass();
        void begin();
        void send_byte(uint8_t byte);
};


extern InfraredClass Infrared;

#endif
