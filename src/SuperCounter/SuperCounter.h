#ifndef SUPERCOUNTER_H
#define SUPERCOUNTER_H

#include <HardwareSerial.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define MILLISECONDS_IN_SECOND 1000
#define TIMER0_COMPA_VAL 249


extern volatile uint32_t supercounter;
void init_supercounter();

#endif
