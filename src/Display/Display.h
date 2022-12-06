#ifndef DISPLAY_H
#define DISPLAY_H
#ifndef PRId8 // Test if inttypes has been included somewhere else.
#include <inttypes.h>
#endif

class Display {
    public:
        Display();
        void Display_Init();
        void Display_ShowSquare();

    private:
        int SPI_setup();
        uint8_t SPI_send(uint8_t data);
};

#endif
