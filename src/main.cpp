#include <Wire.h>
#include <HardwareSerial.h>

//#define ARDUINOMODE
// Uncomment line above to enable Arduino.h & disable main.

#ifdef ARDUINOMODE
#include <Arduino.h>
#endif

void setup() {
    Serial.begin(9600);
    Serial.flush();

    Wire.begin();

    Serial.println("Test 1.");
    Serial.flush();

    Wire.beginTransmission(0x52); // Begin transmission to nunchuk.
    Wire.write(0xF0);
    Wire.write(0x55);
    Wire.endTransmission(); // Actually send data.

    Serial.println("Succesfull transmission.");
    Serial.flush();

} // Init function.

void loop() {}; // Unused function for Arduino compiler.

#ifndef ARDUINOMODE
int main() {
    setup();
    return 0;
}
#endif
