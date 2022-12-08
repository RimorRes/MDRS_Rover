/*Serial communication between Arduino and Raspberry Pi. */
#include<Arduino.h>

String serialPull(int TIMEOUT = 1000); // Pulls/Reads incoming data from serial port. It does NOT read the instrution. 

void serialPush(String TXT); // Send bytes from serial port. Simplification of Serial.Write().

void lectureBus();

void parlerBus();
