#include "Display.h"
#ifndef DDRB
#include <avr/io.h>
#endif

#include <util/delay.h>


DisplayClass::DisplayClass() {}


// Send startup commands to the display
void DisplayClass::start_display_startup_sequence() {
    this->send_command(DISPLAY_RESET_COMMAND);
    _delay_ms(150);

    uint8_t startup_commands[] = {
        // Exclusive to the Adafruit ILI
        DISPLAY_UNDOCUMENTED_COMMAND_1, 3, 0x03, 0x80, 0x02,
        DISPLAY_POWER_CONTROL_B_COMMAND, 3, 0x00, 0xC1, 0x30,
        DISPLAY_POWER_ON_SEQUENCE_CONTROL_COMMAND, 4, 0x64, 0x03, 0x12, 0x81,
        DISPLAY_DRIVER_TIMING_CONTROL_A_COMMAND, 3, 0x85, 0x00, 0x78,
        DISPLAY_POWER_CONTROL_A_COMMAND, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
        DISPLAY_PUMP_RATIO_CONTROL_COMMAND, 1, 0x20,
        DISPLAY_DRIVER_TIMING_CONTROL_B_COMMAND, 2, 0x00, 0x00,

        DISPLAY_POWER_CONTROL_1_COMMAND, 1, 0x23,                                                           // Power control VRH[5:0]
        DISPLAY_POWER_CONTROL_2_COMMAND, 1, 0x10,                                                           // Power control SAP[2:0];BT[3:0]
        DISPLAY_VCOM_CONTROL_1_COMMAND, 2, 0x3e, 0x28,                                                      // VCM control
        DISPLAY_VCOM_CONTROL_2_COMMAND, 1, 0x86,                                                            // VCM control2
        DISPLAY_MEMORY_ACCESS_CONTROL_COMMAND, 1, 0x48,                                                     // Memory Access Control
        DISPLAY_VERTICAL_SCROLLING_START_ADDRESS_COMMAND, 1, 0x00,                                          // Vertical scroll zero
        DISPLAY_COLMOD_PIXEL_FORMAT_SET_COMMAND, 1, 0x55,
        DISPLAY_FRAME_RATE_CONTROL_NORMAL_MODE_COMMAND, 2, 0x00, 0x18,
        DISPLAY_FUNCTION_CONTROL_COMMAND, 3, 0x08, 0x82, 0x27,                                              // Display Function Control
        DISPLAY_ENABLE_3G_COMMAND, 1, 0x00,                                                                 // 3Gamma Function Disable
        DISPLAY_GAMMA_SET_COMMAND, 1, 0x01,                                                                 // Gamma curve selected
        DISPLAY_POSITIVE_GAMMA_CORRECTION_COMMAND, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
        DISPLAY_NEGATIVE_GAMMA_CORRECTION_COMMAND, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
        DISPLAY_SLEEP_OUT_COMMAND, 0x80,                                                                                         // Exit Sleep
        DISPLAY_ON_COMMAND, 0x80,                                                                           // Display on
        DISPLAY_NOOP_COMMAND                                                                                // End of list
    };

    uint8_t cmd, x, numArgs;
    uint8_t *addr = startup_commands;
    while ((cmd = *(addr++)) > 0) {
        x = *(addr++);
        numArgs = x & 0x7F;
        this->send_command(cmd, addr, numArgs);
        addr += numArgs;
        if (x & 0x80) {
            _delay_ms(150);
        }
    }
}


