#include <nRF24L01.h>
#include <RF24.h>

enum states{ WAITING_FOR_MSG, RECEIVING_PACKETS };

const int PIN_CE = 7;//9;
const int PIN_CSN = 8;//10;
const byte adresses[][6] = {"PIPE1", "PIPE2"};
const char msgConfirmation[] = "R";

char radioState = WAITING_FOR_MSG;
boolean msgReceived = false;

char msgEntrant[32];
String msg;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  Serial.begin(9600);
  radio.begin();
  
  radio.setAutoAck(1);
//  radio.enableAckPayload();
  radio.maskIRQ(1, 1, 0);
  radio.setPALevel(RF24_PA_MIN);//LOW);
  radio.openWritingPipe(adresses[0]);
  radio.openReadingPipe(1, adresses[1]);
  radio.startListening();
  
  attachInterrupt(1, dataReceivedInterrupt, FALLING);
  
  Serial.println("Setup terminé.");
}

void loop(){
  if(msgReceived){
    msgReceived = false;

    if(radioState == WAITING_FOR_MSG){
      if(msgEntrant[0] == 'A'){
        radioState = RECEIVING_PACKETS;
        msg = "";
        confirmReception();
      }
      else if(msgEntrant[0] == 'F'){ // J'ai mis 'F' mais je ne sais pas comment le rover signale la fin d'un message
        confirmReception();
      }
    }
    else{ //radioState == RECEIVING_PACKETS
      if(msgEntrant[0] == 'A'){
        confirmReception();
      }
      else if(msgEntrant[0] == 'F'){ // Pareil ici
        radioState = WAITING_FOR_MSG;
        Serial.println("Message reçu: " + msg);
        confirmReception();
      }
      else{
        msg += msgEntrant;
      }
    }
  }
}

void confirmReception(){
  radio.stopListening();
  radio.write(msgConfirmation, sizeof(msgConfirmation));
  delay(5); // Delai à determiner plus précisement
  radio.startListening();
}

void dataReceivedInterrupt(){
  if(radio.available()){
    radio.read(msgEntrant, sizeof(msgEntrant));
    msgReceived = true;
  }
}
