#include <nRF24L01.h>
#include <RF24.h>

// configuration matérielle
const int PIN_CE = 7;//9;
const int PIN_CSN = 8;//10;
const int PIN_IRQ = 3;
const byte adresses[][6] = {"PIPE1", "PIPE2"};
RF24 radio(PIN_CE, PIN_CSN);

// constantes conventionnelles
enum RFstatesIN{WAITING_FOR_MSG, RECEIVING_PACKETS};
enum RFstatesOUT{NOT_SENDING, WAITING_CONF_START, SENDING, WAITING_CONF_END};
char msgConfirmation[] = "R";
char msgStartExchange[] = "A";
char msgEndExchange[] = "F";

// gestion temps
unsigned long currentTime, previousTime;

// drapeaux d'état
RFstatesIN radioReceiveState = WAITING_FOR_MSG;
RFstatesOUT radioSendState = NOT_SENDING;
boolean msgReceived = false;
boolean msgEmissionRequired = false;

// messages / paquets
char msgEntrant[32];
String msgRecuComplet;
String msgToEmit;
String msgSortant;
int packetCount;

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
  
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), dataReceivedInterrupt, FALLING);
  previousTime = millis();
  
  Serial.println("Setup terminé.");
}

boolean youpi = true;
void loop(){
  if(msgReceived){
    msgReceived = false;
    updateReceiveStateOnMsg();  // met à jour les drapeaux + reçoit le paquet
    updateSendStateOnMsg(); // met à jour les drapeaux
  }
  if (youpi == true){// pour tester d'envoyer un message en un seul exemplaire
    youpi = !sendMsg("youpi ! tralala !");
    Serial.println("   (envoi n°1)");
  }
/*  if (youpi2 == true){// pour tester d'envoyer deux messages en un seul exemplaire (le deuxième attend que le premier ait fini)
    youpi2 = !sendMsg("youpi ! tralalilala !");
    Serial.println("   (envoi n°2)");
  }
  if (youpi3 == true){// pour tester d'envoyer trois messages en un seul exemplaire (mais là c'est trop rapproché, normalement)
    youpi3 = !sendMsg("youpi ! tralalilalère !");
    Serial.println("   (envoi n°3)");
  }*/
  if(msgEmissionRequired) msgEmissionRequired = !trySendMsg();
  updateSendState();
  delay(1000);
  Serial.println("J'ai fini.\r");
}

void updateReceiveStateOnMsg(){
  if(radioReceiveState == WAITING_FOR_MSG){
    if(msgEntrant[0] == msgStartExchange[0]){ // L'interlocuteur a demandé la parole.
      radioReceiveState = RECEIVING_PACKETS;
      msgRecuComplet = "";
      sendPacket(msgConfirmation);
    }
    else if(msgEntrant[0] == msgEndExchange[0]){  // L'interlocuteur annonce la fin de sa parole (alors qu'on ne l'écoutait même pas).
      sendPacket(msgConfirmation);
    } // Les autres cas possibles ne sont pas traités : ils ne correspondent pas à une conversation normale.
  }
  else{ //radioReceiveState == RECEIVING_PACKETS
    if(msgEntrant[0] == msgStartExchange[0]){ // L'interlocuteur n'a pas compris qu'on l'écoute déjà.
              // Ici, on devrait peut-être enlever tous les "A" qui auraient pu s'accumuler si l'interlocuteur parle trop vite,
              // puis considérer que le reste est le message.
      sendPacket(msgConfirmation);
    } else if(msgEntrant[0] == msgEndExchange[0]){  // L'interlocuteur annonce la fin de sa parole.
      radioReceiveState = WAITING_FOR_MSG;
      Serial.println("Message reçu: " + msgRecuComplet);
      sendPacket(msgConfirmation);
    } else{
      msgRecuComplet += msgEntrant;
    }
  }
}

void updateSendStateOnMsg(){
  switch (radioSendState){
    case NOT_SENDING: return;
    case WAITING_CONF_START:{
      if(msgEntrant[0] == msgConfirmation[0]){  // Normalement, msgEntrant ne comporte que ce seul caractère, et '\0'. Le reste est-il perdu ?
        Serial.println("Début de la transmission.");
        radioSendState = SENDING;
      }
    }
    case SENDING: Serial.println("C'est quoi ce bazar ? JE parle. Tais-toi.");
    case WAITING_CONF_END:{
      if(msgEntrant[0] == msgConfirmation[0]){  // Si msgEntrant contient aussi un 'A', il sera perdu (?), on détectera le suivant.
        Serial.println("Message transmis.");
        radioSendState = NOT_SENDING;
      }
    }
  }
}

void updateSendState(){
  Serial.println("   radioSendState = " + String(radioSendState));
  if(radioSendState == NOT_SENDING) return;
  currentTime = millis();
  Serial.println("      temps écoulé = " + String(currentTime - previousTime));
  if(currentTime - previousTime > 100){ // On peut changer la durée entre chaque envoi de message, ici 100ms
    previousTime = currentTime;
    //radioSendState = SENDING;// JUSTE POUR UN TEST : A ENLEVER !!!
    switch (radioSendState){
      case WAITING_CONF_START:{
        Serial.println(" Je suis dans WAITING_CONF_START");
        sendPacket(msgStartExchange);
        break;
      }
      case SENDING:{
        Serial.println(" Je suis dans SENDING");
        char packet[32];
        msgSortant.substring(31*packetCount, min(31*packetCount + 31,msgSortant.length())).toCharArray(packet, 32);
        Serial.println("Nouveau paquet envoyé: " + String(packet));
        sendPacket(packet);
        //if(msgSortant.length() >> 5 == packetCount) radioSendState = WAITING_CONF_END;
        ++packetCount;   
        if(msgSortant.length()/ 31 <= packetCount) radioSendState = WAITING_CONF_END;
        break;
      }
      case WAITING_CONF_END:{
        Serial.println(" Je suis dans WAITING_CONF_END");
        sendPacket(msgEndExchange);
        break;
      }
    }
  }
}

boolean sendMsg(String msg){
  if(msgEmissionRequired == true){
    Serial.println("Il y a la queue pour l'émission.");
    return false;
  } else {
    msgToEmit = msg;
    msgEmissionRequired = !trySendMsg();
    Serial.println("Votre demande d'émission est prise en compte.");
    return true;
  }
}

boolean trySendMsg(){
if(radioSendState == NOT_SENDING && radioReceiveState == WAITING_FOR_MSG){
    msgSortant = msgToEmit;
    packetCount = 0;
    radioSendState = WAITING_CONF_START;
    Serial.println("Message waiting to be transmitted.");
    return true;
  }
  else{
    Serial.println("Message cannot be transmitted in this moment.");
    return false;
  }
}

void sendPacket(char* packet){
  radio.stopListening();
  radio.write(packet, sizeof(*packet));
  delay(5); // Delai à determiner plus précisement
  radio.startListening();
  Serial.println(" Je viens d'envoyer :" + String(packet));
}

void dataReceivedInterrupt(){
  Serial.println("interruption !");
  if(radio.available()){
    radio.read(msgEntrant, sizeof(msgEntrant));
    msgReceived = true;
  }
}
