#include <SPI.h>
#include <RF24.h>

#define pinCE     7         // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN    8         // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino

#define tunnel1 "PIPE1"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE2"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24

RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"

char msg_entrant[32];
char msg_sortant;

void setup() {
  // bus série
  Serial.begin(9600);
  // antenne RF
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  Serial.println("Setup terminé.");
}

void loop() {
  // ******** ENVOI / RECEPTION BUS SERIE ********
  radio.stopListening();                                                  // On commence par arrêter le mode écoute, pour pouvoir émettre les données
  //Serial.println("écriture RF");
  if(Serial.available()) {                                                // S'il y a quelque chose de reçu sur le bus série ...
    while(Serial.available()>0){                                          // ...on écoute le bus série
      msg_sortant = (char)Serial.read();                                  // lecture bus série
      radio.write(&msg_sortant, sizeof(msg_sortant));                     // … et envoi à l'autre arduino, via le NRF24.
      delay(20); // essayer de diminuer autant que faire se peut
    }
  }
  Serial.println("fin écriture RF");
  delay(5);                                                               // avec une petite pause, avant de passer à la suite

  // ******** RÉCEPTION RF / ENVOI BUS SERIE ********
  radio.startListening();                                                 // On commence par arrêter le mode envoi, pour pouvoir réceptionner des données
  //Serial.println("lecture RF");
  if(radio.available()) {                                                 // Si une donnée a été reçue en RF
    while(radio.available()){                                             // ... on écoute la RF
      radio.read(&msg_entrant, sizeof(msg_entrant));                      // Lecture des données reçues, une par une ...
      Serial.print(String(msg_entrant));                                  // ... et renvoi sur le bus série.
      delay(20);
    }
    delay(20);                                                            // avec une petite pause, avant de reboucler
  }
  //Serial.println("fin lecture RF");
  delay(5);
}
