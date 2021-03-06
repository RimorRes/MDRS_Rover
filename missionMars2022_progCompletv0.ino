/*********************************************************************
  module Arduino de la mission Mars 2022, iOSubOrbital
  Programme père
    
  Version originale iOSubOrbital 2021-2022     
 **********************************************************/
/****
TO DO
*****/
//  classes : Dans l'absolu, il faudrait passer les publiques en privées. Tant pis.
// Et puis ce codage des classes de spec est pourri : il faudrait passer la valeur des "constantes" lors de l'initialisation. Tant pis.
 
/*********************************************
PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
***********************************************/
// commenter ce qui suit pour économiser l'espace sur le microcontroleur
#define AFFICHAGE   // Pour indiquer qu'il y aura une sortie sur la console série, idem dans les modules

/*************
SPECIFICATIONS
**************/
// Pour passer en paramètre des fonctions
#include "specifications.h"
PIN_spec myPINs; // définition des broches
Rover_spec rover_spec; // spécifications du rover (géométrie, valeurs limites, ...)

/*********************************
CONNEXION DES PERIPHERIQUES EN I2C
**********************************
brancher GND
brancher Vin sur 3,3 V ou 5 V selon le capteur
  BMP180 : 3,3 V (entre 1,8 et 3,6 V, surtout pas plus de 4,25 V)
  TMP102 : 3,3 V (entre 1,4 et 3,6 V, surtout pas plus de 4,0 V)
  ...
brancher SCL sur A5 (qui est le port SCL pour les Arduino Uno R3)
brancher SDA sur A4 (qui est le port SDA pour les Arduino Uno R3)
L'adresse 7 bits du BMP180 est Ox77.
*/

/***************
MODULES EXTERNES
****************/
/* capteur température interne */
#include <SparkFunTMP102.h>
//const int ALERT_PIN = A3; //Si on veut utiliser la sortie alert
TMP102 sensorTinterne; // donnera la température interne à 0,0625°C près
/* On peut changer l'adresse par défaut, qui est 0x48 = 1001000.
 * Il faut connecter la broche ADD0 (par défaut : sur la masse)
 *    mass : 0x48
 *    VCC - 0x49
 *    SDA - 0x4A
 * mais en fait il n'y a pas la broche ADD0 sur le nôtre, donc l'adresse sera fixe à 0x48
 * On peut aussi modifier le bus utilisé avec E.g. sensor0.begin(0x49,Wire1)
 * It will return true on success or false on failure to communicate.
 */

/* détecteurs d'obstacle */
#include <Ultrasonic.h>
Ultrasonic ultrasonic_1(myPINs.PIN_detectObst1_Trig,myPINs.PIN_detectObst1_Echo); // regarde devant
Ultrasonic ultrasonic_2(myPINs.PIN_detectObst2_Trig,myPINs.PIN_detectObst2_Echo); //  regarde en bas

/* communication série */
#include "serialComm.h" // module codé par nous

/* surveillance de la température interne du rover */
#include "temperatureInterne.h"  // module codé par nous

/* antenne RF */
#include <SPI.h>
#include <RF24.h>
#define pinCE   7             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  8             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel  "PIPE1"       // On définit un "nom de tunnel" (5 caractères), pour pouvoir communiquer d'un NRF24 à l'autre
RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01
const byte adresseAntenne[6] = tunnel;               // Mise au format "byte array" du nom du tunnel

/**********************
FONCTIONNALITES VARIEES
***********************/

#include "deplacement.h"

/**********************
VARIABLES GLOBALES CODE
***********************/
/* indicateurs d'état */
boolean OK_init_Tint, OK_Tint;  // initialisation capteur température interne, état température interne
float Tint_min = 1; // seuil d'alerte bas pour la température interne
float Tint_max = 80; // seuil d'alerte haut pour la température interne
boolean OK_init_moteurs, OK_moteurs;  // initialisation capteur température interne, état température interne
String msg_alerte = "tout va bien; ";

/* mémoire du rover */
String successionOrdresMarche = "";
String cheminSuivi = "";

/* mémoire tampon */
String messageBus = "";

