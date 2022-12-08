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

// conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale
float convertDegMinSecToDecimal(float deg, float minutes, float sec){
  minutes += sec /60;
  deg += minutes / 60;
  return deg;
}

// conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
Point convertSphereToPlan(float lat, float lon, Point origine){
  // angles en degrés, valeurs décimales
  // latitude : positive pour le nord, négative pour le sud
  // longitude : positive pour l'est, négative pour l'ouest
  const long RayonTerreEnMetres = 6370000;
  float y = lat / 360 * 2 * 3.141592654 * RayonTerreEnMetres;
  y -= origine.getY();
  float x = lon / 360 * 2 * 3.141592654 * RayonTerreEnMetres * cos(lat);
  x -= origine.getX();
  return Point(x, y);
}
/*Point convertSphereToPlan(float lat, float lon){
  return convertSphereToPlan(lat, lon, _centreRepere);
}*/

const PIN_spec myPINs = PIN_spec();
const Rover_spec rover_spec = Rover_spec();
const Rover_config rover_config = Rover_config();