// Initialize SPI configuration
void DisplayClass::spi_init() {
    DDRB |= (
        (1 << DDB2) // Set SPI CS (Chip Select) to output mode
        | (1 << DDB3) // Set SPI MOSI (Master Out Slave In) to output
        | (1 << DDB5) // Set SPI SCK (Source Clock) to output
    );

    DDRB &= ~(1 << DDB4); // Set SPI MISO (Master In Slave Out) to input

    SPCR = (
        (1 << SPE) // SPI Enable
        | (1 << MSTR) // Set SPI Master mode
    ); // Enable SPI and set self as master

    SPCR &= ~(
        (1 << DORD) // Set SPI MSB
        | (1 << CPOL) // Set SPI leading clock edge to rising
        | (1 << CPHA) // Sample SPI data on leading edge of the clock
        | (1 << SPR1) // Spe
        | (1 << SPR0) //    ed
    ); // Set SPI bit order to MSB, sample data on leading clock edge and set the clock to 'fosc/4'.
    SPSR |= (1 << SPI2X); // Double SPI speed

    PORTB &= ~(1 << DDB2); // Set SPI CS (Chip Select) to active LOW
}


// Initialize SPI transfer by setting SPI CS (Chip Select) to active LOW
inline void DisplayClass::spi_begin() {
    PORTB &= ~(1 << DDB2); // Set SPI CS (Chip Select) to active LOW
}


// Finalize SPI transfer by setting SPI CS (Chip Select) to active HIGH
inline void DisplayClass::spi_end() {
    PORTB |= (1 << DDB2); // Set SPI CS (Chip Select) to active HIGH
}


// Transfer one byte over SPI
uint8_t DisplayClass::spi_transfer(uint8_t data) {
    SPDR = data; // Get transfer'd

    asm volatile("nop"); // Performing a nop speeds up the wait loop by about 10%... somehow.
    while(!(SPSR & (1 << SPIF))); // Hold program until SPI has been sent.

    return SPDR;
}


// Initialize the registers needed for the display to function
inline void DisplayClass::init_registers() {
    DDRB |= (1 << DDB1); // Set DC to output mode
}


// Initialize display functionality
void DisplayClass::begin() {
    this->is_inverted = false;
    this->spi_init();
    this->init_registers();
    this->start_display_startup_sequence();
}


// Send 8bit Display SPI command with no arguments
void DisplayClass::send_command(uint8_t command, bool end_spi_after_command = true) {
    this->spi_begin();
    PORTB &= ~(1 << PORTB1); // Set DC LOW
    this->spi_transfer(command);
    PORTB |= (1 << PORTB1); // Set DC HIGH
    if (end_spi_after_command) {
        this->spi_end();
    }
}


// Send 8bit Display SPI command with 8bit arguments
void DisplayClass::send_command(uint8_t command, uint8_t* args, uint32_t args_len) {
    this->spi_begin();
    PORTB &= ~(1 << PORTB1); // Set DC LOW
    this->spi_transfer(command);

    PORTB |= (1 << PORTB1); // Set DC HIGH
    for (uint32_t i = 0; i < args_len; i++) {
        this->spi_transfer(args[i]);
    }
    this->spi_end();
}


// Sets the address window for the pixels to be written to
void DisplayClass::set_address_window(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
    uint8_t params[DISPLAY_ADDRESS_PARAM_SIZE] = {
        (uint8_t)((y_start & 0xFF00) >> 8),
        (uint8_t)((y_start & 0x00FF)),
        (uint8_t)((y_end & 0xFF00) >> 8),
        (uint8_t)((y_end & 0x00FF)),
    };
    this->send_command(DISPLAY_COLUMN_ADDRESS_SET_COMMAND, params, DISPLAY_ADDRESS_PARAM_SIZE);

    params[0] = (x_start & 0xFF00) >> 8;
    params[1] = (x_start & 0x00FF);
    params[2] = (x_end & 0xFF00) >> 8;
    params[3] = (x_end & 0x00FF);
    this->send_command(DISPLAY_PAGE_ADDRESS_SET_COMMAND, params, DISPLAY_ADDRESS_PARAM_SIZE);
}


// Fill the entire screen with a solid color
void DisplayClass::fill_screen(uint16_t color) {
    this->draw_rectangle(0, 0, DISPLAY_ROW_PIXEL_AMOUNT, DISPLAY_COLUMN_PIXEL_AMOUNT, color);
}


