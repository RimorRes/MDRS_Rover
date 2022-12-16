// Dans actualiseDelayTime() : mettre mécanisme de contrôle pour éviter que tout devienne fou si la mesure dysfonctionne
// Par exemple limiter la tension mesurée à un intervalle comme [10,13] Volt avec des fonctions min et max.

/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif 
#if !defined MOTEURS_H
  #include "moteurs.h"
  #define MOTEURS_H
#endif
extern  Moteur moteurAVD, moteurAVG, moteurARD, moteurARG;

Moteur::Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_mesure_tension_alim){
  _PIN_moteur_1 = PIN_moteur_1;
  _PIN_moteur_2 = PIN_moteur_2;
  _PIN_mesure_tension_alim = PIN_mesure_tension_alim;
  _delayTime = 1000*30/12;
}

boolean Moteur::init_moteur() {
  pinMode(_PIN_moteur_1, OUTPUT);
  pinMode(_PIN_moteur_2, OUTPUT);
  pinMode(_PIN_mesure_tension_alim, INPUT);
  _delayTime = actualiseDelayTime();
  return true;  // pas encore de test de réussite !
}

float Moteur::getTensionAlim() const{
  int sensorValue = analogRead(_PIN_mesure_tension_alim);
    //analogRead retourne une valeur entre (0 pour 0 volts) et (1023 pour 5 volts)
  //de plus, la tension reçue a été divisée par 3 donc on la reconvertit
  float voltage = sensorValue * (5.0 / 1023.0) * 3 * 1.035;  // facteur magique parce que ce n'est pas exactement 3
  return voltage;
}

float Moteur::actualiseDelayTime(){
  _delayTime = getDelayTime();
  return _delayTime;
}

//fonction pour activer le moteur dans le sens <sens>
void Moteur::activer(int sens) const{
  if (sens == 1){
    digitalWrite(_PIN_moteur_1, HIGH);
    digitalWrite(_PIN_moteur_2, LOW);
  }
  if (sens == 2){
    digitalWrite(_PIN_moteur_1, LOW);
    digitalWrite(_PIN_moteur_2, HIGH);
  }
}

//fonction pour desactiver le moteur
void Moteur::desactiver() const {
  digitalWrite(_PIN_moteur_1, HIGH);
  digitalWrite(_PIN_moteur_2, HIGH);
}

//fonction pour avancer un nombre <t> de tours
void Moteur::avancer_t(int sens, float tours){
  //Serial.println("Avancer tours START");
  _delayTime = actualiseDelayTime();
  float tours_delaytime = _delayTime * tours;
  activer(sens);
  delay(tours_delaytime);
  desactiver();
  distanceParcourue += tours * PI * 2 * rover_spec.rayonExterneRoueEnMetres;
  //Serial.println("Avancer tours END");
}

//fonction pour avancer un nombre <m> de mètres
void Moteur::avancer_m(int sens, float m){
//  Serial.println("Avancer metres START");
  //le diamètre des roues est de 17.5cm donc la circonférence est de 55cm soit 0.55m
  //afin de convertir les mètres en tours, on multiplie les mètres par 1/0.55 soit 1.82
  float perimetre = PI * 2 * rover_spec.rayonExterneRoueEnMetres;
  float tours = m / perimetre;
  avancer_t(sens, tours);
//  Serial.println("Avancer metres END");
}

// faire avancer tous les moteurs, dans le sens <sens> du nombre de mètres <m>
void avancerTous(int sens, float m){
  // on doit paramétrer de sorte que sens = 1 fasse avancer et sens = 2 fasse reculer, à causde de la fonction tournerSurPlace()
  float perimetre = PI * 2 * rover_spec.rayonExterneRoueEnMetres;
  float tours = m / perimetre;
  float delayTime = getDelayTime();
  float tours_delaytime = delayTime * tours;
  moteurAVD.activer(sens);
  moteurAVG.activer(sens);
  moteurARD.activer(sens);
  moteurARG.activer(sens);
  delay(tours_delaytime);
  moteurAVD.desactiver();
  moteurAVG.desactiver();
  moteurARD.desactiver();
  moteurARG.desactiver();
  distanceParcourue += m;
  return;
}

// faire avancer les deux moteurs d'un côté, et reculer les deux autres, dans le sens <sens>, d'un angle <angle> en degrés
void tournerSurPlace(char sens, int angle){
  // sens vaut 'D' ou 'G'
  // si les PINs sont tels que sens = 1 pour un moteur le fasse avancer, et sens = 2 le fasse reculer.
  float perimetreSurPlace = PI * 2 * rover_spec.rayonSurPlaceEnMetres;
//  Serial.println(perimetreSurPlace);
  float m = angle / 360. * perimetreSurPlace;
//  Serial.println(m);
  float perimetreRoue = PI * 2 * rover_spec.rayonExterneRoueEnMetres;
  float tours = m / perimetreRoue;
//  Serial.println(tours);
  float delayTime = getDelayTime()*rover_spec.numeroMagique2;
  float tours_delaytime = delayTime * tours;
//  Serial.println(tours_delaytime);
  switch (sens){
    case 'G':
      moteurAVD.activer(1);
      moteurARD.activer(1);
      moteurAVG.activer(2);
      moteurARG.activer(2);
    case 'D':
      moteurAVD.activer(2);
      moteurARD.activer(2);
      moteurAVG.activer(1);
      moteurARG.activer(1);
    default:
      ;
  }
  delay(tours_delaytime);
  moteurAVD.desactiver();
  moteurAVG.desactiver();
  moteurARD.desactiver();
  moteurARG.desactiver();
  directionRover += angle;
  return;
}

float getTensionAlim(){
  int sensorValue = analogRead(myPINs.PIN_mesure_tension_alim);
    //analogRead retourne une valeur entre (0 pour 0 volts) et (1023 pour 5 volts)
  //de plus, la tension reçue a été divisée par 3 donc on la reconvertit
  float voltage = sensorValue * (5.0 / 1023.0) * 3 * 1.035;  // facteur magique parce que ce n'est pas exactement 3
  return voltage;
}

// calcule le temps pour qu'un moteur fasse un tour exactement, selon la tension d'alimentation.
float getDelayTime() {
  float voltage = getTensionAlim();
  voltage = max(voltage, rover_config.tensionCodeMin);  // protège contre mesure aberrante
  voltage = min(voltage, rover_config.tensionCodeMax);  // protège contre mesure aberrante

  //conversion pour trouver (numero magique = ~30)
  float t = 1000 * (rover_spec.numeroMagique / voltage);

  //Serial.println("Delaytime: " + String(t));
  return t;
}
