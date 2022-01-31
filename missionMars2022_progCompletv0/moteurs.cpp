/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>
#include "specifications.h"
extern PIN_spec myPINs;
extern Rover_spec rover_spec;
#include "moteurs.h"

Moteur::Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_moteur_3, int PIN_moteur_4, int PIN_mesure_tension_alim){
  _PIN_moteur_1 = PIN_moteur_1;
  _PIN_moteur_2 = PIN_moteur_2;
  _PIN_moteur_3 = PIN_moteur_3;
  _PIN_moteur_4 = PIN_moteur_4;
  _PIN_mesure_tension_alim = PIN_mesure_tension_alim;
  _delayTime = 1000*30/12;
}

boolean Moteur::init_moteur() {
  pinMode(_PIN_moteur_1, OUTPUT);
  pinMode(_PIN_moteur_2, OUTPUT);
  pinMode(_PIN_moteur_3, OUTPUT);
  pinMode(_PIN_moteur_4, OUTPUT);
  pinMode(_PIN_mesure_tension_alim, INPUT);
  _delayTime = actualiseDelayTime();
  return true;  // pas encore de test de réussite !
}

float Moteur::actualiseDelayTime(){
  int sensorValue = analogRead(_PIN_mesure_tension_alim);
  
  //analogRead retourne une valeur entre (0 pour 0 volts) et (1023 pour 5 volts)
  //de plus, la tension reçue a été divisée par 3 donc on la reconvertit
  float voltage = sensorValue * (5.0 / 1023.0) * 3 * 1.035;
  
  //Serial.println("Voltage: " + String(voltage));

  //conversion pour trouver (numero magique = ~30)
  float t = 1000 * (30 / voltage);

  //Serial.println("Delaytime: " + String(t));

  _delayTime = t;
  return t;
}

//fonction pour activer le moteur <moteur> dans le sens <sens>
void Moteur::activer(char moteur, int sens) const{

  if (moteur == 'A'){
    if (sens == 1){
      digitalWrite(_PIN_moteur_1, HIGH);
      digitalWrite(_PIN_moteur_2, LOW);
    }
    if (sens == 2){
      digitalWrite(_PIN_moteur_1, LOW);
      digitalWrite(_PIN_moteur_2, HIGH);
      }
  }
  if (moteur == 'B'){
    if (sens == 1){
      digitalWrite(_PIN_moteur_3, HIGH);
      digitalWrite(_PIN_moteur_4, LOW);
    }
    if (sens == 2){
      digitalWrite(_PIN_moteur_3, LOW);
      digitalWrite(_PIN_moteur_4, HIGH);
      }
  }
  
}

//fonction pour desactiver le moteur <moteur>
void Moteur::desactiver(char moteur) const {

  if (moteur == 'A'){
    digitalWrite(_PIN_moteur_1, HIGH);
    digitalWrite(_PIN_moteur_2, HIGH);
  }
  if (moteur == 'B'){
    digitalWrite(_PIN_moteur_3, HIGH);
    digitalWrite(_PIN_moteur_4, HIGH);
  }
  
}

//fonction pour avancer un nombre <t> de tours
void Moteur::avancer_t(char moteur, int sens, float tours){
  //Serial.println("Avancer tours START");
  _delayTime = actualiseDelayTime();
  float tours_delaytime = _delayTime * tours;
  activer(moteur, sens);
  delay(tours_delaytime);
  desactiver(moteur);
  //Serial.println("Avancer tours END");
}

//fonction pour avancer un nombre <m> de mètres
void Moteur::avancer_m(char moteur, int sens, float m){
  Serial.println("Avancer metres START");
  //le diamètre des roues est de 17.5cm donc la circonférence est de 55cm soit 0.55m
  //afin de convertir les mètres en tours, on multiplie les mètres par 1/0.55 soit 1.82
  float perimetre = 3.141592654 * 2 * rover_spec.rayonExterneRoueEnMetres;
  float tours = m / perimetre;
  avancer_t(moteur, sens, tours);
  Serial.println("Avancer metres END");
}
