/*
   ______               _                  _///  _           _                   _
  /   _  \             (_)                |  __\| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier:      CommunicationBidirectionnelleNRF24L01-Arduino1
  Description:  Pilotage d'un servomoteur à distance, depuis un autre Arduino, via un NRF24L01
  Auteur:       Passion-Électronique

  Librairie utilisée : https://github.com/nRF24/RF24

  Créé le 20.03.2021
*/
#include <SPI.h>
#include <RF24.h>

#define pinCE     7         // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN    8         // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino

#define tunnel1 "PIPE1"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE2"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24

RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"

char msg_entrant[32];
char msg_sortant[32] = "Hello world!";

void setup() {
  Serial.begin(9600);
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  Serial.println("Setup terminé.");
}

void loop() {
  // ******** ENVOI ********
  radio.stopListening();                                                  // On commence par arrêter le mode écoute, pour pouvoir émettre les données
  //Serial.println("J'arrête d'écouter.");
  radio.write(&msg_sortant, sizeof(msg_sortant));                           // … et on envoi cette valeur à l'autre arduino, via le NRF24
  //Serial.println("Message envoyé.");
  delay(5);                                                               // avec une petite pause, avant de passer à la suite

  // ******** RÉCEPTION ********
  radio.startListening();                                                 // On commence par arrêter le mode envoi, pour pouvoir réceptionner des données
  //Serial.println("J'écoute.");
  if(radio.available()) {                                                 // On regarde si une donnée a été reçue
    String message;
    while(radio.available()){
      radio.read(&msg_entrant, sizeof(msg_entrant));                        // Lecture des données reçues, une par une
      strcpy(msg_sortant, msg_entrant);
      message += String(msg_entrant);
      delay(20);
    }
    Serial.print("Message reçu: "); Serial.println(message);
    delay(20);                                                            // avec une petite pause, avant de reboucler
  }
  delay(5);
}
