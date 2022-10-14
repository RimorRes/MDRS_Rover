#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
#if !defined GPS_H
  #include "GPS.h"
  #define GPS_H
#endif
#if !defined SPECIFICATIONS_H
  #include "specifications.h"  // module codé par nous
  #define SPECIFICATIONS_H
#endif 

Rover_config::Rover_config(){
  setCentreRepere(_limiteSO); // à initialiser d'après la carte
  actualiserLimites();
  cheminParDefaut = Chemin(_limiteSO, _limiteNE); // diagonale à travers la zone de jeu.
  cheminParDefaut.addPoint(_limiteSO);  // retour à la case départ.
}

void Rover_config::setCentreRepere(Point point){_centreRepere = point; return;}
Point Rover_config::getCentreRepere() const {return _centreRepere;}

void Rover_config::actualiserLimites(){
    _limiteSO = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 22.90), -convertDegMinSecToDecimal(110, 47, 23.05)); // 38°24'22.90"N, 110°47'23.05"O
    _limiteNO = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 42.11), -convertDegMinSecToDecimal(110, 47, 23.74)); // 38°24'42.11"N, 110°47'23.74"O
    _limiteNE = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 42.55), -convertDegMinSecToDecimal(110, 47, 5.81)); // 38°24'42.55"N, 110°47'5.81"O
    return;
}
Point Rover_config::getLimiteSO() const {return _limiteSO;}
Point Rover_config::getLimiteNO() const {return _limiteNO;}
Point Rover_config::getLimiteNE() const {return _limiteNE;}
