#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
#if !defined SPECIFICATIONS_H
  #include "specifications.h"  // module codé par nous
  #define SPECIFICATIONS_H
#endif 

Chemin Rover_config::initCheminParDefaut() {
  Chemin chemin = Chemin(_limiteSO, _limiteNE); // diagonale à travers la zone de jeu.
  chemin.addPoint(_limiteSO);  // retour à la case départ.
  return chemin;
}

/*Rover_config::Rover_config(){
  //initCheminParDefaut();
}*/

Point Rover_config::getCentreRepere() const {return _centreRepere;}
Point Rover_config::getLimiteSO() const {return _limiteSO;}
Point Rover_config::getLimiteNO() const {return _limiteNO;}
Point Rover_config::getLimiteNE() const {return _limiteNE;}
Chemin Rover_config::getCheminParDefaut() const {;}

const PIN_spec myPINs = PIN_spec();
const Rover_spec rover_spec = Rover_spec();
const Rover_config rover_config = Rover_config();
