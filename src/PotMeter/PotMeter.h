#ifndef PAIN
#define PAIN

#ifndef uint8_t
    #include <stdint.h>
#endif

class PotMeterClass
{
public:
    // Public functions & attributes.
    PotMeterClass();
    void setBacklightPinRaw(int input); // Set backlight on/off at full power.
    void setBacklightBrightness(uint8_t inputByte);
private:
    void setPWM(uint8_t inputByte,bool reset);
    /* data */
};

extern PotMeterClass PotMeter;
#endif
