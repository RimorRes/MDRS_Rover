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
    limiteSO = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 22.90), -convertDegMinSecToDecimal(110, 47, 23.05)); // 38°24'22.90"N, 110°47'23.05"O
    limiteNO = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 42.11), -convertDegMinSecToDecimal(110, 47, 23.74)); // 38°24'42.11"N, 110°47'23.74"O
    limiteNE = convertSphereToPlan(convertDegMinSecToDecimal(38, 24, 42.55), -convertDegMinSecToDecimal(110, 47, 5.81)); // 38°24'42.55"N, 110°47'5.81"O
    centreRepere = limiteSO; // à initialiser d'après la carte

     // La déclaration suivante doit être la dernière, sinon ça bugge.
    cheminParDefaut = Chemin(limiteSO, limiteNE); // diagonale à travers la zone de jeu.
};
