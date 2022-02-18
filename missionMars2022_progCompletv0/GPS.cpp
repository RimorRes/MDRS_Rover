/*****************************************************************
 * Définition des fonctions utiles à la gestion de la position GPS
******************************************************************/

#include <Arduino.h>
#include <Math.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif 
extern Rover_config rover_config;
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
#if !defined GPS_H
  #include "GPS.h"
  #define GPS_H
#endif

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
Point convertSphereToPlan(float lat, float lon){
  return convertSphereToPlan(lat, lon, rover_config.getCentreRepere());
}

// conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale
float convertDegMinSecToDecimal(float deg, float minutes, float sec){
  minutes += sec /60;
  deg += minutes / 60;
  return deg;
}

/*// retourne la position GPS en mètre, position absolue (par rapport à l'intersection Greenwich/équateur
Point pointGPS_absolu(){
  return convertSphereToPlan(gps.latitude, gps.longitude, Point(O,O,O));
}

// retourne la position GPS en mètre, par rapport au centre de la carte
Point pointGPS_carte(){
  return convertSphereToPlan(gps.latitude, gps.longitude, getCentreRepere());
}*/
