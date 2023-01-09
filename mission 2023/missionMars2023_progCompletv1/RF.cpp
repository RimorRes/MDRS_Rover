/*************************************************
 * Gestion de la communication par radio-fréquence
***************************************************/

//#define AFFICHAGE // à commenter pour rendre moins bavard...
#include <Arduino.h>
#include <RF24.h>
#if !defined RF_H
#include "RF.h"  // module codé par nous
#define RF_H
#endif

boolean emettreMessage(String message){
  int nbrCaracteresMax = 31; // déterminé empiriquement (32 en principe !)
  int nbrPhrases = int(message.length() / nbrCaracteresMax) +1;
  String phrase;
  for (int i=0;i<nbrPhrases;i++){
    phrase = message; // ligne inutile, je crois
    int nbrCaracteres = min(nbrCaracteresMax, message.length());
    phrase = message.substring(0, nbrCaracteres); // on extrait la phrase à émettre
    phrase.concat('\n');
#ifdef AFFICHAGE
    Serial.println("début : " + message + " nbr = " + String(nbrCaracteres));
    Serial.println("   phrase : " + phrase);
#endif
    message = message.substring(nbrCaracteres); // et on l'enlève du message
    char msg[phrase.length()];     // Message à transmettre à l'autre NRF24 (32 caractères maxi, avec cette librairie)
    phrase.toCharArray(msg, sizeof(msg)); // mise en forme : tableau de caractères
    radio.write(&msg, sizeof(msg));     // Envoi de notre message 
    //Serial.write(msg, sizeof(msg));
  }
}

String RFpull(){
  String reception;
  char entrant[32];
  if(radio.available()){
    radio.read(entrant, sizeof(entrant));
  }
  reception = String(entrant);
  return reception;
}

void RFpush(String envoi){
  char paquet[32];
  envoi.toCharArray(paquet,sizeof(*paquet));
  radio.stopListening();
  radio.write(paquet, sizeof(*paquet));
  delay(5); // Delai à determiner plus précisement
  radio.startListening();
  return;
}

/*String recupererMessageRF(){
  int nbrCaractereMax = 31; // identique emettreMessage()
  String phrase = "";
  String message = "";
//  A CODER
  return message;
}*/

String ecouterAntenneRF(){
  int dureeAttente = 1000;  // en millisecondes
  //int TIMEOUT = 1000; // durée max de surveillance de l'antenne
  String reception = RFpull();  // écoute
  if (reception =="A"){ // vérifier que le message commence par "A" permet d'éviter de transmettre de mauvaise instructions si le début du message s'est perdu
    RFpush("R");  // on répond qu'on est prêt
    delay(dureeAttente);
    //Serial.setTimeout(TIMEOUT); // le temps au bout duquel on arrête de surveiller le bus
    while (radio.available()){    //  tant que le bus n'est pas vide...
      reception = RFpull();       // on lit une instruction
      if(reception == "F"){       // Si c'est la fin on arrête d'écouter.
        RFpush("F");
        break;
      }
      Run(reception);             // Sinon c'est qu'on a reçu une instruction, et on l'exécute.
    } // le bus est vide, maintenant
  } // si ça ne commençait pas par "A" on n'a rien fait
  //return;

  //String messageRF = recupererMessageRF();
  return "";//messageRF;
}