// Transfer pixel color over SPI
inline void DisplayClass::transfer_pixel_color_spi(uint16_t color) {
    /*
    A color takes up 16 bits, but the SPI only transfers 8 bits at a time
    So we split the 16-bit color up in to two 8 bit transfers
    */
    this->spi_transfer((color & 0xFF00) >> 8);
    this->spi_transfer(color & 0x00FF);
}


// Set the color of a given pixel
void DisplayClass::draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    this->set_address_window(x, y, x, y);

    this->send_command(DISPLAY_MEMORY_WRITE_COMMAND, false);
    this->transfer_pixel_color_spi(color);
    this->spi_end();
}


// Draw a rectangle in the given boundaries and color it the given color
void DisplayClass::draw_rectangle(
    uint16_t x_start,
    uint16_t y_start,
    uint16_t x_end,
    uint16_t y_end,
    uint16_t color
) {
    this->set_address_window(x_start, y_start, x_end, y_end);

    this->send_command(DISPLAY_MEMORY_WRITE_COMMAND, false);
    for (uint16_t x = x_start; x <= x_end; x += 1) {
        for (uint16_t y = y_start; y <= y_end; y += 1) {
            this->transfer_pixel_color_spi(color);
        }
    }
    this->spi_end();
}


void DisplayClass::draw_border(
    uint16_t x_start,
    uint16_t y_start,
    uint16_t x_end,
    uint16_t y_end,
    uint16_t thickness,
    bool corner_rounding,
    uint16_t color
) {
    this->draw_rectangle(x_start + (thickness * corner_rounding), y_start, x_end - (thickness * corner_rounding), y_start + thickness - 1, color);
    this->draw_rectangle(x_start, y_start + (thickness * corner_rounding), x_start + thickness - 1, y_end - (thickness * corner_rounding), color);
    this->draw_rectangle(x_start + (thickness * corner_rounding), y_end - thickness + 1, x_end - (thickness * corner_rounding), y_end, color);
    this->draw_rectangle(x_end - thickness + 1, y_start + (thickness * corner_rounding), x_end, y_end - (thickness * corner_rounding), color);
}


// Draw a circle with a given radius and color. The given coordinates are the center of the circle
void DisplayClass::draw_circle(
    uint16_t x,
    uint16_t y,
    uint16_t radius,
    uint16_t color
) {
    uint16_t diameter = radius * 2;
    uint16_t line_length_half;
    uint16_t current_x;
    for (uint8_t i = 0; i < diameter; i++) {
        line_length_half = (sqrt(pow((float)diameter, 2.0) - pow((float)i - (float)diameter, 2.0))) / 2.0;

        current_x = (x - radius + (i / 2)) + 1;
        this->draw_rectangle(
            current_x,
            y - line_length_half,
            current_x,
            y + line_length_half,
            color
        );

        current_x = (y + radius - (i / 2)) - 1;
        this->draw_rectangle(
            current_x,
            y - line_length_half,
            current_x,
            y + line_length_half,
            color
        );
    }
}