void setup()
{
  Serial.begin(9600);                       
  while(!Serial){;}                         // On attend que le port série soit disponible
// la ligne précédente est-elle à enlever lorsque ce ne sera plus le moniteur série ?
  delay(1000);
  Wire.begin(); //Join I2C Bus
  
  OK_init_Tint = init_tmp102(sensorTinterne);
  if(!OK_init_Tint){
    msg_alerte.concat("problème d'initialisation du capteur de température interne (tmp102); ");
  }

  OK_init_moteurs = init_moteurs(myPINs);
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation des moteurs; ");
  }
  
  // initialisation de l'antenne RF
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresseAntenne);     // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.stopListening();              // Arrêt de l'écoute du NRF24 (signifiant qu'on va émettre, et non recevoir, ici)


#ifdef AFFICHAGE
  Serial.print("état du rover à l'initialisation : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif

pinMode(8, OUTPUT); // LED bleue
pinMode(9, OUTPUT); // LED jaune

}

void loop()
{
  if (OK_init_Tint){
    msg_alerte = msg_alerte.concat(test_temp_int(sensorTinterne, Tint_min, Tint_max));
  }
/*
  int dist_1 = ultrasonic_1.Ranging(CM);
  int dist_2 = ultrasonic_2.Ranging(CM);
#ifdef AFFICHAGE
   Serial.print("distance devant : ");
   Serial.print(dist_1);
   Serial.println(" cm");
   Serial.print("distance vers le bas : ");
   Serial.print(dist_2);
   Serial.println(" cm");
#endif
  
  float tensionBrute = analogRead(A0); // avec un pont diviseur de tension d'un facteur environ 3
  float tension = tensionBrute * 5 /1023 *3; //  Ce facteur 3 vient du pont diviseur de tension, qui évite de mettre le 12 V en entrée de l'arduino.
  //Serial.println("tension d'alimentation moteur = " + String(tension) + " V");

  */
  
  // avancer('a', 1, 1000);
/*  avancerMetres(0.1);
  tournerSurPlaceDegres(1, 90); //dir: 1 clockwise 2 counterclockwise, angle: angle de rotation en degrés
  avancerMetres(0.2);
  tournerSurPlaceDegres(2, 90);
  avancerMetres(0.1);

  successionOrdresMarche.concat("Av_0.100;");
  successionOrdresMarche.concat("Tg_90.00;");
  successionOrdresMarche.concat("Av_0.200;");
  successionOrdresMarche.concat("Td_90.00;");
  successionOrdresMarche.concat("Av_0.100;");*/
/*  
#ifdef AFFICHAGE
  Serial.print("récapitulation du chemin suivi : ");
  Serial.println(successionOrdresMarche);
  Serial.print("état du rover : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif
*/  

  //messageBus = msg_alerte;
  if (messageBus != ""){parlerBus();}
  delay(1000);  // Wait 1000ms
}

void serialEvent(){
  lectureBus();
  return;
}

void lectureBus(){
  int dureeAttente = 1000;  // en millisecondes
  int TIMEOUT = 1000; // durée max de surveillance du bus
  String reception = serialPull();  // lecture bus
  if (reception =="A"){ // vérifier que le message commence par "A" permet d'éviter de transmettre de mauvaise instructions si le début du message s'est perdu
    // la fin du message n'est pas vérifiée, ce qui est un défaut.
    Serial.print("R");  // on répond qu'on est prêt
    delay(dureeAttente);
    Serial.setTimeout(TIMEOUT); // le temps au bout duquel on arrête de surveiller le bus
    while (Serial.available()){ // tant que le bus n'est pas vide...
      reception = serialPull(); // on lit une instruction
      Run(reception); // et on exécute l'instruction
    } // le bus est vide
  } // si ça ne commençait pas par "A" on n'a rien fait
  return;
}

void parlerBus(){
  int dureeAttente = 1000;  // en millisecondes
  int nombreEssais = 2;
  for (int i = 0; i<nombreEssais; i++){ // essais de prise de parole
    Serial.print("A");  // demande la parole
    delay(dureeAttente);
    String reception = serialPull();  // lit le bus
    if (reception =="R"){ // si ça répond...
      Serial.print(messageBus); // on parle
      messageBus="";  // plus rien à dire
      break;  // et on arrête
    } // ...sinon on retente
  }
  return;
}

/*************************
GROS BAZAR DE FONCTIONS...
**************************/

