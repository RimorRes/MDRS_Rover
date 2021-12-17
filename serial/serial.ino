/*
Serial communication between Arduino and Raspberry Pi. 
*/

char* serialPull(int TIMEOUT = 1000){ // Pulls/Reads incoming data from serial port. It does NOT read the instrution. 
    char* RETURN[0] = 0; Serial.setTimeout(TIMEOUT);
    for(int i ; Serial.available() > 0 ; i++){ // While the serial port is available. 
        RETURN[i] = Serial.read(); // Fill RETURN with incoming bytes. 
    }return RETURN;
}

void serialPush(const char* TXT){ // Send bytes from serial port. Simplification of Serial.Write().
    Serial.write(TXT, sizeof(TXT));
}

void Run(char* INSTRUCTION){ // Reads the instruction to call it after. 
    char* FX[0] = 0; char* ARG[0] = 0; int INDEX = 0;
    for(int i ; i < sizeof(INSTRUCTION) ; i++){
        if(INSTRUCTION[i] != ";"){
            FX[i] = INSTRUCTION[i];
        }else{INDEX = i; break;}
    }for(int i = INDEX; i < sizeof(INSTRUCTION) ; i++){
        if(INSTRUCTION[i] != ";" && INSTRUCTION[i++] != ";"){
            ARG[i] = INSTRUCTION[i];
        }else{break;}
    }switch(FX){ // All functions callable from Raspberry Pi shall have a case here. 
        case "push": 
            serialPush(ARG);
        case "test":
            digitalWrite(4, HIGH);
        default: // Case of unmatched function. 
            return(0); // Panic. 
    }
}