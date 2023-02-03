#include "Infrared.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BIT0 8 // 256 microseconds
#define BIT1 16
#define BIT_STOP 24
#define BIT_START 32

#define TIME_RECEIVE_DIFFERENCE 2

#define TIMER1_COMP_VAL 7; // 32 microseconds

volatile uint16_t data = 0;
volatile bool is_sending = false;
volatile int bit = 0;
volatile int8_t bit_duration = -1;

volatile int16_t time_receive = -1;
volatile bool done_receiving = false;
volatile bool received_start_bit = false;
volatile uint16_t receive_data = 0;
volatile uint16_t bit_location = 0;

volatile uint32_t ir_time = 0; // find a way to count this only when actually receiving or sending IR


bool ir_frequency = 0; // IR receiver frequency

bool is_bit(uint16_t bit_length) {
    return (time_receive > ( bit_length-TIME_RECEIVE_DIFFERENCE ) && time_receive < (bit_length+TIME_RECEIVE_DIFFERENCE));
}

static void start_ir() {
    // set timer and variables
    bit = 0;

    //TCCR0B |= (1 << CS01) | (1 << CS00); // timer 0 prescaler 64
    is_sending = true;
}

void send_start_bit() {

    TCCR2A ^= (1 << COM2B1); // trigger pulse on 50% of duty cycle

    //OCR0A = BIT_START;
    bit_duration = BIT_START;

}

void send_bit_next() {
    bit++;
    TCCR2A ^= (1 << COM2B1); // flip output
    if (data) {
        // OCR0A = (data & 1) ? BIT1 : BIT0; // there is a clue someewhere here
        bit_duration = (data & 1) ? BIT1 : BIT0;
    // right, it gets the value of BIT1 if yeh yeh eyh euyheyeh ehyh seyahye fhAW FHAWEYHYWEHFOWEHWOUH Fpouheofuhfh!!!!!!!!!!!!!!!!!!!!!111
    } else {
        if ((bit % 2) == 1 && OCR0A == BIT_STOP) { // has stop bit been sent
            //TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // disable timer
            bit_duration = -1;

            data = 0;
            is_sending = false;
        } else {
            //OCR0A = BIT_STOP; // send stop bit
            bit_duration = BIT_STOP;
        }
    }
    data = (data >> 1);
}

void send_ir(uint16_t input_data) {
    start_ir();
    data = input_data;
    send_start_bit();
}

void receive_bit() {
    if (is_bit(BIT_START)) {
        received_start_bit = true;
        receive_data = 0;
    }

    if (received_start_bit) {
        if (is_bit(BIT_STOP)) {
            done_receiving = true;
            time_receive = -1;
            received_start_bit = false;
        } else if (is_bit(BIT1)) {
            receive_data |= (1 << bit_location);
            bit_location++;
        } else if (is_bit(BIT0)) {
            bit_location++;
        }
    }
}

uint16_t receive_ir() {
    if (done_receiving) {
        done_receiving = false;
        bit_location = 0;
        uint16_t ir_data = receive_data;
        receive_data = 0;
        return ir_data;
    }
    return 0;
}

void pin_init() {
    DDRD &= ~(1 << PORTD2); // set PD2 to input
    PORTD |= (1 << PORTD2); // enable pull-up

    DDRD |= (1 << PORTD3); // set PD3 to output

    EIMSK |= (1 << INT0);                   // enable INT0 interrupt
    EICRA |= (1 << ISC00) & (~(1 << ISC01)); // set interrupt on any change for receiving IR
}

/*void timer0_init() { // remove
    // clear registers
    TCCR0A = 0;
    TCCR0B = 0;

    TCCR0A |= (1 << WGM01); // clear timer on compare match

    TIMSK0 |= (1 << OCIE0A); // enable compare interrupt

    TCNT0 = 0; // set timer to 0
}*/

void timer1_init() {
    // currently used to measure time of bit
    /*TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B |= (1 << CS11) | (1 << CS10);

    TIMSK1 |= (1 << TOIE1); // oveflow interrupt

    TCNT1 = 0;*/

    // will now become counter

    // Set the prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // Set wave generation mode to CTC (mode 2)
    TCCR1B |= (1 << WGM12);
    TCCR1A &= ~((1 << WGM11) | (1 << WGM10));

    // Set the output compare register value
    OCR1A = 8; // 32 microseconds

    // Enable Output Compare A Match Interrupt
    TIMSK1 |= (1 << OCIE1A);
}

void timer2_init() { // IR pulses
    TCCR2A = 0;
    TCCR2B = 0;

    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS21);

    OCR2A = ir_frequency;
    OCR2B = (ir_frequency / 2);

    TCNT2 = 0;
}

void timer2_clear() {
    TCCR2A = 0;
    TCCR2B = 0;
    OCR2A = 0;
    OCR2B = 0;
    TIMSK2 = 0;
}

void ir_setup(int client) {
    // divide clock frequency (16MHz) by the multiplication of the prescaler (8) and the receiver frequency
    if (client == 1) {
        ir_frequency = 53; // 38kHz
    } else {
        ir_frequency = 36; // 56kHz
    }
    pin_init();
    timer1_init();
    timer2_init();
}

/*ISR(TIMER0_COMPA_vect) {
    send_bit();
}*/

/*ISR(TIMER1_OVF_vect)
{
    // clear timer 1
    TCCR1A = 0;
    TCCR1B = 0;

    time_receive = BIT_STOP + 1;
}*/

ISR(TIMER1_COMPA_vect)
{
    if (time_receive >= 0) {
        time_receive++;
    }
    if (bit_duration >= 0) {
        bit_duration--;
        if (bit_duration == 0) {
            send_bit_next(); // wow custom interrupt in an interrupt
        }
    }
}

ISR(INT0_vect)
{
    if (!done_receiving)
    {
        //time_receive = TCNT1; // receive time as measured by timer
        // ja dat werkt dus niet kut
        // oke dus dit moet anders
        // dat kan wel ja
        // moet ik ff denken hoe maar kan wel ja
        //timer1_init(); // start measuring time
        // measuring time can be done with clock, u dont need timer smh
        // except clock is too slow D:
        time_receive = 0; // new init
        receive_bit(); // process bit
    }
}
