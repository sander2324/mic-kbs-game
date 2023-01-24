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
    void checkPotmeterBrightness();
    void prepareADC();
    uint16_t rawADC;
    bool adcReady;
private:
    void setPWM(uint8_t inputByte,bool reset);
    /* data */
    uint8_t currentBrightness;
};

extern PotMeterClass PotMeter;
#endif
