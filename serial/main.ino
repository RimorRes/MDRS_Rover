#include "serial.h"

void setup(){
    Serial.begin(9600);
    initPorts();
}

void loop(){
  serialPull();

  
    //while(0){
      //  Run(serialPull());
    //}
    Serial.println("fin boucle");
}

void initPorts(){
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
}