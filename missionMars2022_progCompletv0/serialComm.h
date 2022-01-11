/*Serial communication between Arduino and Raspberry Pi. */
#include<Arduino.h>

char* serialPull(int TIMEOUT = 1000); // Pulls/Reads incoming data from serial port. It does NOT read the instrution. 

void serialPush(const char* TXT); // Send bytes from serial port. Simplification of Serial.Write().

void noel(); // for fun

void Run(char* INSTRUCTION); // Reads the instruction to call it after.
