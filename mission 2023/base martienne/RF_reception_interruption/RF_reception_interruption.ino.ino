#include <nRF24L01.h>
#include <RF24.h>

enum states{WAITING_FOR_MSG, RECEIVING_PACKETS, NOT_SENDING, WAITING_CONF_START, SENDING, WAITING_CONF_END};

unsigned long currentTime, previousTime;

const int PIN_CE = 7;//9;
const int PIN_CSN = 8;//10;
const byte adresses[][6] = {"PIPE1", "PIPE2"};

char msgConfirmation[] = "R";
char msgStartExchange[] = "A";
char msgEndExchange[] = "F"; // J'ai mis 'F' mais je ne sais pas comment le rover signale la fin d'un message

char radioReceiveState = WAITING_FOR_MSG;
char radioSendState = NOT_SENDING;
boolean msgReceived = false;

char msgEntrant[32];
String msg;

char* msgSortant;
char packetCount;

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
  previousTime = millis();
  
  Serial.println("Setup terminé.");
}

boolean youpi = true;
void loop(){
  if(msgReceived){
    msgReceived = false;
    updateReceiveStateOnMsg();
    updateSendStateOnMsg();
  }
  updateSendState();
  if (youpi){
    youpi = false;
    char msg[8]= "youpi !";
    sendMsg(msg);
  }
}

void updateReceiveStateOnMsg(){
  if(radioReceiveState == WAITING_FOR_MSG){
    if(msgEntrant[0] == msgStartExchange[0]){
      radioReceiveState = RECEIVING_PACKETS;
      msg = "";
      sendPacket(msgConfirmation);
    }
    else if(msgEntrant[0] == msgEndExchange[0]){
      sendPacket(msgConfirmation);
    }
  }
  else{ //radioReceiveState == RECEIVING_PACKETS
    if(msgEntrant[0] == msgStartExchange[0]){
      sendPacket(msgConfirmation);
    }
    else if(msgEntrant[0] == msgEndExchange[0]){
      radioReceiveState = WAITING_FOR_MSG;
      Serial.println("Message reçu: " + msg);
      sendPacket(msgConfirmation);
    }
    else{
      msg += msgEntrant;
    }
  }
}

void updateSendStateOnMsg(){
  if(radioSendState == NOT_SENDING) return;
  if(radioSendState == WAITING_CONF_START){
    if(msgEntrant[0] == msgConfirmation[0]){
      Serial.println("Début de la transmission.");
      radioSendState = SENDING;
    }
  }
  else if(radioSendState == WAITING_CONF_END){
    if(msgEntrant[0] == msgConfirmation[0]){
      Serial.println("Message transmis.");
      radioSendState = NOT_SENDING;
    }
  }
}

void updateSendState(){
  if(radioSendState == NOT_SENDING) return;
  currentTime = millis();
  if(currentTime - previousTime > 100){ // On peut changer la durée entre chaque envoi de message, ici 100ms
    previousTime = currentTime;
    if(radioSendState == WAITING_CONF_START){
      sendPacket(msgStartExchange);
    }
    else if(radioSendState == SENDING){
      char packet[32];
      memcpy(packet, msgSortant+(31*packetCount), 31);
      packet[31] = '\0';
      sendPacket(packet);
      if(strlen(msgSortant) >> 5 == packetCount) radioSendState = WAITING_CONF_END;
      ++packetCount;
    }
    else{ //radioSendState == WAITING_CONF_END
      sendPacket(msgEndExchange);
    }
  }
}

void sendMsg(char* msg){
  if(radioSendState == NOT_SENDING){
    msgSortant = msg;
    packetCount = 0;
    radioSendState = WAITING_CONF_START;
    Serial.println("Message waiting to be transmitted.");
  }
  Serial.println("Message cannot be transmitted in this moment.");
}

void sendPacket(char* packet){
  radio.stopListening();
  radio.write(packet, sizeof(*packet));
  delay(5); // Delai à determiner plus précisement
  radio.startListening();
}

void dataReceivedInterrupt(){
  if(radio.available()){
    radio.read(msgEntrant, sizeof(msgEntrant));
    msgReceived = true;
  }
}
