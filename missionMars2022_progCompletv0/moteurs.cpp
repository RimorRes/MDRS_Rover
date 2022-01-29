/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>
#include "specifications.h"
extern PIN_spec myPINs;
#include "moteurs.h"

Moteur::Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_moteur_3, int PIN_moteur_4, int PIN_mesure_tension_alim){
  int _PIN_moteur_1 = PIN_moteur_1;
  int _PIN_moteur_2 = PIN_moteur_2;
  int _PIN_moteur_3 = PIN_moteur_3;
  int _PIN_moteur_4 = PIN_moteur_4;
  int _PIN_mesure_tension_alim = PIN_mesure_tension_alim;
}

boolean Moteur::init_moteur() const {
  pinMode(_PIN_moteur_1, OUTPUT);
  pinMode(_PIN_moteur_2, OUTPUT);
  pinMode(_PIN_moteur_3, OUTPUT);
  pinMode(_PIN_moteur_4, OUTPUT);
  pinMode(_PIN_mesure_tension_alim, INPUT);
  return true;  // pas encore de test de réussite !
}