// Draw a sprite array
void DisplayClass::draw_sprite(
    const uint8_t* sprite,
    const uint16_t* colors,
    uint16_t x,
    uint16_t y,
    uint8_t scale = 1
) {
    // The maximum values (exclusive) of the x and y positions of the sprite
    // const uint16_t x_end = x + sprite[0];  // Sprite[0] == Sprite width
    // const uint16_t y_end = y + sprite[1];  // Sprite[1] == Sprite height

    /*
    This value is here to track the current index of the sprite array.
    It will be incremented after every odd sprite pixel iteration
    */
    uint16_t sprite_array_index = 2;

    // This value will check if the current pixel index is even
    bool pixel_index_is_even = true;

    for (uint16_t relative_x = 0; relative_x < sprite[0]; relative_x += 1) {
        for (uint16_t relative_y = 0; relative_y < sprite[1]; relative_y += 1) {
            /*
            The sprite array consists of two 4 bit color indexes per 8 bit array entry
            For on every even pixel, we get the SPRITE_INDEX_FIRST_PIXEL_MASK bits of the sprite array index.
            And on every odd pixel, we get the SPRITE_INDEX_SECOND_PIXEL_MASK bits of the
            sprite array index and increment sprite_array_index to go to the next array entry.
            */
            uint8_t color_index;
            if (pixel_index_is_even) {
                color_index = (sprite[sprite_array_index] & SPRITE_INDEX_FIRST_PIXEL_MASK) >> SPRITE_INDEX_FIRST_PIXEL_SHIFT;
            } else {
                color_index = sprite[sprite_array_index] & SPRITE_INDEX_SECOND_PIXEL_MASK >> SPRITE_INDEX_SECOND_PIXEL_SHIFT;
                sprite_array_index += 1;
            }

            // If the SPRITE_TRANSPARENT_COLOR is found, skip drawing on this pixel
            if (colors[color_index] != SPRITE_TRANSPARENT_COLOR) {
                // this->draw_pixel(current_x, current_y, colors[color_index]);
                this->draw_rectangle(
                    x + (relative_x * scale),
                    y + (relative_y * scale),
                    x + (relative_x * scale) + scale - 1,
                    y + (relative_y * scale) + scale - 1,
                    colors[color_index]
                );
            }

            // We finished drawing the pixel, so continue to the next one
            pixel_index_is_even = !pixel_index_is_even;
        }
    }
}


void DisplayClass::draw_text(
    const char* text,
    uint16_t x,
    uint16_t y,
    const uint8_t** font,
    uint8_t scale = 1,
    uint16_t color = TEXT_DEFAULT_COLOR
) {
    const uint16_t sprite_colors[] = {
        SPRITE_TRANSPARENT_COLOR,
        color,
    };
    uint16_t cursor_x = x;
    uint16_t cursor_y = y;
    for (uint8_t i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            cursor_x = x;
            cursor_y = cursor_y - font[1][1] * scale - 2 * scale;
        } else {
            const uint8_t* current_sprite;
            if (text[i] >= 'a' && text[i] <= 'z') {
                current_sprite = font[(text[i]-32) - ' '];
            } else {
                current_sprite = font[text[i] - ' '];
            }
            draw_sprite(
                current_sprite,
                sprite_colors,
                cursor_x,
                cursor_y - ((current_sprite[1] - font[1][1]) * scale),
                scale
            );
            cursor_x = cursor_x + (current_sprite[0] * scale) + scale;
        }
    }
}

void DisplayClass::draw_centered_text(
    const char* text,
    uint16_t y,
    const uint8_t** font,
    uint8_t scale = 1,
    uint16_t color = TEXT_DEFAULT_COLOR
) {
    uint16_t total_width = 0;
    uint16_t line_width = 0;
    for (uint8_t i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            line_width = 0;
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            line_width = line_width + (font[(text[i]-32) - ' '][0] * scale) + scale;
        } else {
            line_width = line_width + (font[text[i] - ' '][0] * scale) + scale;
        }
        if (line_width > total_width) {
            total_width = line_width;
        }
    }
    this->draw_text(text, floor(DISPLAY_ROW_PIXEL_AMOUNT/2) - floor((line_width-scale)/2), y, font, scale, color);
}


// Invert all colors that are on the display right now
void DisplayClass::invert_colors() {
    if (this->is_inverted) {
        this->send_command(DISPLAY_INVERSION_OFF_COMMAND);
        this->is_inverted = false;
    } else {
        this->send_command(DISPLAY_INVERSION_ON_COMMAND);
        this->is_inverted = true;
    }
}


DisplayClass Display = DisplayClass();
