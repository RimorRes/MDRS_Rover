/***************
  SPECIFICATIONS
****************/
// Pour passer des paramètres de config aux fonctions
#if !defined SPECIFICATIONS_H
#include "specifications.h"  // module codé par nous
#define SPECIFICATIONS_H
#endif
/* antenne RF */
#include <SPI.h>
#include <RF24.h>
#define tunnel  "PIPE1"       // On définit un "nom de tunnel" (5 caractères), pour pouvoir communiquer d'un NRF24 à l'autre
RF24 radio(myPINs.PIN_RF_CE, myPINs.PIN_RF_CSN);    // Instanciation du NRF24L01 // déclarer extern en tête de RF.cpp
const byte adresseAntenneEcriture[6] = "PIPE2";              // Mise au format "byte array" du nom du tunnel (6 caractère à cause du caractère de fin de chaîne)
const byte adresseAntenneLecture[6] = "PIPE1";
#if !defined RF_H
#include "RF.h"  // module codé par nous
#define RF_H
#endif
String messageRF ="";

/*******************************************************************************
            SETUP()
********************************************************************************/
void setup()
{
// initialisation de la comm série USB
  Serial.begin(9600); // pour afficher correctement le GPS, il faudrait 115200 ? A voir <---------------------------------------
  delay(1000);
  Serial.println("coucou, setup()");

  
  // initialisation de l'antenne RF
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresseAntenneEcriture);     // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.openReadingPipe(1,adresseAntenneLecture);     // Ouverture d'un des cinq tunnels en LECTURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.stopListening();              // Arrêt de l'écoute du NRF24 (signifiant qu'on va émettre, et non recevoir, ici)
  Serial.println("fin de l'initialisation de l'antenne RF");

  messageRF = "Vive la Bretagne ! C'est vraiment un chouette pays. On ne va pas se battre pour un morceau de cailloux au milieu de l'eau.";
}

/*******************************************************************************
            LOOP()
********************************************************************************/

void loop()
{
  // réception RF
  // ------------
  //messageRF ="Ceci est un message.";
  // if(j'ai reçu quelque chose) {veille = false;}
  String message = recupererMessageRF();
  radio.stopListening();              // Arrêt de l'écoute du NRF24
  Serial.println("J'arrête d'écouter.");
  if(message==messageRF){
    messageRF = "Bien reçu.";
  }
  emettreMessage(messageRF);
  delay(5);
  
  delay(2000);  // Wait 1000ms // bien le temps des tests, mais ça peut être réduit ensuite. jusqu'à zéro ? déjà 100 serait plus fluide.
} // FIN DE LOOP()