void Run(String INSTRUCTION){ // Reads the instruction to call it after. 
    if (INSTRUCTION != ""){
#ifdef AFFICHAGE
      Serial.println("instruction dans Run : " + INSTRUCTION);
      Serial.println("   taille instruction : " + String(INSTRUCTION.length())); 
#endif
      char separator='_'; // caractère de séparation entre fonction et arguments, ou entre arguments
      
      // récupération du code de la fonction appelée
      int posFin = INSTRUCTION.indexOf(separator); // repère le passage de la fonction aux arguments de la fonction
      int numFonction = INSTRUCTION.substring(0, posFin).toInt(); // lit le début de l'instruction comme un entier codant la future fonction à exécuter. Du coup, on aurait pu coder avec parseInt().
      String chaineArguments = INSTRUCTION.substring(posFin);
#ifdef AFFICHAGE
      Serial.println("   fonction n° : " + String(numFonction));
      Serial.println("   chaine d'arguments : " + chaineArguments);
#endif
      
      // récupération des arguments
      int i=0; int numArg=0;
      for (i=0;i<chaineArguments.length();i++){ // on compte les arguments
        if (chaineArguments[i] == separator){numArg++;}
      }
#ifdef AFFICHAGE
      Serial.print("   Il y a "); Serial.print(numArg); Serial.println(" arguments.");
#endif
      String arguments[numArg]; // le tableau des arguments
      for(i=0;i<numArg;i++){
        chaineArguments = chaineArguments.substring(1); // on enlève le séparateur
        posFin = chaineArguments.indexOf(separator);  // on repère la fin de l'argument
        arguments[i] = chaineArguments.substring(0, posFin); // on extrait l'argument
        chaineArguments = chaineArguments.substring(posFin); // on enlève ce qui a été lu
#ifdef AFFICHAGE
        Serial.println("   argument : " + arguments[i]);
#endif
      }
      
      switch(numFonction){
        case 1:
          Serial.println("Youpi !"); break;
        case 2:
          noel(); break;
        case 3:
          if(numArg != 0){serialPush(arguments[0]);} break;
        case 4:
          if(numArg != 0){Serial.println(arguments[0]);} break;
        case 5:
          if(numArg != 0){
            String message = "";
            for (i=0;i<numArg;i++){
              message += arguments[i];
              message += " ";
            }
            Serial.println(message);
          } break;
        case 6:
          emettreMessage(msg_alerte); break;
        case 7:
          emettreMessage("123456789_123456789_123456789_123456789"); break;
        default: // Case of unmatched function. 
#ifdef AFFICHAGE
            Serial.println("Ca serait gentil de me donner des instructions que je comprends !");
#endif
            return; // Panic.
      }
    }
}

boolean emettreMessage(String message){
  int nbrCaracteresMax = 31; // déterminé empiriquement (36 en principe !)
  int nbrPhrases = int(message.length() / nbrCaracteresMax) +1;
  String phrase;
  for (int i=0;i<nbrPhrases;i++){
    phrase = message;
    int nbrCaracteres = min(nbrCaracteresMax, message.length());
    Serial.println("début : " + message + " nbr = " + String(nbrCaracteres));
    phrase = message.substring(0, nbrCaracteres); // on extrait la phrase à émettre
    phrase.concat('\n');
    Serial.println("   phrase : " + phrase);
    message = message.substring(nbrCaracteres); // et on l'enlève du message
    char msg[phrase.length()];     // Message à transmettre à l'autre NRF24 (32 caractères maxi, avec cette librairie)
    phrase.toCharArray(msg, sizeof(msg)); // mise en forme : tableau de caractères
    radio.write(&msg, sizeof(msg));     // Envoi de notre message 
    //Serial.write(msg, sizeof(msg));
  }
}


boolean init_moteurs(PIN_spec myPINs){
  pinMode(myPINs.PIN_moteur1_1, OUTPUT);
  pinMode(myPINs.PIN_moteur1_2, OUTPUT);
  pinMode(myPINs.PIN_moteur1_3, OUTPUT);
  pinMode(myPINs.PIN_moteur1_4, OUTPUT);
  pinMode(myPINs.PIN_mesure_tension_alim, INPUT);
  return true;
}

