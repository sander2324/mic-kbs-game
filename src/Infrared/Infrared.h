#ifndef INFRARED_H
#define INFRARED_H

#include <stdint.h>

class Infrared {
    public:
        Infrared(uint8_t rxfreq, uint8_t txfreq);
    private:
        uint8_t rxfreq;
        uint8_t txfreq;
};

#endif
