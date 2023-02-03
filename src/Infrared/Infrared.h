#ifndef INFRARED_H
#define INFRARED_H

#include <stdint.h>

void init_ir();
void send_ir(uint16_t input_data); // main function
void send_start_bit();
void send_stop_bit();
void send_bit_next();

uint16_t receive_ir();
void receive_bit();
bool is_bit(uint16_t bit_length); // returns true if received bit equals given bit length

void ir_setup(int host);
// internal functions
void pin_init();
void timer0_init();
void timer2_init();
void timer2_clear();

#endif