void avancer2(int dir, int ms, PIN_spec myPINs) {
  int PIN_moteur1_1 = myPINs.PIN_moteur1_1;
  int PIN_moteur1_2 = myPINs.PIN_moteur1_2;
  int PIN_moteur1_3 = myPINs.PIN_moteur1_3;
  int PIN_moteur1_4 = myPINs.PIN_moteur1_4;
  
  if (dir == 1){
    digitalWrite(PIN_moteur1_1, HIGH);
    digitalWrite(PIN_moteur1_2, LOW);
    digitalWrite(PIN_moteur1_3, HIGH);
    digitalWrite(PIN_moteur1_4, LOW);
  }

  if (dir ==2){
    digitalWrite(PIN_moteur1_2, HIGH);
    digitalWrite(PIN_moteur1_1, LOW);
    digitalWrite(PIN_moteur1_4, HIGH);
    digitalWrite(PIN_moteur1_3, LOW);
  }
  
  
  delay(ms);
  
  digitalWrite(PIN_moteur1_1, HIGH);
  digitalWrite(PIN_moteur1_2, HIGH);
  digitalWrite(PIN_moteur1_3, HIGH);
  digitalWrite(PIN_moteur1_4, HIGH);
}


//motor: 'a'ou 'b', dir: 1 clockwise 2 counterclockwise, ms: temps d'activation en ms
void avancer(char motor, int dir, int ms, PIN_spec myPINs){

  int ia, ib;
    
   if (motor == 'a') {
    ia = myPINs.PIN_moteur1_1; ib = myPINs.PIN_moteur1_2;
    
  }
 
  if (motor == 'b') {
    ia = myPINs.PIN_moteur1_3; ib = myPINs.PIN_moteur1_4;
  }
  
  if (dir == 1){
    digitalWrite(ia, HIGH);
    digitalWrite(ib, LOW);
  }

  if (dir ==2){
    digitalWrite(ib, HIGH);
    digitalWrite(ia, LOW);
  }
  
  delay(ms);
  
  digitalWrite(ia, HIGH);
  digitalWrite(ib, HIGH);
  
}

float lireTensionAlimMoteurs(){ // donne la tension mesurée pour l'alimentation des moteurs, en V
  float tensionBrute = analogRead(A0); // avec un pont diviseur de tension d'un facteur environ 3
  float tension = tensionBrute * 5 /1023 *3; //  Ce facteur 3 vient du pont diviseur de tension, qui évite de mettre le 12 V en entrée de l'arduino.
  return tension;
}
  
void avancerMetres(float dist, Rover_spec rover_spec, PIN_spec myPINs){
  float tensionAlimEnV = lireTensionAlimMoteurs();
  float vitesseMetresParSecondes = 0.248 * (tensionAlimEnV - rover_spec.tensionSeuilAlimMoteurs) * rover_spec.rayonExterneRoueEnMetres;
  
  int ms = int(dist / vitesseMetresParSecondes * 1000); // durée d'alimentation des moteurs, en ms
  
  digitalWrite(myPINs.PIN_moteur1_1, LOW);
  digitalWrite(myPINs.PIN_moteur1_2, LOW);
  digitalWrite(myPINs.PIN_moteur1_3, LOW);
  digitalWrite(myPINs.PIN_moteur1_4, LOW);

  delay(ms);
  
  digitalWrite(myPINs.PIN_moteur1_1, HIGH);
  digitalWrite(myPINs.PIN_moteur1_2, HIGH);
  digitalWrite(myPINs.PIN_moteur1_3, HIGH);
  digitalWrite(myPINs.PIN_moteur1_4, HIGH);
}

void tournerSurPlaceDegres(int dir, float angle, Rover_spec rover_spec, PIN_spec myPINs){
//dir: 1 clockwise 2 counterclockwise, angle: angle de rotation en degrés
  float tensionAlimEnV = lireTensionAlimMoteurs();
  float vitesseMetresParSecondes = 0.248 * (tensionAlimEnV - rover_spec.tensionSeuilAlimMoteurs) * rover_spec.rayonExterneRoueEnMetres;

  float dist = rover_spec.rayonSurPlaceEnMetres * angle / 180 * 3.141592654;
  int ms = int(dist / vitesseMetresParSecondes * 1000); // durée d'alimentation des moteurs, en ms
  
  avancer2(dir, ms, myPINs);
}

void suivreOrdreMarchePonctuel(String ordreMarche){
  // analyser chaîne de caractères
  // deux caractères pour la nature de l'ordre de marche
  // un underscore
  // cinq caractères pour le paramètre numérique (à extraire)
  // un point virgule
  //
  // utiliser les fonctions précédentes
  
  // garde mémoire du chemin suivi
  successionOrdresMarche.concat(ordreMarche);
}
