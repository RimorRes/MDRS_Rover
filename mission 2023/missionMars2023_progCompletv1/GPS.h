/*****************************************************************
 * Définition des fonctions utiles à la gestion de la position GPS
******************************************************************/

#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 

boolean GPS_init();
void testGPS();
Point convertSphereToPlanGPS(float lat, float lon, Point origine); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
Point convertSphereToPlanGPS(float lat, float lon); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
float convertDegMinSecToDecimalGPS(float deg, float minutes, float sec);  // conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale
