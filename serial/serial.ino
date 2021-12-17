/*Serial communication between Arduino and Raspberry Pi. */

char* serialPull(int TIMEOUT = 1000){ // Pulls/Reads incoming data from serial port. It does NOT read the instrution. 
  int incomingByte = 0; // for incoming serial data
    /*char* RETURN;
    RETURN[0]=0;
    Serial.setTimeout(TIMEOUT);
    for(int i ; Serial.available() > 0 ; i++){ // While the serial port is available. 
        RETURN[i] = Serial.read(); // Fill RETURN with incoming bytes. 
    }
    Serial.println(RETURN);
    return RETURN;*/
    // reply only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}

void serialPush(const char* TXT){ // Send bytes from serial port. Simplification of Serial.Write().
    Serial.write(TXT, sizeof(TXT));
}

void noel(/*const char* ARG*/){
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    delay(1000);
    //Serial.println(ARG);
}

void Run(char* INSTRUCTION){ // Reads the instruction to call it after. 
    char* FX; FX[0]=0; char* ARG; ARG[0]=0; int INDEX = 0;
    int instruction_int;
    for(int i ; i < sizeof(INSTRUCTION) ; i++){
      instruction_int= (int)INSTRUCTION[i];
        if(instruction_int != 59){ // 59 is ascii for ";"
            FX[i] = INSTRUCTION[i];
        }
        else{INDEX = i; break;}
        Serial.print("aléatoire");
        Serial.println(FX);
    }
    for(int i = INDEX; i < sizeof(INSTRUCTION) ; i++){
        if(INSTRUCTION[i] != 59 && INSTRUCTION[i++] != 59){
            ARG[i] = INSTRUCTION[i];
        }else{break;}
    }
    switch((int)FX){ // All functions callable from Raspberry Pi shall have a case here. 
        case 1: 
            serialPush(ARG);
        //case "test":
        //    digitalWrite(4, HIGH);
        case 2:
            noel(/*ARG*/);
        default: // Case of unmatched function. 
            return; // Panic. 
    }
}