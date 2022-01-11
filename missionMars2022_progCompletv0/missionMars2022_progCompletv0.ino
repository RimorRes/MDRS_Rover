/*********************************************************************
  module Arduino de la mission Mars 2022, iOSubOrbital
  Programme père
    
  Version originale iOSubOrbital 2021-2022     
 **********************************************************/
 
/*********************************************
PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
***********************************************/
// commenter ce qui suit pour économiser l'espace sur le microcontroleur
#define AFFICHAGE   // Pour indiquer qu'il y aura une sortie sur la console série

/*********************
DEFINITION DES BROCHES
***********************/
const int PIN_moteur1_1 = 4; // avant droit ?
const int PIN_moteur1_2 = 5; // avant gauche ?
const int PIN_moteur1_3 = 6; // arrière droit ?
const int PIN_moteur1_4 = 7; // arrière gauche ?
const int PIN_mesure_tension_alim = A0; // mesure tension alimentation des moteurs

const int PIN_detectObst1_Trig = 8;
const int PIN_detectObst1_Echo = 9;
const int PIN_detectObst2_Trig = 10;
const int PIN_detectObst2_Echo = 11;

const int PIN_SCL = A5; // inutile, c'est juste pour penser à la réserver. On ne peut pas changer la valeur.
const int PIN_SDA = A4; // inutile, c'est juste pour penser à la réserver. On ne peut pas changer la valeur.


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

/**********************
SPECIFICATIONS DU ROVER
***********************/
const float rayonExterneRoueEnMetres = 0.03; // valeur réelle ?
const float tensionSeuilAlimMoteurs = 1.39; // tension à partir de laquelle les moteurs tournent, en V
const float rayonBraquageEnMetres = 0.40; // valeur réelle ? Il s'agit du rayon de braquage en tournant sur place.

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

/* détercteurs d'obstacle */
#include <Ultrasonic.h>
Ultrasonic ultrasonic_1(PIN_detectObst1_Trig,PIN_detectObst1_Echo); // regarde devant
Ultrasonic ultrasonic_2(PIN_detectObst2_Trig,PIN_detectObst2_Echo); //  regarde en bas

/* communication série */
#include "serialComm.h" // module codé par nous

/* surveillance de la température interne du rover */
#include "temperatureInterne.h"  // module codé par nous

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
String cheminSuivi = "";

void setup()
{
#ifdef AFFICHAGE
  Serial.begin(9600);                       
  while(!Serial){;}                         // On attend que le port série soit disponible
  delay(1000);
#endif
  Wire.begin(); //Join I2C Bus
  
  OK_init_Tint = init_tmp102(sensorTinterne);
  if(!OK_init_Tint){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du capteur de température interne (tmp102); ");
  }

  OK_init_moteurs = init_moteurs();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation des moteurs; ");
  }

#ifdef AFFICHAGE
  Serial.print("état du rover à l'initialisation : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif

}

void loop()
{
  if (OK_init_Tint){
    msg_alerte = msg_alerte.concat(test_temp_int(sensorTinterne, Tint_min, Tint_max));
  }

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
#ifdef AFFICHAGE
  Serial.print("tension d'alimentation moteur = ");
  Serial.print(tension);
  Serial.println(" V");
#endif
  
  serialPull();
  
  // avancer('a', 1, 1000);
/*  avancerMetres(0.1);
  tournerSurPlaceDegres(1, 90); //dir: 1 clockwise 2 counterclockwise, angle: angle de rotation en degrés
  avancerMetres(0.2);
  tournerSurPlaceDegres(2, 90);
  avancerMetres(0.1);

  cheminSuivi.concat("Av_0.100;");
  cheminSuivi.concat("Tg_90.00;");
  cheminSuivi.concat("Av_0.200;");
  cheminSuivi.concat("Td_90.00;");
  cheminSuivi.concat("Av_0.100;");*/
  
#ifdef AFFICHAGE
  Serial.print("récapitulation du chemin suivi : ");
  Serial.println(cheminSuivi);
#endif

#ifdef AFFICHAGE
  Serial.print("état du rover : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif
  
  delay(1000);  // Wait 1000ms
}

/*************************
GROS BAZAR DE FONCTIONS...
**************************/

boolean init_moteurs(){
  pinMode(PIN_moteur1_1, OUTPUT);
  pinMode(PIN_moteur1_2, OUTPUT);
  pinMode(PIN_moteur1_3, OUTPUT);
  pinMode(PIN_moteur1_4, OUTPUT);
  pinMode(PIN_mesure_tension_alim, INPUT);
  return true;
}

void avancer2(int dir, int ms) {
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
void avancer(char motor, int dir, int ms){

  int ia, ib;
    
   if (motor == 'a') {
    ia = PIN_moteur1_1; ib = PIN_moteur1_2;
    
  }
 
  if (motor == 'b') {
    ia = PIN_moteur1_3; ib = PIN_moteur1_4;
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
  
void avancerMetres(float dist){
  float tensionAlimEnV = lireTensionAlimMoteurs();
  float vitesseMetresParSecondes = 0.248 * (tensionAlimEnV - tensionSeuilAlimMoteurs) * rayonExterneRoueEnMetres;
  
  int ms = int(dist / vitesseMetresParSecondes * 1000); // durée d'alimentation des moteurs, en ms
  
  digitalWrite(PIN_moteur1_1, LOW);
  digitalWrite(PIN_moteur1_2, LOW);
  digitalWrite(PIN_moteur1_3, LOW);
  digitalWrite(PIN_moteur1_4, LOW);

  delay(ms);
  
  digitalWrite(PIN_moteur1_1, HIGH);
  digitalWrite(PIN_moteur1_2, HIGH);
  digitalWrite(PIN_moteur1_3, HIGH);
  digitalWrite(PIN_moteur1_4, HIGH);
}

void tournerSurPlaceDegres(int dir, float angle){
//dir: 1 clockwise 2 counterclockwise, angle: angle de rotation en degrés
  float tensionAlimEnV = lireTensionAlimMoteurs();
  float vitesseMetresParSecondes = 0.248 * (tensionAlimEnV - tensionSeuilAlimMoteurs) * rayonExterneRoueEnMetres;

  float dist = rayonBraquageEnMetres * angle / 180 * 3.141592654;
  int ms = int(dist / vitesseMetresParSecondes * 1000); // durée d'alimentation des moteurs, en ms
  
  avancer2(dir, ms);
}

void suivreOrdreMarchePonctuel(String ordreMarche){
  // analyser chaine de caractères
  // deux caractères pour la nature de l'ordre de marche
  // un underscore
  // cinq caractères pour le paramètre numérique (à extraire)
  // un point virgule
  //
  // utiliser les fonctions précédentes
  
  // garde mémoire du chemin suivi
  cheminSuivi.concat(ordreMarche);
}
