#include <Wire.h>

// Address of the IO expander
#define IO_ADDRESS 0x21 // For non-A versions, 0x39 for A versions
#define MAX_DISPLAY_COUNT 16

// An array that holds the binary representation of numbers to be displayed
volatile uint8_t number_representations[MAX_DISPLAY_COUNT] = {
        0b11000000, // 0
        0b11111001, // 1
        0b10100100, // 2
        0b10110000, // 3
        0b10011001, // 4
        0b10010010, // 5
        0b10000010, // 6
        0b11111000, // 7
        0b10000000, // 8
        0b10010000, // 9
        0b10001000, // A
        0b10000011, // B
        0b10100111, // C
        0b10100001, // D
        0b10000110, // E
        0b10001110  // F
};

// Sends a byte to the IO expander over I2C
void send_byte_to_io_expander(uint8_t byte)
{
	Wire.beginTransmission(IO_ADDRESS);
	Wire.write(byte);
	Wire.endTransmission();
}

// Shows a value on the segment display by sending the corresponding binary representation over I2C
void segment_display_char(uint8_t value)
{
	send_byte_to_io_expander(number_representations[value % MAX_DISPLAY_COUNT]);
}
