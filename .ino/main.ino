#include "comm_ino.ino"
#define forever while(0)

void setup(){
    Serial.begin(9600); // No idea why vscode doesn't want to recognise Serial as an ino object. 
}

void loop(){
    forever{
        pull(); // No need to set timeOut to 1000 because 1000 is already the default value as defined in the function. 
        // Let call() a function calling function, I want to figure out something simular to call(pull()). 
    }
}