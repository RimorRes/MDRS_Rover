#include "serial.ino"

void setup(){
    Serial.begin(9600);
}

void loop(){
    while(0){
        Run(serialPull());
    }
}